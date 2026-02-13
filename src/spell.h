namespace spell
{

    enum ID : int
    {
        MELEE_MAIN_HAND = 1,
        MELEE_OFF_HAND = 2,
        BLOODTHIRST = 30335,
        MORTAL_STRIKE = 30330,
        WHIRLWIND = 30357,
        EXECUTE = 25236,
        HEROIC_STRIKE = 30324,
        SLAM = 25242,
        RAMPAGE = 30033,
        BLOODRAGE = 2687,
        DEATH_WISH = 12328,
        RECKLESSNESS = 1719,
        SWEEPING_STRIKES = 12328,
    };

    enum Result : int
    {
        HIT,
        CRIT,
        MISS,
        DODGE,
        PARRY,
        GLANCE
    };

    class Spell
    {

    public:
        ID id;
        std::string name;
        double cost = 0;
        double min_dmg = 0;
        double max_dmg = 0;
        double cast_time = 0;
        double cooldown = 0;
        bool on_gcd = true;
        bool proc = false;
        bool normalized = true;
        double weapon_multiplier = 1.0;
        double bonus_dmg = 0;
        DamageType damage_type = DAMAGE_PHYSICAL;
        bool can_crit = true;
        bool can_glance = false;
        bool is_melee = false;

        Result result;
        double dmg = 0;
        double actual_cost = 0;
        bool done = false;

        int misses = 0;
        int dodges = 0;
        int parries = 0;
        int hits = 0;
        int crits = 0;
        int glances = 0;

        double avgDmg()
        {
            return (min_dmg + max_dmg)/2.0;
        }

    };

    class MeleeMainHand : public Spell
    {

    public:
        MeleeMainHand()
        {
            id = MELEE_MAIN_HAND;
            name = "Melee (MH)";
            cost = 0;
            cast_time = 0;
            on_gcd = false;
            normalized = false;
            is_melee = true;
            can_glance = true;
        }

    };

    class MeleeOffHand : public Spell
    {

    public:
        MeleeOffHand()
        {
            id = MELEE_OFF_HAND;
            name = "Melee (OH)";
            cost = 0;
            cast_time = 0;
            on_gcd = false;
            normalized = false;
            weapon_multiplier = 0.5;
            is_melee = true;
            can_glance = true;
        }

    };

    class Bloodthirst : public Spell
    {

    public:
        Bloodthirst()
        {
            id = BLOODTHIRST;
            name = "Bloodthirst";
            cost = 30;
            cast_time = 0;
            cooldown = 6.0;
            normalized = true;
            is_melee = true;
        }

    };

    class MortalStrike : public Spell
    {

    public:
        MortalStrike()
        {
            id = MORTAL_STRIKE;
            name = "Mortal Strike";
            cost = 30;
            bonus_dmg = 210;
            cast_time = 0;
            cooldown = 6.0;
            normalized = true;
            is_melee = true;
        }

    };

    class Whirlwind : public Spell
    {

    public:
        Whirlwind()
        {
            id = WHIRLWIND;
            name = "Whirlwind";
            cost = 25;
            cast_time = 0;
            cooldown = 10.0;
            normalized = true;
            is_melee = true;
        }

    };

    class Execute : public Spell
    {

    public:
        Execute()
        {
            id = EXECUTE;
            name = "Execute";
            cost = 15; // Base cost, consumes all rage
            bonus_dmg = 925;
            cast_time = 0;
            normalized = false;
            is_melee = true;
        }

    };

    class HeroicStrike : public Spell
    {

    public:
        HeroicStrike()
        {
            id = HEROIC_STRIKE;
            name = "Heroic Strike";
            cost = 15;
            bonus_dmg = 176;
            cast_time = 0;
            on_gcd = false;
            normalized = false;
            is_melee = true;
        }

    };

    class Slam : public Spell
    {

    public:
        Slam()
        {
            id = SLAM;
            name = "Slam";
            cost = 15;
            bonus_dmg = 87;
            cast_time = 1.5;
            normalized = false;
            is_melee = true;
        }

    };

}
