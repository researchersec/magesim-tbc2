namespace cooldown
{

    enum ID : int
    {
        NONE = 0,
        POTION = 1,
        TRINKET1 = 3,
        TRINKET2 = 4,
        TRINKET_SHARED = 5,
        BLOODTHIRST = 30335,
        MORTAL_STRIKE = 30330,
        WHIRLWIND = 30357,
        DEATH_WISH = 12328,
        RECKLESSNESS = 1719,
        BLOODRAGE = 2687,
        SWEEPING_STRIKES = 12328,
        DRUMS = 29529,
        BERSERKERS_CALL = 23558,
        SLAYERS_CREST = 28121,
        BADGE_TENACITY = 28528,
        DRAGONSPINE_TROPHY = 28830,
        SHARD_CONTEMPT = 34472,
        MADNESS = 32505,
        BLACKENED_NAARU_SLIVER = 34427,
        ROMULOS_POISON_VIAL = 28579,
        BLOODMOON = 32492,
    };


    class Cooldown
    {

    public:
        ID id;
        double duration;

        Cooldown(ID _id = NONE, double _duration = 0)
        {
            if (_id)
                id = _id;
            if (_duration > 0)
                duration = _duration;
        }

    };

    class Potion : public Cooldown
    {

    public:
        Potion()
        {
            id = POTION;
            duration = 120;
        }

    };

    class TrinketShared : public Cooldown
    {

    public:
        TrinketShared(double _duration = 20)
        {
            id = TRINKET_SHARED;
            duration = _duration;
        }

    };

    class Bloodthirst : public Cooldown
    {

    public:
        Bloodthirst()
        {
            id = BLOODTHIRST;
            duration = 6.0;
        }

    };

    class MortalStrike : public Cooldown
    {

    public:
        MortalStrike()
        {
            id = MORTAL_STRIKE;
            duration = 6.0;
        }

    };

    class Whirlwind : public Cooldown
    {

    public:
        Whirlwind()
        {
            id = WHIRLWIND;
            duration = 10.0;
        }

    };

    class DeathWish : public Cooldown
    {

    public:
        DeathWish()
        {
            id = DEATH_WISH;
            duration = 180;
        }

    };

    class Recklessness : public Cooldown
    {

    public:
        Recklessness()
        {
            id = RECKLESSNESS;
            duration = 1800;
        }

    };

    class Bloodrage : public Cooldown
    {

    public:
        Bloodrage()
        {
            id = BLOODRAGE;
            duration = 60;
        }

    };

    class SweepingStrikes : public Cooldown
    {

    public:
        SweepingStrikes()
        {
            id = SWEEPING_STRIKES;
            duration = 30;
        }

    };

    class Drums : public Cooldown
    {

    public:
        Drums()
        {
            id = DRUMS;
            duration = 120;
        }

    };

    class BerserkersCall : public Cooldown
    {

    public:
        BerserkersCall()
        {
            id = BERSERKERS_CALL;
            duration = 20;
        }

    };

    class SlayersCrest : public Cooldown
    {

    public:
        SlayersCrest()
        {
            id = SLAYERS_CREST;
            duration = 120;
        }

    };

    class BadgeTenacity : public Cooldown
    {

    public:
        BadgeTenacity()
        {
            id = BADGE_TENACITY;
            duration = 90;
        }

    };

    class DragonspineTrophy : public Cooldown
    {

    public:
        DragonspineTrophy()
        {
            id = DRAGONSPINE_TROPHY;
            duration = 20;
        }

    };

    class ShardContempt : public Cooldown
    {

    public:
        ShardContempt()
        {
            id = SHARD_CONTEMPT;
            duration = 45;
        }

    };

    class Madness : public Cooldown
    {

    public:
        Madness()
        {
            id = MADNESS;
            duration = 50;
        }

    };

    class BlackenedNaaruSliver : public Cooldown
    {

    public:
        BlackenedNaaruSliver()
        {
            id = BLACKENED_NAARU_SLIVER;
            duration = 120;
        }

    };

    class RomulosPoisonVial : public Cooldown
    {

    public:
        RomulosPoisonVial()
        {
            id = ROMULOS_POISON_VIAL;
            duration = 120;
        }

    };

    class Bloodmoon : public Cooldown
    {

    public:
        Bloodmoon()
        {
            id = BLOODMOON;
            duration = 50;
        }

    };

}
