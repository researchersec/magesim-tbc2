using namespace std;

class Player
{

public:
    Stats stats;
    Talents talents;
    shared_ptr<Config> config;
    Race race = RACE_ORC;

    Player(shared_ptr<Config> _config)
    {
        config = _config;

        setBaseStats();
    }

    // Stats without gear or talents
    void setBaseStats()
    {
        // Orc default
        stats.strength = 123;
        stats.agility = 77;
        stats.stamina = 122;
        stats.crit = 0;
        stats.hit = 0;
        stats.haste = 0;
        stats.expertise = 0;
        stats.armor_pen = 0;
        stats.attack_power = 0;

        if (race == RACE_HUMAN) {
            stats.strength = 120;
            stats.agility = 80;
            stats.stamina = 120;
        }
        if (race == RACE_DWARF) {
            stats.strength = 122;
            stats.agility = 76;
            stats.stamina = 123;
        }
        if (race == RACE_NIGHT_ELF) {
            stats.strength = 117;
            stats.agility = 85;
            stats.stamina = 119;
        }
        if (race == RACE_GNOME) {
            stats.strength = 115;
            stats.agility = 83;
            stats.stamina = 119;
        }
        if (race == RACE_DRAENEI) {
            stats.strength = 121;
            stats.agility = 77;
            stats.stamina = 122;
        }
        if (race == RACE_UNDEAD) {
            stats.strength = 119;
            stats.agility = 78;
            stats.stamina = 121;
        }
        if (race == RACE_TAUREN) {
            stats.strength = 125;
            stats.agility = 75;
            stats.stamina = 122;
        }
        if (race == RACE_TROLL) {
            stats.strength = 121;
            stats.agility = 82;
            stats.stamina = 121;
        }
    }

    // Stats with gear but without talents
    void setDefaultStats()
    {
        stats.strength = 500;
        stats.agility = 400;
        stats.stamina = 500;
        stats.crit = 25;
        stats.hit = 9;
        stats.haste = 0;
        stats.expertise = 0;
        stats.armor_pen = 0;
        stats.attack_power = 2000;
    }

    void quickReady()
    {
        setDefaultTalents();
        setDefaultStats();
        setConfigStats();
    }

    void ready()
    {
        setConfigStats();
    }

    void setStats(Stats _stats)
    {
        stats = _stats;
    }

    void setConfigStats()
    {
        // Attribute additions
        if (config->battle_shout) {
            double ap = 306;
            if (config->improved_battle_shout)
                ap*= 1.25;
            stats.attack_power+= ap;
        }
        if (config->blessing_of_might) {
            double ap = 220;
            if (config->improved_blessing_of_might)
                ap*= 1.25;
            stats.attack_power+= ap;
        }
        if (config->strength_of_earth) {
            double str = 86;
            if (config->improved_strength_of_earth)
                str+= 12;
            stats.strength+= str;
        }
        if (config->grace_of_air) {
            double agi = 88;
            if (config->improved_grace_of_air)
                agi+= 12;
            stats.agility+= agi;
        }
        if (config->mark_of_the_wild) {
            double bonus = 14;
            if (config->improved_mark_of_the_wild)
                bonus+= 2;
            stats.strength+= bonus;
            stats.agility+= bonus;
            stats.stamina+= bonus;
        }
        if (config->flask == FLASK_RELENTLESS_ASSAULT)
            stats.attack_power+= 120;
        if (config->flask == FLASK_FORTIFICATION)
            stats.stamina+= 500;
        if (config->flask == FLASK_CHROMATIC_WONDER) {
            stats.strength+= 18;
            stats.agility+= 18;
            stats.stamina+= 18;
        }
        if (config->battle_elixir == ELIXIR_MAJOR_AGILITY)
            stats.agility+= 35;
        if (config->battle_elixir == ELIXIR_MAJOR_STRENGTH)
            stats.strength+= 35;
        if (config->guardian_elixir == ELIXIR_MAJOR_FORTITUDE)
            stats.stamina+= 250;
        if (config->guardian_elixir == ELIXIR_MASTERY) {
            stats.strength+= 15;
            stats.agility+= 15;
            stats.stamina+= 15;
        }
        if (config->food == FOOD_GRILLED_MUDFISH)
            stats.agility+= 20;
        if (config->food == FOOD_RAVAGER_DOG)
            stats.strength+= 20;
        if (config->food == FOOD_ROASTED_CLEFTHOOF)
            stats.strength+= 20;
        if (config->weapon_oil == OIL_ADAMANTITE_WEIGHTSTONE)
            stats.attack_power+= 28;

        // Attribute multipliers
        if (config->blessing_of_kings) {
            stats.strength*= 1.1;
            stats.agility*= 1.1;
            stats.stamina*= 1.1;
        }
        stats.strength = round(stats.strength);
        stats.agility = round(stats.agility);
        stats.stamina = round(stats.stamina);

        // Attack power from strength
        stats.attack_power+= stats.strength * 2;

        // Crit from agility
        stats.crit+= stats.agility / 25.0;

        // Racial bonuses
        if (race == RACE_HUMAN) {
            // Humans: +5 expertise with swords and maces
            stats.expertise+= 5;
        }
        if (race == RACE_ORC) {
            // Orcs: +5 expertise with axes
            stats.expertise+= 5;
        }
        if (race == RACE_DWARF) {
            // Dwarves: +5 expertise with maces
            stats.expertise+= 5;
        }

        // Talent bonuses
        if (talents.cruelty)
            stats.crit+= talents.cruelty;
        if (talents.precision)
            stats.hit+= talents.precision;
        if (talents.axe_spec)
            stats.crit+= talents.axe_spec;
        if (talents.polearm_spec)
            stats.crit+= talents.polearm_spec;

        // Raid buff multipliers
        if (config->trueshot_aura)
            stats.attack_power = round(stats.attack_power * 1.1);
        if (config->unleashed_rage)
            stats.attack_power = round(stats.attack_power * 1.1);
        if (config->battle_squawk)
            stats.attack_power = round(stats.attack_power * 1.05);
    }

    void setDefaultTalents()
    {
        // Fury build (17/44/0)
        talents.cruelty = 5;
        talents.unbridled_wrath = 5;
        talents.commanding_presence = 5;
        talents.dual_wield_spec = 5;
        talents.imp_execute = 2;
        talents.enrage = 5;
        talents.precision = 3;
        talents.death_wish = 1;
        talents.imp_berserker_rage = 2;
        talents.flurry = 5;
        talents.intensify_rage = 3;
        talents.bloodthirst = 1;
        talents.imp_whirlwind = 2;
        talents.imp_berserker_stance = 5;
        talents.rampage = 1;
        
        // Arms
        talents.imp_heroic_strike = 3;
        talents.deflection = 5;
        talents.imp_rend = 3;
        talents.tactical_mastery = 1;
        talents.deep_wounds = 3;
        talents.two_handed_spec = 2;
    }

    void loadTalentsFromString(std::string str)
    {
        int tree = 0, t = 0, p = 0;

        for (int i=0; i<str.length(); i++) {
            if (str[i] == '-') {
                tree++;
                t = 0;
            }
            else {
                p = str[i] - '0';

                // Arms tree
                if (tree == 0 && t == 0) talents.imp_heroic_strike = p;
                else if (tree == 0 && t == 1) talents.deflection = p;
                else if (tree == 0 && t == 2) talents.imp_rend = p;
                else if (tree == 0 && t == 3) talents.imp_charge = p;
                else if (tree == 0 && t == 4) talents.tactical_mastery = p;
                else if (tree == 0 && t == 5) talents.imp_overpower = p;
                else if (tree == 0 && t == 6) talents.anger_management = p;
                else if (tree == 0 && t == 7) talents.deep_wounds = p;
                else if (tree == 0 && t == 8) talents.two_handed_spec = p;
                else if (tree == 0 && t == 9) talents.impale = p;
                else if (tree == 0 && t == 10) talents.axe_spec = p;
                else if (tree == 0 && t == 11) talents.sweeping_strikes = p;
                else if (tree == 0 && t == 12) talents.mace_spec = p;
                else if (tree == 0 && t == 13) talents.sword_spec = p;
                else if (tree == 0 && t == 14) talents.polearm_spec = p;
                else if (tree == 0 && t == 15) talents.imp_hamstring = p;
                else if (tree == 0 && t == 16) talents.mortal_strike = p;
                else if (tree == 0 && t == 17) talents.strength_of_arms = p;
                else if (tree == 0 && t == 18) talents.imp_mortal_strike = p;
                else if (tree == 0 && t == 19) talents.unrelenting_assault = p;
                else if (tree == 0 && t == 20) talents.endless_rage = p;
                
                // Fury tree
                else if (tree == 1 && t == 0) talents.booming_voice = p;
                else if (tree == 1 && t == 1) talents.cruelty = p;
                else if (tree == 1 && t == 2) talents.imp_demoralizing_shout = p;
                else if (tree == 1 && t == 3) talents.unbridled_wrath = p;
                else if (tree == 1 && t == 4) talents.imp_cleave = p;
                else if (tree == 1 && t == 5) talents.piercing_howl = p;
                else if (tree == 1 && t == 6) talents.blood_craze = p;
                else if (tree == 1 && t == 7) talents.commanding_presence = p;
                else if (tree == 1 && t == 8) talents.dual_wield_spec = p;
                else if (tree == 1 && t == 9) talents.imp_execute = p;
                else if (tree == 1 && t == 10) talents.enrage = p;
                else if (tree == 1 && t == 11) talents.precision = p;
                else if (tree == 1 && t == 12) talents.death_wish = p;
                else if (tree == 1 && t == 13) talents.imp_intercept = p;
                else if (tree == 1 && t == 14) talents.imp_berserker_rage = p;
                else if (tree == 1 && t == 15) talents.flurry = p;
                else if (tree == 1 && t == 16) talents.intensify_rage = p;
                else if (tree == 1 && t == 17) talents.bloodthirst = p;
                else if (tree == 1 && t == 18) talents.imp_whirlwind = p;
                else if (tree == 1 && t == 19) talents.imp_berserker_stance = p;
                else if (tree == 1 && t == 20) talents.rampage = p;

                t++;
            }
        }
    }

    Faction faction()
    {
        if (race == RACE_HUMAN || race == RACE_DWARF || race == RACE_NIGHT_ELF || race == RACE_GNOME || race == RACE_DRAENEI)
            return FACTION_ALLIANCE;
        return FACTION_HORDE;
    }

};
