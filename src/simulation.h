using namespace std;

class Event
{

public:
    double t;
    double rage;
    string source;
    EventType type;
    shared_ptr<spell::Spell> spell;
    shared_ptr<buff::Buff> buff;
    shared_ptr<debuff::Debuff> debuff;
    shared_ptr<cooldown::Cooldown> cooldown;
    shared_ptr<dot::Dot> dot;

};

class Simulation
{

public:
    bool logging = true;
    list<shared_ptr<Event>> queue;
    vector<shared_ptr<LogEntry>> log;
    shared_ptr<State> state;
    shared_ptr<Player> player;
    shared_ptr<Config> config;

    Simulation(shared_ptr<Config> _config, shared_ptr<Player> _player)
    {
        config = _config;
        player = _player;
        state = make_shared<State>(config);
    }

    void reset()
    {
        clearLog();
        state->reset();
        state->rage = 0;
    }

    SimulationsResult runMultiple(int iterations)
    {
        SimulationResult r;
        SimulationsResult result;

        logging = false;

        double bin_size = 20;
        int bin;
        map<int, int> histogram;
        ostringstream results;

        if (config->additional_data)
            results << "DPS,Duration\n";

        for (int i=0; i<iterations; i++) {
            if (config->rng_seed)
                setRNGSeed(config->rng_seed+i);

            r = run();

            if (i == 0 || r.dps < result.min_dps)
                result.min_dps = r.dps;
            if (i == 0 || r.dps > result.max_dps)
                result.max_dps = r.dps;
            result.avg_dps+= (r.dps - result.avg_dps) / (i+1);

            bin = floor(r.dps/bin_size)*bin_size;
            if (histogram.find(bin) != histogram.end())
                histogram[bin]++;
            else
                histogram[bin] = 1;

            if (config->additional_data)
                results << r.dps << "," << r.t << "\n";
        }

        result.iterations = iterations;

        if (config->additional_data)
            result.all_results = results.str();

        // Histogram json string
        ostringstream ss;
        ss << "{";
        for (auto itr = histogram.begin(); itr != histogram.end(); itr++) {
            if (itr != histogram.begin())
                ss << ",";
            ss << "\"" << itr->first << "\":" << itr->second;
        }
        ss << "}";
        result.histogram = ss.str();

        return result;
    }

    SimulationResult run()
    {
        reset();

        // Start auto attacks
        pushMeleeMainHand(config->mh_weapon_speed);
        if (config->dual_wield)
            pushMeleeOffHand(config->oh_weapon_speed);

        // Setup bloodrage ticks
        if (config->bloodrage) {
            for (int i=0; i<config->bloodrage_t.size(); i++)
                pushBloodrage(config->bloodrage_t.at(i));
        }

        // Setup bloodlust
        if (config->bloodlust) {
            for (int i=0; i<config->bloodlust_t.size(); i++)
                pushBuffGain(make_shared<buff::Bloodlust>(), config->bloodlust_t.at(i));
        }

        // Setup drums
        if (config->drums && config->drums_friend) {
            double t = 0;
            for (int i=0; i<config->drums_t.size(); i++) {
                if (config->drums_t.at(i) >= t)
                    t = config->drums_t.at(i);
                pushDrums(t);
            }
            for (t+= 120; t<state->duration; t+= 120)
                pushDrums(t);
        }

        // Setup debuffs
        if (config->sunder_armor) {
            pushDebuffGain(make_shared<debuff::SunderArmor>(), 0);
            for (double t=30; t<state->duration; t+= 30)
                pushDebuffGain(make_shared<debuff::SunderArmor>(), t);
        }

        useCooldowns();
        workCurrent();

        cast(nextSpell());

        work();

        SimulationResult result;
        result.dmg = state->dmg;
        result.t = state->t;
        result.dps = state->dmg/state->t;

        if (logging) {
            result.log = jsonLog();
            result.spells = spellStats();
        }

        return result;
    }

    void workCurrent()
    {
        shared_ptr<Event> event;

        while (true) {
            event = queue.front();
            if (event->t != state->t)
                return;
            queue.pop_front();
            tick(event);
        }
    }

    void work()
    {
        shared_ptr<Event> event;

        while (true) {
            event = queue.front();
            queue.pop_front();

            if (event->t >= state->duration) {
                state->t = state->duration;
                break;
            }

            tick(event);
        }

        queue.clear();
    }

    void tick(shared_ptr<Event> event)
    {
        state->t = event->t;

        if (event->type == EVENT_CAST)
            cast(event->spell);
        else if (event->type == EVENT_SPELL)
            onCast(event->spell);
        else if (event->type == EVENT_MELEE_MAIN_HAND)
            onMeleeMainHand();
        else if (event->type == EVENT_MELEE_OFF_HAND)
            onMeleeOffHand();
        else if (event->type == EVENT_RAGE_GAIN)
            onRageGain(event->rage, event->source);
        else if (event->type == EVENT_BUFF_GAIN)
            onBuffGain(event->buff);
        else if (event->type == EVENT_BUFF_EXPIRE)
            onBuffExpire(event->buff);
        else if (event->type == EVENT_DEBUFF_GAIN)
            onDebuffGain(event->debuff);
        else if (event->type == EVENT_DEBUFF_EXPIRE)
            onDebuffExpire(event->debuff);
        else if (event->type == EVENT_DOT)
            onDot(event->dot);
        else if (event->type == EVENT_CD_GAIN)
            onCooldownGain(event->cooldown);
        else if (event->type == EVENT_CD_EXPIRE)
            onCooldownExpire(event->cooldown);
        else if (event->type == EVENT_BLOODRAGE)
            bloodrage();
        else if (event->type == EVENT_BLOODRAGE_TICK)
            bloodrageTick();
        else if (event->type == EVENT_DRUMS)
            useDrums();
        else if (event->type == EVENT_WAIT)
            onWait();
    }

    void push(shared_ptr<Event> event)
    {
        event->t+= state->t;

        for (auto itr = queue.begin(); itr != queue.end(); itr++) {
            if (event->t < (*itr)->t) {
                queue.insert(itr, event);
                return;
            }
        }

        queue.push_back(event);
    }

    void pushCast(shared_ptr<spell::Spell> spell, double t)
    {
        shared_ptr<Event> event(new Event());
        event->type = EVENT_CAST;
        event->spell = spell;
        event->t = t;

        push(event);
    }

    void pushSpell(shared_ptr<spell::Spell> spell, double t)
    {
        shared_ptr<Event> event(new Event());
        event->type = EVENT_SPELL;
        event->spell = spell;
        event->t = t;

        push(event);
    }

    void pushMeleeMainHand(double t)
    {
        shared_ptr<Event> event(new Event());
        event->type = EVENT_MELEE_MAIN_HAND;
        event->t = t;
        push(event);
    }

    void pushMeleeOffHand(double t)
    {
        shared_ptr<Event> event(new Event());
        event->type = EVENT_MELEE_OFF_HAND;
        event->t = t;
        push(event);
    }

    void pushRageGain(double t, double rage, string source = "")
    {
        shared_ptr<Event> event(new Event());
        event->type = EVENT_RAGE_GAIN;
        event->t = t;
        event->rage = rage;
        event->source = source;

        push(event);
    }

    void pushBuffGain(shared_ptr<buff::Buff> buff, double t)
    {
        shared_ptr<Event> event(new Event());
        event->type = EVENT_BUFF_GAIN;
        event->t = t;
        event->buff = buff;

        push(event);
    }

    void pushBuffExpire(shared_ptr<buff::Buff> buff, double t = 0)
    {
        shared_ptr<Event> event(new Event());
        event->type = EVENT_BUFF_EXPIRE;
        event->t = t == 0 ? buff->duration : t;
        event->buff = buff;

        push(event);
    }

    void pushDebuffGain(shared_ptr<debuff::Debuff> debuff, double t)
    {
        shared_ptr<Event> event(new Event());
        event->type = EVENT_DEBUFF_GAIN;
        event->t = t;
        event->debuff = debuff;

        push(event);
    }

    void pushDebuffExpire(shared_ptr<debuff::Debuff> debuff)
    {
        shared_ptr<Event> event(new Event());
        event->type = EVENT_DEBUFF_EXPIRE;
        event->t = debuff->duration;
        event->debuff = debuff;

        push(event);
    }

    void pushDot(shared_ptr<dot::Dot> dot)
    {
        if (!dot->stackable && dot->tick == 0)
            removeDot(dot);

        shared_ptr<Event> event(new Event());
        event->type = EVENT_DOT;
        event->t = dot->t_interval;
        event->dot = dot;

        push(event);
    }

    void pushCooldownGain(shared_ptr<cooldown::Cooldown> cooldown, double t)
    {
        shared_ptr<Event> event(new Event());
        event->type = EVENT_CD_GAIN;
        event->t = t;
        event->cooldown = cooldown;

        push(event);
    }

    void pushCooldownExpire(shared_ptr<cooldown::Cooldown> cooldown)
    {
        shared_ptr<Event> event(new Event());
        event->type = EVENT_CD_EXPIRE;
        event->t = cooldown->duration;
        event->cooldown = cooldown;

        push(event);
    }

    void pushDrums(double t)
    {
        shared_ptr<Event> event(new Event());
        event->type = EVENT_DRUMS;
        event->t = t;

        push(event);
    }

    void pushBloodrage(double t)
    {
        shared_ptr<Event> event(new Event());
        event->type = EVENT_BLOODRAGE;
        event->t = t;

        push(event);
    }

    void pushBloodrageTick()
    {
        shared_ptr<Event> event(new Event());
        event->type = EVENT_BLOODRAGE_TICK;
        event->t = 1;

        push(event);
    }

    void pushWait(double t)
    {
        shared_ptr<Event> event(new Event());
        event->type = EVENT_WAIT;
        event->t = t;

        push(event);

        ostringstream s;
        s << std::fixed << std::setprecision(2);
        s << "Out of rage, waiting " << t << " seconds...";
        addLog(LOG_WAIT, s.str());
    }

    void cast(shared_ptr<spell::Spell> spell)
    {
        if (canCast(spell)) {
            if (spell->on_gcd && state->t_gcd > state->t) {
                pushCast(spell, state->t_gcd - state->t);
            }
            else {
                useCooldowns();

                if (spell->on_gcd)
                    state->t_gcd = state->t + gcd();

                if (spell->cast_time > 0)
                    pushSpell(spell, spell->cast_time);
                else
                    onCast(spell);
            }
        }
        else {
            pushWait(0.5);
        }
    }

    void onCast(shared_ptr<spell::Spell> spell)
    {
        shared_ptr<spell::Spell> next = NULL;

        if (canCast(spell)) {
            onCastDmg(spell);
            onCastSuccess(spell);

            if (!spell->done || spell->proc)
                return;

            next = onCastComplete(spell);
        }
        else {
            next = nextSpell();
        }

        if (next != NULL) {
            // Drums 1 sec cast
            if (config->drums && isTimerReady(config->drums_t) && !state->hasCooldown(cooldown::DRUMS) && !config->drums_friend) {
                useDrums();
                pushCast(next, 1.0);
            }
            else {
                cast(next);
            }
        }
    }

    void onCastSuccess(shared_ptr<spell::Spell> spell)
    {
        if (spell->proc)
            return;

        spell->actual_cost = rageCost(spell);
        state->rage-= spell->actual_cost;

        if (state->spells.find(spell->id) == state->spells.end())
            state->spells[spell->id].name = spell->name;
        state->spells[spell->id].casts++;

        // Cooldowns
        if (spell->cooldown > 0) {
            if (spell->id == spell::BLOODTHIRST)
                onCooldownGain(make_shared<cooldown::Bloodthirst>());
            else if (spell->id == spell::MORTAL_STRIKE)
                onCooldownGain(make_shared<cooldown::MortalStrike>());
            else if (spell->id == spell::WHIRLWIND)
                onCooldownGain(make_shared<cooldown::Whirlwind>());
        }

        // Unbridled Wrath proc (1% per talent point per hit)
        if (player->talents.unbridled_wrath && random<int>(0, 99) < player->talents.unbridled_wrath)
            onRageGain(1, "Unbridled Wrath");
    }

    void onCastDmg(shared_ptr<spell::Spell> spell)
    {
        if (state->spells.find(spell->id) == state->spells.end())
            state->spells[spell->id].name = spell->name;

        spell->done = true;
        spell->result = attackRoll(spell);
        
        if (spell->result == spell::MISS) {
            spell->misses++;
            state->spells[spell->id].misses++;
        }
        else if (spell->result == spell::DODGE) {
            spell->dodges++;
            state->spells[spell->id].dodges++;
        }
        else if (spell->result == spell::PARRY) {
            spell->parries++;
            state->spells[spell->id].parries++;
        }
        else if (spell->result == spell::GLANCE) {
            spell->glances++;
            state->spells[spell->id].glances++;
        }
        else if (spell->result == spell::CRIT) {
            spell->crits++;
            state->spells[spell->id].crits++;
        }
        else {
            spell->hits++;
            state->spells[spell->id].hits++;
        }

        if (spell->result != spell::MISS && spell->result != spell::DODGE && spell->result != spell::PARRY) {
            spell->dmg = attackDmg(spell);

            if (spell->result == spell::CRIT)
                spell->dmg*= critMultiplier(spell);
            else if (spell->result == spell::GLANCE)
                spell->dmg*= glanceMultiplier();

            spell->dmg = round(spell->dmg);

            state->dmg+= spell->dmg;

            state->spells[spell->id].dmg+= spell->dmg;
            if (spell->dmg > state->spells[spell->id].max_dmg)
                state->spells[spell->id].max_dmg = spell->dmg;
            if (spell->dmg < state->spells[spell->id].min_dmg || state->spells[spell->id].min_dmg == 0)
                state->spells[spell->id].min_dmg = spell->dmg;
        }

        logAttackDmg(spell);

        if (spell->proc)
            return;

        // Generate rage from damage dealt (for white hits)
        if (spell->is_melee && !spell->normalized && spell->result != spell::MISS && spell->result != spell::DODGE && spell->result != spell::PARRY) {
            double rage = spell->dmg / 274.7 * 7.5;
            if (spell->id == spell::MELEE_OFF_HAND)
                rage*= 0.5;
            onRageGain(rage, "Melee");
        }

        if (spell->result != spell::MISS && spell->result != spell::DODGE && spell->result != spell::PARRY) {
            // Flurry proc on crit
            if (spell->result == spell::CRIT && player->talents.flurry) {
                state->flurry_charges = 3;
                onBuffGain(make_shared<buff::Flurry>());
            }

            // Rampage
            if (spell->result == spell::CRIT && player->talents.rampage)
                onBuffGain(make_shared<buff::Rampage>());

            // Deep Wounds
            if (spell->result == spell::CRIT && player->talents.deep_wounds)
                addDeepWounds(spell);

            // Trinket procs
            if (hasTrinket(TRINKET_CRUSADE))
                onBuffGain(make_shared<buff::Crusade>());

            // 10% proc rate
            if (hasTrinket(TRINKET_DRAGONSPINE_TROPHY) && !state->hasCooldown(cooldown::DRAGONSPINE_TROPHY) && random<int>(0, 9) == 0) {
                onCooldownGain(make_shared<cooldown::DragonspineTrophy>());
                onBuffGain(make_shared<buff::DragonspineTrophy>());
            }

            // 15% proc rate
            if (hasTrinket(TRINKET_SHARD_CONTEMPT) && !state->hasCooldown(cooldown::SHARD_CONTEMPT) && random<int>(0, 19) < 3) {
                onCooldownGain(make_shared<cooldown::ShardContempt>());
                onBuffGain(make_shared<buff::ShardContempt>());
            }

            // 10% proc rate
            if (hasTrinket(TRINKET_MADNESS) && !state->hasCooldown(cooldown::MADNESS) && random<int>(0, 9) == 0) {
                onCooldownGain(make_shared<cooldown::Madness>());
                onBuffGain(make_shared<buff::Madness>());
            }

            // 10% proc rate
            if (hasTrinket(TRINKET_BLOODMOON) && !state->hasCooldown(cooldown::BLOODMOON) && random<int>(0, 9) == 0) {
                onCooldownGain(make_shared<cooldown::Bloodmoon>());
                onBuffGain(make_shared<buff::Bloodmoon>());
            }
        }
    }

    shared_ptr<spell::Spell> onCastComplete(shared_ptr<spell::Spell> spell)
    {
        shared_ptr<spell::Spell> next = NULL;

        if (shouldUsePotion())
            usePotion();

        next = nextSpell(spell);

        return next;
    }

    void onMeleeMainHand()
    {
        shared_ptr<spell::Spell> spell = make_shared<spell::MeleeMainHand>();
        
        // Check for Heroic Strike queue
        if (shouldHeroicStrike()) {
            spell = make_shared<spell::HeroicStrike>();
        }

        onCastDmg(spell);
        onCastSuccess(spell);

        // Schedule next swing
        double swing_time = config->mh_weapon_speed / swingHaste();
        pushMeleeMainHand(swing_time);
    }

    void onMeleeOffHand()
    {
        shared_ptr<spell::Spell> spell = make_shared<spell::MeleeOffHand>();
        
        onCastDmg(spell);
        onCastSuccess(spell);

        // Schedule next swing
        double swing_time = config->oh_weapon_speed / swingHaste();
        pushMeleeOffHand(swing_time);
    }

    void onRageGain(double rage, string source = "")
    {
        state->rage = min(RAGE_CAP, state->rage + rage);
        logRageGain(rage, source);
    }

    void onDot(shared_ptr<dot::Dot> dot)
    {
        state->dmg+= dot->dmg;
        logDotDmg(dot);

        dot->onTick();

        if (dot->tick < dot->ticks)
            pushDot(dot);
    }

    void onWait()
    {
        cast(nextSpell());
    }

    void onBuffGain(shared_ptr<buff::Buff> buff)
    {
        int stacks = state->addBuff(buff);
        removeBuffExpiration(buff);
        pushBuffExpire(buff);

        if (stacks)
            logBuffGain(buff, stacks);
    }

    void onBuffExpire(shared_ptr<buff::Buff> buff)
    {
        removeBuffExpiration(buff);
        logBuffExpire(buff);
        state->removeBuff(buff->id);

        if (buff->id == buff::FLURRY)
            state->flurry_charges = 0;
    }

    void onDebuffGain(shared_ptr<debuff::Debuff> debuff)
    {
        int stacks = state->addDebuff(debuff);
        removeDebuffExpiration(debuff);
        pushDebuffExpire(debuff);

        if (stacks)
            logDebuffGain(debuff, stacks);
    }

    void onDebuffExpire(shared_ptr<debuff::Debuff> debuff)
    {
        removeDebuffExpiration(debuff);
        logDebuffExpire(debuff);
        state->removeDebuff(debuff->id);
    }

    void onCooldownGain(shared_ptr<cooldown::Cooldown> cooldown)
    {
        removeCooldownExpiration(cooldown);
        state->addCooldown(cooldown);
        pushCooldownExpire(cooldown);
        if (cooldown->id == cooldown::POTION && !state->used_pot)
            state->used_pot = true;
    }

    void onCooldownExpire(shared_ptr<cooldown::Cooldown> cooldown)
    {
        state->removeCooldown(cooldown->id);
    }

    double timeRemain()
    {
        return state->duration - state->t;
    }

    shared_ptr<spell::Spell> nextSpell(shared_ptr<spell::Spell> prev = NULL)
    {
        shared_ptr<spell::Spell> next = NULL;

        // Check execute phase
        if (!state->execute_phase && timeRemain() / state->duration * 100 <= config->execute_phase_start) {
            state->execute_phase = true;
            addLog(LOG_DEBUG, "Execute phase started");
        }

        // Execute phase
        if (state->execute_phase && config->use_execute) {
            if (state->rage >= 15)
                return make_shared<spell::Execute>();
        }

        // Main rotation abilities
        if (config->main_rotation == MAIN_ROTATION_BT) {
            if (!state->hasCooldown(cooldown::BLOODTHIRST) && player->talents.bloodthirst)
                return make_shared<spell::Bloodthirst>();
        }
        else if (config->main_rotation == MAIN_ROTATION_MS) {
            if (!state->hasCooldown(cooldown::MORTAL_STRIKE) && player->talents.mortal_strike)
                return make_shared<spell::MortalStrike>();
        }

        // Whirlwind
        if (config->use_whirlwind && !state->hasCooldown(cooldown::WHIRLWIND))
            return make_shared<spell::Whirlwind>();

        // Default: wait for cooldowns
        return NULL;
    }

    bool shouldHeroicStrike()
    {
        if (!config->use_heroic_strike)
            return false;

        if (state->rage < config->heroic_strike_rage_threshold)
            return false;

        return true;
    }

    void addDeepWounds(shared_ptr<spell::Spell> spell)
    {
        double dmg = spell->dmg * 0.16 * player->talents.deep_wounds;

        for (auto itr = queue.begin(); itr != queue.end(); itr++) {
            if ((*itr)->type == EVENT_DOT && (*itr)->dot->id == dot::DEEP_WOUNDS) {
                (*itr)->dot->stack(dmg);
                (*itr)->t = state->t + (*itr)->dot->t_interval;
                return;
            }
        }

        pushDot(make_shared<dot::DeepWounds>(dmg));
    }

    void useCooldowns()
    {
        if (isTimerReady(config->death_wish_t) && !state->hasCooldown(cooldown::DEATH_WISH) && player->talents.death_wish && config->death_wish)
            useDeathWish();
        if (isTimerReady(config->recklessness_t) && !state->hasCooldown(cooldown::RECKLESSNESS) && config->recklessness)
            useRecklessness();

        if (!state->hasCooldown(cooldown::POTION) && nextPotion() != POTION_NONE && isTimerReady(config->potion_t))
            usePotion();

        if (!state->hasCooldown(cooldown::TRINKET1) && !isTrinketOnSharedCD(config->trinket1) && isTimerReady(config->trinket1_t))
            useTrinket(config->trinket1, cooldown::TRINKET1);

        if (!state->hasCooldown(cooldown::TRINKET2) && !isTrinketOnSharedCD(config->trinket2) && isTimerReady(config->trinket2_t))
            useTrinket(config->trinket2, cooldown::TRINKET2);
    }

    void useTrinket(Trinket trinket_id, cooldown::ID cd)
    {
        double duration = 120;
        shared_ptr<buff::Buff> buff = NULL;

        if (trinket_id == TRINKET_BLOODLUST_BROOCH)
            buff = make_shared<buff::BloodlustBrooch>();
        if (trinket_id == TRINKET_ABACUS)
            buff = make_shared<buff::Abacus>();
        if (trinket_id == TRINKET_TSUNAMI_TALISMAN)
            buff = make_shared<buff::TsunamiTalisman>();
        if (trinket_id == TRINKET_HOURGLASS)
            buff = make_shared<buff::Hourglass>();
        if (trinket_id == TRINKET_SLAYERS_CREST) {
            buff = make_shared<buff::SlayersCrest>();
            duration = 120;
        }
        if (trinket_id == TRINKET_BADGE_TENACITY) {
            buff = make_shared<buff::BadgeTenacity>();
            duration = 90;
        }
        if (trinket_id == TRINKET_ICON_UNYIELDING_COURAGE) {
            buff = make_shared<buff::IconUnyieldingCourage>();
            duration = 120;
        }
        if (trinket_id == TRINKET_BLACKENED_NAARU_SLIVER) {
            buff = make_shared<buff::BlackenedNaaruSliver>();
            duration = 120;
        }
        if (trinket_id == TRINKET_ROMULOS_POISON_VIAL) {
            buff = make_shared<buff::RomulosPoisonVial>();
            duration = 120;
        }

        if (buff != NULL) {
            onBuffGain(buff);
            onCooldownGain(make_shared<cooldown::Cooldown>(cd, duration));
            if (trinketSharesCD(trinket_id))
                onCooldownGain(make_shared<cooldown::TrinketShared>(buff->duration));
        }
    }

    bool isTrinketOnSharedCD(Trinket trinket_id)
    {
        if (!trinketSharesCD(trinket_id))
            return false;
        return state->hasCooldown(cooldown::TRINKET_SHARED);
    }

    bool trinketSharesCD(Trinket trinket_id)
    {
        if (trinket_id == TRINKET_BERSERKERS_CALL)
            return false;

        return true;
    }

    void useDrums()
    {
        shared_ptr<buff::Buff> buff = NULL;

        if (config->drums == DRUMS_OF_BATTLE)
            buff = make_shared<buff::DrumsOfBattle>();
        else if (config->drums == DRUMS_OF_WAR)
            buff = make_shared<buff::DrumsOfWar>();
        else
            return;

        if (config->drums_friend) {
            onCooldownGain(make_shared<cooldown::Drums>());
            onBuffGain(buff);
        }
        else {
            pushCooldownGain(make_shared<cooldown::Drums>(), 1.0);
            pushBuffGain(buff, 1.0);
        }
    }

    void usePotion()
    {
        if (nextPotion() == POTION_HASTE)
            onBuffGain(make_shared<buff::HastePotion>());
        else if (nextPotion() == POTION_INSANE_STRENGTH)
            onBuffGain(make_shared<buff::InsaneStrengthPotion>());
        else
            return;

        onCooldownGain(make_shared<cooldown::Potion>());
    }

    void useDeathWish()
    {
        onCooldownGain(make_shared<cooldown::DeathWish>());
        onBuffGain(make_shared<buff::DeathWish>());
    }

    void useRecklessness()
    {
        onCooldownGain(make_shared<cooldown::Recklessness>());
        onBuffGain(make_shared<buff::Recklessness>());
    }

    void bloodrage()
    {
        onCooldownGain(make_shared<cooldown::Bloodrage>());
        onBuffGain(make_shared<buff::Bloodrage>());
        onRageGain(10, "Bloodrage");

        // Schedule ticks
        for (int i=1; i<=10; i++)
            pushBloodrageTick();
    }

    void bloodrageTick()
    {
        onRageGain(1, "Bloodrage");
    }

    bool hasTrinket(Trinket trinket)
    {
        return config->trinket1 == trinket || config->trinket2 == trinket;
    }

    void removeBuffExpiration(shared_ptr<buff::Buff> buff)
    {
        for (auto itr = queue.begin(); itr != queue.end(); itr++) {
            if ((*itr)->type == EVENT_BUFF_EXPIRE && (*itr)->buff->id == buff->id) {
                queue.erase(itr);
                return;
            }
        }
    }

    void removeDebuffExpiration(shared_ptr<debuff::Debuff> debuff)
    {
        for (auto itr = queue.begin(); itr != queue.end(); itr++) {
            if ((*itr)->type == EVENT_DEBUFF_EXPIRE && (*itr)->debuff->id == debuff->id) {
                queue.erase(itr);
                return;
            }
        }
    }

    void removeCooldownExpiration(shared_ptr<cooldown::Cooldown> cooldown)
    {
        for (auto itr = queue.begin(); itr != queue.end(); itr++) {
            if ((*itr)->type == EVENT_CD_EXPIRE && (*itr)->cooldown->id == cooldown->id) {
                queue.erase(itr);
                return;
            }
        }
    }

    void removeDot(shared_ptr<dot::Dot> dot)
    {
        for (auto itr = queue.begin(); itr != queue.end(); itr++) {
            if ((*itr)->type == EVENT_DOT && (*itr)->dot->id == dot->id) {
                queue.erase(itr);
                return;
            }
        }
    }

    double buffDuration(buff::ID id)
    {
        for (auto itr = queue.begin(); itr != queue.end(); itr++) {
            if ((*itr)->type == EVENT_BUFF_EXPIRE && (*itr)->buff->id == id)
                return (*itr)->t - state->t;
        }

        return 0;
    }

    bool canCast(shared_ptr<spell::Spell> spell)
    {
        return state->rage >= rageCost(spell);
    }

    double rageCost(shared_ptr<spell::Spell> spell)
    {
        double cost = spell->cost;

        // Improved Heroic Strike
        if (spell->id == spell::HEROIC_STRIKE && player->talents.imp_heroic_strike)
            cost-= player->talents.imp_heroic_strike;

        // Focused Rage
        if (player->talents.focused_rage)
            cost-= cost * 0.03 * player->talents.focused_rage;

        // Intensify Rage
        if (player->talents.intensify_rage && (spell->id == spell::BLOODTHIRST || spell->id == spell::MORTAL_STRIKE || spell->id == spell::WHIRLWIND))
            cost-= cost * 0.05 * player->talents.intensify_rage;

        return max(0.0, cost);
    }

    double gcd()
    {
        double t = 1.5;
        double cap = GCD_MIN;

        t/= castHaste();

        if (t < cap)
            t = cap;

        return t;
    }

    double castHaste()
    {
        double haste = 1;
        double phaste = player->stats.haste;
        double rating = 0;

        if (state->hasBuff(buff::BLOODLUST))
            haste*= 1.3;
        if (state->hasBuff(buff::DRUMS_OF_BATTLE))
            rating+= 80;
        if (state->hasBuff(buff::HASTE_POTION))
            rating+= 400;

        if (rating)
            phaste+= hasteRatingToHaste(rating);

        haste+= phaste/100.0;

        return haste;
    }

    double swingHaste()
    {
        double haste = castHaste();

        // Flurry
        if (state->hasBuff(buff::FLURRY) && state->flurry_charges > 0) {
            haste*= 1.0 + (0.05 * player->talents.flurry);
            state->flurry_charges--;
            if (state->flurry_charges == 0)
                onBuffExpire(make_shared<buff::Flurry>());
        }

        // Windfury Totem (not haste, but increases swing speed)
        if (config->windfury_totem)
            haste*= 1.16;

        return haste;
    }

    double hitChance(shared_ptr<spell::Spell> spell)
    {
        double hit = 91.0 + player->stats.hit;

        // Dual wield penalty
        if (config->dual_wield && spell->is_melee)
            hit-= 19;

        return min(hit, 99.0);
    }

    double critChance(shared_ptr<spell::Spell> spell)
    {
        double crit = player->stats.crit;

        // Rampage
        if (state->hasBuff(buff::RAMPAGE))
            crit+= 5.0;

        // Leader of the Pack
        if (config->leader_of_the_pack)
            crit+= 5.0;

        // Recklessness
        if (state->hasBuff(buff::RECKLESSNESS))
            crit+= 100.0;

        // Enrage
        if (state->hasBuff(buff::ENRAGE))
            crit+= 10.0 * player->talents.enrage / 5.0;

        return crit;
    }

    double critMultiplier(shared_ptr<spell::Spell> spell)
    {
        double base = 2.0;
        double talents = 1;

        if (config->meta_gem == META_RELENTLESS_EARTHSTORM)
            base*= 1.03;
        if (config->meta_gem == META_CHAOTIC_SKYFIRE)
            base*= 1.03;

        if (spell->proc)
            return base;

        // Impale
        if (player->talents.impale)
            talents+= player->talents.impale * 0.1;

        return (base - 1) * talents + 1;
    }

    double glanceMultiplier()
    {
        // Boss is level 73, glancing blows do ~75% damage
        return 0.75;
    }

    double buffDmgMultiplier(shared_ptr<spell::Spell> spell)
    {
        double multi = 1;

        // Death Wish
        if (state->hasBuff(buff::DEATH_WISH))
            multi*= 1.2;

        // Enrage
        if (state->hasBuff(buff::ENRAGE))
            multi*= 1.0 + (0.05 * player->talents.enrage / 5.0);

        // Two-Handed Weapon Specialization
        if (!config->dual_wield && player->talents.two_handed_spec)
            multi*= 1.0 + (player->talents.two_handed_spec * 0.01);

        // Dual Wield Specialization (only for special attacks)
        if (config->dual_wield && player->talents.dual_wield_spec && spell->normalized)
            multi*= 1.0 + (player->talents.dual_wield_spec * 0.01);

        // Trinket buffs
        if (state->hasBuff(buff::BLOODLUST_BROOCH))
            multi*= 1.1;
        if (state->hasBuff(buff::ABACUS))
            multi*= 1.0 + (state->buffStacks(buff::ABACUS) * 0.01);
        if (state->hasBuff(buff::TSUNAMI_TALISMAN))
            multi*= 1.05;
        if (state->hasBuff(buff::HOURGLASS))
            multi*= 1.05;
        if (state->hasBuff(buff::SLAYERS_CREST))
            multi*= 1.1;
        if (state->hasBuff(buff::BADGE_TENACITY))
            multi*= 1.05;
        if (state->hasBuff(buff::ICON_UNYIELDING_COURAGE))
            multi*= 1.1;
        if (state->hasBuff(buff::DRAGONSPINE_TROPHY))
            multi*= 1.05;
        if (state->hasBuff(buff::SHARD_CONTEMPT))
            multi*= 1.1;
        if (state->hasBuff(buff::MADNESS))
            multi*= 1.05;
        if (state->hasBuff(buff::BLACKENED_NAARU_SLIVER))
            multi*= 1.1;
        if (state->hasBuff(buff::ROMULOS_POISON_VIAL))
            multi*= 1.1;
        if (state->hasBuff(buff::BLOODMOON))
            multi*= 1.05;
        if (state->hasBuff(buff::CRUSADE))
            multi*= 1.0 + (state->buffStacks(buff::CRUSADE) * 0.008);

        // Potion buffs
        if (state->hasBuff(buff::INSANE_STRENGTH_POTION))
            multi*= 1.05;

        return multi;
    }

    double debuffDmgMultiplier(shared_ptr<spell::Spell> spell)
    {
        double multi = 1;

        // Blood Frenzy
        if (config->blood_frenzy)
            multi*= 1.04;

        // Curse of Recklessness
        if (config->curse_of_recklessness)
            multi*= 1.03;

        return multi;
    }

    double attackPower()
    {
        double ap = player->stats.attack_power;

        // Insane Strength Potion
        if (state->hasBuff(buff::INSANE_STRENGTH_POTION))
            ap+= 120;

        return ap;
    }

    double attackDmg(shared_ptr<spell::Spell> spell)
    {
        double dmg;
        double weapon_dmg;
        double ap = attackPower();

        // Calculate weapon damage
        if (spell->id == spell::MELEE_OFF_HAND || spell->id == spell::HEROIC_STRIKE) {
            if (config->avg_spell_dmg)
                weapon_dmg = (config->oh_low_dmg + config->oh_high_dmg) / 2.0;
            else
                weapon_dmg = random<double>(config->oh_low_dmg, config->oh_high_dmg);
        }
        else {
            if (config->avg_spell_dmg)
                weapon_dmg = (config->mh_low_dmg + config->mh_high_dmg) / 2.0;
            else
                weapon_dmg = random<double>(config->mh_low_dmg, config->mh_high_dmg);
        }

        // Apply weapon multiplier
        weapon_dmg*= spell->weapon_multiplier;

        // Normalized attacks use normalized weapon speed
        if (spell->normalized) {
            double normalized_speed = config->dual_wield ? 2.4 : 3.3;
            dmg = weapon_dmg + (ap / 14.0) * normalized_speed;
        }
        else {
            double weapon_speed = (spell->id == spell::MELEE_OFF_HAND) ? config->oh_weapon_speed : config->mh_weapon_speed;
            dmg = weapon_dmg + (ap / 14.0) * weapon_speed;
        }

        // Add bonus damage
        dmg+= spell->bonus_dmg;

        // Execute consumes extra rage for damage
        if (spell->id == spell::EXECUTE) {
            double extra_rage = min(state->rage - 15, 15.0);
            dmg+= extra_rage * 21;
        }

        // Apply multipliers
        dmg*= buffDmgMultiplier(spell);
        dmg*= debuffDmgMultiplier(spell);

        return dmg;
    }

    spell::Result attackRoll(shared_ptr<spell::Spell> spell)
    {
        // Miss roll
        if (random<double>(0, 100) > hitChance(spell))
            return spell::MISS;

        // Dodge roll (6.5% base for boss)
        double dodge_chance = 6.5 - player->stats.expertise * 0.25;
        if (random<double>(0, 100) <= dodge_chance)
            return spell::DODGE;

        // Parry roll (14% base for boss, only from front)
        double parry_chance = 14.0 - player->stats.expertise * 0.25;
        if (random<double>(0, 100) <= parry_chance)
            return spell::PARRY;

        // Glance roll (only for white hits)
        if (spell->can_glance && random<double>(0, 100) <= 24.0)
            return spell::GLANCE;

        // Crit roll
        if (random<double>(0, 100) <= critChance(spell))
            return spell::CRIT;

        return spell::HIT;
    }

    bool shouldUsePotion()
    {
        if (nextPotion() == POTION_NONE)
            return false;

        if (state->hasCooldown(cooldown::POTION))
            return false;

        return isTimerReady(config->potion_t);
    }

    Potion nextPotion()
    {
        if (state->used_pot || config->first_potion == POTION_NONE)
            return config->potion;
        return config->first_potion;
    }

    bool isTimerReady(vector<double>& v, double t = -1)
    {
        if (t == -1)
            t = state->t;

        for (int i=0; i<v.size(); i++) {
            if (v.at(i) > t)
                return false;
            if (v.at(i) <= t && v.at(i) + 20 > t)
                return true;
        }

        return true;
    }

    void logAttackDmg(shared_ptr<spell::Spell> spell)
    {
        if (!logging)
            return;

        ostringstream s;

        s << spell->name;
        if (spell->result == spell::MISS)
            s << " missed";
        else if (spell->result == spell::DODGE)
            s << " was dodged";
        else if (spell->result == spell::PARRY)
            s << " was parried";
        else if (spell->result == spell::GLANCE)
            s << " glanced for " << spell->dmg;
        else if (spell->result == spell::CRIT)
            s << " crit for " << spell->dmg;
        else
            s << " hit for " << spell->dmg;

        addLog(LOG_MELEE, s.str());
    }

    void logDotDmg(shared_ptr<dot::Dot> dot)
    {
        if (!logging)
            return;

        ostringstream s;

        s << dot->name << " ticks for " << dot->dmg;

        addLog(LOG_DOT, s.str());
    }

    void logBuffGain(shared_ptr<buff::Buff> buff, int stacks = 1)
    {
        if (!logging || buff->hidden)
            return;

        ostringstream s;

        s << "Gained " << buff->name;
        if (buff->max_stacks > 1)
            s << " (" << stacks << ")";

        addLog(LOG_BUFF, s.str());
    }

    void logBuffExpire(shared_ptr<buff::Buff> buff)
    {
        if (!logging || buff->hidden)
            return;

        ostringstream s;

        s << "Lost " << buff->name;

        addLog(LOG_BUFF, s.str());
    }

    void logDebuffGain(shared_ptr<debuff::Debuff> debuff, int stacks = 1)
    {
        if (!logging || debuff->hidden)
            return;

        ostringstream s;

        s << "Target gained " << debuff->name;
        if (debuff->max_stacks > 1)
            s << " (" << stacks << ")";

        addLog(LOG_BUFF, s.str());
    }

    void logDebuffExpire(shared_ptr<debuff::Debuff> debuff)
    {
        if (!logging || debuff->hidden)
            return;

        ostringstream s;

        s << "Target lost " << debuff->name;

        addLog(LOG_BUFF, s.str());
    }

    void logRageGain(double rage, string source)
    {
        if (!logging)
            return;

        ostringstream s;

        s << fixed << setprecision(1);
        s << "Gained " << rage << " rage from " << source;

        addLog(LOG_RAGE, s.str());
    }

    string jsonLog()
    {
        ostringstream s;

        s << "[";

        for (int i=0; i<log.size(); i++) {
            if (i > 0)
                s << ",";
            s << "{";
            s << "\"text\":\"" << log[i]->text << "\"";
            s << ",\"t\":" << log[i]->t;
            s << ",\"type\":" << log[i]->type;
            s << ",\"dmg\":" << log[i]->dmg;
            s << ",\"rage\":" << log[i]->rage;
            s << ",\"rage_percent\":" << log[i]->rage_percent;
            s << "}";
        }

        s << "]";

        return s.str();
    }

    void addLog(LogType type, string text)
    {
        if (!logging)
            return;

        shared_ptr<LogEntry> entry(new LogEntry);
        entry->type = type;
        entry->text = text;
        entry->t = state->t;
        entry->dmg = state->dmg;
        entry->rage = state->rage;
        entry->rage_percent = state->rage / RAGE_CAP * 100.0;

        log.push_back(entry);
    }

    void printLog()
    {
        for (auto itr = log.begin(); itr != log.end(); itr++) {
            printf("%.2f %s\n", (*itr)->t, (*itr)->text.c_str());
        }
    }

    void clearLog()
    {
        log.clear();
    }

    string spellStats()
    {
        ostringstream s;

        s << "[";

        for (auto itr = state->spells.begin(); itr != state->spells.end(); itr++) {
            if (itr != state->spells.begin())
                s << ",";
            s << "{";
            s << "\"id\":" << itr->first << ",";
            s << "\"name\":\"" << itr->second.name << "\",";
            s << "\"casts\":" << itr->second.casts << ",";
            s << "\"misses\":" << itr->second.misses << ",";
            s << "\"dodges\":" << itr->second.dodges << ",";
            s << "\"parries\":" << itr->second.parries << ",";
            s << "\"hits\":" << itr->second.hits << ",";
            s << "\"crits\":" << itr->second.crits << ",";
            s << "\"glances\":" << itr->second.glances << ",";
            s << "\"min_dmg\":" << itr->second.min_dmg << ",";
            s << "\"max_dmg\":" << itr->second.max_dmg << ",";
            s << "\"dmg\":" << itr->second.dmg;
            s << "}";
        }

        s << "]";

        return s.str();
    }

};
