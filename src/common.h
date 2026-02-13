const double GCD_MIN = 1.0;
const double RAGE_CAP = 100.0;

enum MainRotation: int
{
    MAIN_ROTATION_BT, // Bloodthirst (Fury)
    MAIN_ROTATION_MS, // Mortal Strike (Arms)
    MAIN_ROTATION_SLAM, // Slam spec
};

enum Race : int
{
    RACE_HUMAN,
    RACE_DWARF,
    RACE_NIGHT_ELF,
    RACE_GNOME,
    RACE_DRAENEI,
    RACE_ORC,
    RACE_UNDEAD,
    RACE_TAUREN,
    RACE_TROLL
};

enum Faction : int
{
    FACTION_HORDE,
    FACTION_ALLIANCE,
};

enum DamageType : int
{
    DAMAGE_PHYSICAL,
    DAMAGE_BLEED,
};

enum MetaGem : int {
    META_NONE = 0,
    META_RELENTLESS_EARTHSTORM = 32409,
    META_CHAOTIC_SKYFIRE = 34220,
};

enum Trinket : int
{
    TRINKET_NONE = 0,
    TRINKET_BLOODLUST_BROOCH = 28830,
    TRINKET_ABACUS = 28288,
    TRINKET_TSUNAMI_TALISMAN = 30627,
    TRINKET_HOURGLASS = 28034,
    TRINKET_BERSERKERS_CALL = 23558,
    TRINKET_SLAYERS_CREST = 28121,
    TRINKET_BADGE_TENACITY = 28528,
    TRINKET_ICON_UNYIELDING_COURAGE = 28041,
    TRINKET_DRAGONSPINE_TROPHY = 28830,
    TRINKET_SHARD_CONTEMPT = 34472,
    TRINKET_MADNESS = 32505,
    TRINKET_BLACKENED_NAARU_SLIVER = 34427,
    TRINKET_ROMULOS_POISON_VIAL = 28579,
    TRINKET_BLOODMOON = 32492,
    TRINKET_CRUSADE = 31856,
};

enum Food : int
{
    FOOD_NONE = 0,
    FOOD_GRILLED_MUDFISH = 27664,
    FOOD_RAVAGER_DOG = 27655,
    FOOD_ROASTED_CLEFTHOOF = 27658,
};

enum Flask : int
{
    FLASK_NONE = 0,
    FLASK_RELENTLESS_ASSAULT = 28520,
    FLASK_FORTIFICATION = 28518,
    FLASK_CHROMATIC_WONDER = 33208,
};

enum Elixir : int
{
    ELIXIR_NONE = 0,
    ELIXIR_MAJOR_AGILITY = 22831,
    ELIXIR_MAJOR_STRENGTH = 28490,
    ELIXIR_MASTERY = 28104,
    ELIXIR_MAJOR_FORTITUDE = 32062,
};

enum Drums : int
{
    DRUMS_NONE = 0,
    DRUMS_OF_WAR = 29528,
    DRUMS_OF_BATTLE = 29529,
};

enum WeaponOil : int
{
    OIL_NONE = 0,
    OIL_ADAMANTITE_WEIGHTSTONE = 28421,
};

enum Potion : int
{
    POTION_NONE = 0,
    POTION_HASTE = 28507,
    POTION_INSANE_STRENGTH = 22828,
};

enum EventType : int
{
    EVENT_CAST,
    EVENT_SPELL,
    EVENT_MELEE_MAIN_HAND,
    EVENT_MELEE_OFF_HAND,
    EVENT_RAGE_GAIN,
    EVENT_BUFF_GAIN,
    EVENT_BUFF_EXPIRE,
    EVENT_DEBUFF_GAIN,
    EVENT_DEBUFF_EXPIRE,
    EVENT_DOT,
    EVENT_CD_GAIN,
    EVENT_CD_EXPIRE,
    EVENT_BLOODRAGE,
    EVENT_BLOODRAGE_TICK,
    EVENT_DRUMS,
    EVENT_WAIT
};

enum LogType : int
{
    LOG_NONE,
    LOG_SPELL,
    LOG_MELEE,
    LOG_RAGE,
    LOG_BUFF,
    LOG_DOT,
    LOG_DEBUG,
    LOG_WAIT,
};

struct LogEntry
{
    LogType type;
    std::string text;
    double t;
    double rage;
    double rage_percent;
    int dmg;
};

struct SpellStats
{
    std::string name;
    int casts = 0;
    int misses = 0;
    int dodges = 0;
    int parries = 0;
    int hits = 0;
    int crits = 0;
    int glances = 0;
    double dmg = 0;
    double min_dmg = 0;
    double max_dmg = 0;
};

struct SimulationResult
{
    int dmg;
    double t;
    double dps;
    std::string log;
    std::string spells;
};

struct SimulationsResult
{
    double min_dps, avg_dps, max_dps;
    int iterations;
    std::string histogram;
    std::string all_results;
    std::string stats;
};

double critRatingToChance(double rating)
{
    return rating / 22.08;
}

double hitRatingToChance(double rating)
{
    return rating / 15.77;
}

double hasteRatingToHaste(double rating)
{
    return rating / 15.77;
}

double expertiseRatingToExpertise(double rating)
{
    return rating / 15.77;
}

// Global rng generator
thread_local static std::mt19937 g_rng(std::random_device{}());

template<typename Numeric>
Numeric random(Numeric from, Numeric to)
{
    using dist_type = typename std::conditional
    <
        std::is_integral<Numeric>::value
        , std::uniform_int_distribution<Numeric>
        , std::uniform_real_distribution<Numeric>
    >::type;

    thread_local static dist_type dist;

    return dist(g_rng, typename dist_type::param_type{from, to});
}

void setRNGSeed(int seed)
{
    g_rng.seed(seed);
}
