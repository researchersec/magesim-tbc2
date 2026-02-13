using namespace std;

class State
{

public:
    double t;
    double t_gcd;
    double rage;
    int dmg;
    double duration;
    double mh_timer;
    double oh_timer;
    bool used_pot;
    bool execute_phase;
    int flurry_charges;

    map<cooldown::ID, shared_ptr<cooldown::Cooldown>> cooldowns;
    map<buff::ID, shared_ptr<buff::Buff>> buffs;
    map<debuff::ID, shared_ptr<debuff::Debuff>> debuffs;
    map<spell::ID, SpellStats> spells;

    shared_ptr<Config> config;

    State(shared_ptr<Config> _config)
    {
        config = _config;
        reset();
    }

    void reset()
    {
        t = 0;
        t_gcd = 0;
        rage = 0;
        dmg = 0;
        duration = config->duration;
        duration+= -config->duration_variance + random<double>(0, config->duration_variance*2);
        mh_timer = 0;
        oh_timer = 0;
        used_pot = false;
        execute_phase = false;
        flurry_charges = 0;

        buffs.clear();
        debuffs.clear();
        cooldowns.clear();
        spells.clear();
    }

    double dps()
    {
        if (t == 0)
            return 0;

        return dmg / t;
    }

    bool hasCooldown(cooldown::ID id)
    {
        return cooldowns.find(id) != cooldowns.end();
    }

    void addCooldown(shared_ptr<cooldown::Cooldown> cooldown)
    {
        cooldowns[cooldown->id] = cooldown;
    }

    void removeCooldown(cooldown::ID id)
    {
        cooldowns.erase(id);
    }

    int buffStacks(buff::ID id)
    {
        if (hasBuff(id))
            return buffs[id]->stacks;
        return 0;
    }

    bool hasBuff(buff::ID id)
    {
        return buffs.find(id) != buffs.end();
    }

    int addBuff(shared_ptr<buff::Buff> buff)
    {
        if (hasBuff(buff->id))
            return buffs[buff->id]->addStack();
        else
            buffs[buff->id] = buff;

        return 1;
    }

    void removeBuff(buff::ID id)
    {
        buffs.erase(id);
    }

    int debuffStacks(debuff::ID id)
    {
        if (hasDebuff(id))
            return debuffs[id]->stacks;
        return 0;
    }

    bool hasDebuff(debuff::ID id)
    {
        return debuffs.find(id) != debuffs.end();
    }

    int addDebuff(shared_ptr<debuff::Debuff> debuff)
    {
        if (hasDebuff(debuff->id))
            return debuffs[debuff->id]->addStack();
        else
            debuffs[debuff->id] = debuff;

        return 1;
    }

    void removeDebuff(debuff::ID id)
    {
        debuffs.erase(id);
    }

};
