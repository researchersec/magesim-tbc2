namespace buff
{

    enum ID : int
    {
        BLOODLUST = 2825,
        DEATH_WISH = 12328,
        RECKLESSNESS = 1719,
        BLOODRAGE = 2687,
        RAMPAGE = 30033,
        FLURRY = 12319,
        ENRAGE = 13048,
        SWEEPING_STRIKES = 12328,
        BATTLE_SHOUT = 2048,
        COMMANDING_SHOUT = 469,
        
        // Trinkets
        BLOODLUST_BROOCH = 28830,
        ABACUS = 28288,
        TSUNAMI_TALISMAN = 30627,
        HOURGLASS = 28034,
        SLAYERS_CREST = 28121,
        BADGE_TENACITY = 28528,
        ICON_UNYIELDING_COURAGE = 28041,
        DRAGONSPINE_TROPHY = 28830,
        SHARD_CONTEMPT = 34472,
        MADNESS = 32505,
        BLACKENED_NAARU_SLIVER = 34427,
        ROMULOS_POISON_VIAL = 28579,
        BLOODMOON = 32492,
        CRUSADE = 31856,
        
        // Consumables
        DRUMS_OF_BATTLE = 29529,
        DRUMS_OF_WAR = 29528,
        HASTE_POTION = 28507,
        INSANE_STRENGTH_POTION = 22828,
        
        // Raid buffs
        WINDFURY_TOTEM = 8512,
        GRACE_OF_AIR = 25359,
        STRENGTH_OF_EARTH = 25528,
        BLESSING_OF_KINGS = 20217,
        BLESSING_OF_MIGHT = 27140,
        MARK_OF_THE_WILD = 26990,
        LEADER_OF_THE_PACK = 24932,
        UNLEASHED_RAGE = 30811,
        TRUESHOT_AURA = 27066,
        BATTLE_SQUAWK = 23060,
    };


    class Buff
    {

    public:
        ID id;
        std::string name;
        double duration;
        int stacks = 1;
        int max_stacks = 1;
        bool hidden = false;

        int addStack()
        {
            if (stacks < max_stacks)
                stacks++;
            else
                return 0;

            return stacks;
        }

    };

    class Bloodlust : public Buff
    {

    public:
        Bloodlust()
        {
            id = BLOODLUST;
            name = "Bloodlust";
            duration = 40;
        }

    };

    class DeathWish : public Buff
    {

    public:
        DeathWish()
        {
            id = DEATH_WISH;
            name = "Death Wish";
            duration = 30;
        }

    };

    class Recklessness : public Buff
    {

    public:
        Recklessness()
        {
            id = RECKLESSNESS;
            name = "Recklessness";
            duration = 15;
        }

    };

    class Bloodrage : public Buff
    {

    public:
        Bloodrage()
        {
            id = BLOODRAGE;
            name = "Bloodrage";
            duration = 10;
        }

    };

    class Rampage : public Buff
    {

    public:
        Rampage()
        {
            id = RAMPAGE;
            name = "Rampage";
            duration = 30;
        }

    };

    class Flurry : public Buff
    {

    public:
        Flurry()
        {
            id = FLURRY;
            name = "Flurry";
            duration = 15;
            max_stacks = 3;
        }

    };

    class Enrage : public Buff
    {

    public:
        Enrage()
        {
            id = ENRAGE;
            name = "Enrage";
            duration = 12;
        }

    };

    class SweepingStrikes : public Buff
    {

    public:
        SweepingStrikes()
        {
            id = SWEEPING_STRIKES;
            name = "Sweeping Strikes";
            duration = 10;
        }

    };

    class BattleShout : public Buff
    {

    public:
        BattleShout()
        {
            id = BATTLE_SHOUT;
            name = "Battle Shout";
            duration = 120;
        }

    };

    class BloodlustBrooch : public Buff
    {

    public:
        BloodlustBrooch()
        {
            id = BLOODLUST_BROOCH;
            name = "Bloodlust";
            duration = 20;
        }

    };

    class Abacus : public Buff
    {

    public:
        Abacus()
        {
            id = ABACUS;
            name = "Abacus";
            duration = 10;
            max_stacks = 10;
        }

    };

    class TsunamiTalisman : public Buff
    {

    public:
        TsunamiTalisman()
        {
            id = TSUNAMI_TALISMAN;
            name = "Tsunami Talisman";
            duration = 10;
        }

    };

    class Hourglass : public Buff
    {

    public:
        Hourglass()
        {
            id = HOURGLASS;
            name = "Hourglass of the Unraveller";
            duration = 10;
        }

    };

    class SlayersCrest : public Buff
    {

    public:
        SlayersCrest()
        {
            id = SLAYERS_CREST;
            name = "Slayer's Crest";
            duration = 20;
        }

    };

    class BadgeTenacity : public Buff
    {

    public:
        BadgeTenacity()
        {
            id = BADGE_TENACITY;
            name = "Badge of Tenacity";
            duration = 10;
        }

    };

    class IconUnyieldingCourage : public Buff
    {

    public:
        IconUnyieldingCourage()
        {
            id = ICON_UNYIELDING_COURAGE;
            name = "Icon of Unyielding Courage";
            duration = 20;
        }

    };

    class DragonspineTrophy : public Buff
    {

    public:
        DragonspineTrophy()
        {
            id = DRAGONSPINE_TROPHY;
            name = "Dragonspine Trophy";
            duration = 10;
        }

    };

    class ShardContempt : public Buff
    {

    public:
        ShardContempt()
        {
            id = SHARD_CONTEMPT;
            name = "Shard of Contempt";
            duration = 20;
        }

    };

    class Madness : public Buff
    {

    public:
        Madness()
        {
            id = MADNESS;
            name = "Madness of the Betrayer";
            duration = 10;
        }

    };

    class BlackenedNaaruSliver : public Buff
    {

    public:
        BlackenedNaaruSliver()
        {
            id = BLACKENED_NAARU_SLIVER;
            name = "Blackened Naaru Sliver";
            duration = 20;
        }

    };

    class RomulosPoisonVial : public Buff
    {

    public:
        RomulosPoisonVial()
        {
            id = ROMULOS_POISON_VIAL;
            name = "Romulo's Poison Vial";
            duration = 20;
        }

    };

    class Bloodmoon : public Buff
    {

    public:
        Bloodmoon()
        {
            id = BLOODMOON;
            name = "Bloodmoon";
            duration = 10;
        }

    };

    class Crusade : public Buff
    {

    public:
        Crusade()
        {
            id = CRUSADE;
            name = "Aura of the Crusade";
            duration = 10;
            max_stacks = 10;
        }

    };

    class DrumsOfBattle : public Buff
    {

    public:
        DrumsOfBattle()
        {
            id = DRUMS_OF_BATTLE;
            name = "Drums of Battle";
            duration = 30;
        }

    };

    class DrumsOfWar : public Buff
    {

    public:
        DrumsOfWar()
        {
            id = DRUMS_OF_WAR;
            name = "Drums of War";
            duration = 30;
        }

    };

    class HastePotion : public Buff
    {

    public:
        HastePotion()
        {
            id = HASTE_POTION;
            name = "Haste Potion";
            duration = 15;
        }

    };

    class InsaneStrengthPotion : public Buff
    {

    public:
        InsaneStrengthPotion()
        {
            id = INSANE_STRENGTH_POTION;
            name = "Insane Strength Potion";
            duration = 15;
        }

    };

}
