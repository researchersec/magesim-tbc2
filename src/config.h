struct Config
{

    int duration = 200;
    double duration_variance = 1.5;
    int rng_seed = 0;
    bool avg_spell_dmg = false;
    bool additional_data = false;
    int targets = 1;
    
    // Weapon configuration
    double mh_weapon_speed = 2.6;
    double mh_low_dmg = 200;
    double mh_high_dmg = 372;
    double oh_weapon_speed = 1.5;
    double oh_low_dmg = 85;
    double oh_high_dmg = 159;
    bool dual_wield = true;

    // Debuffs
    bool sunder_armor = true;
    bool expose_armor = false;
    bool curse_of_recklessness = true;
    bool faerie_fire = false;
    bool blood_frenzy = false;
    bool mangle = false;
    bool gift_of_arthas = false;
    bool improved_hunters_mark = false;

    // Raid buffs
    bool battle_shout = true;
    bool commanding_shout = false;
    bool blessing_of_kings = true;
    bool blessing_of_might = true;
    bool mark_of_the_wild = true;
    bool strength_of_earth = true;
    bool grace_of_air = true;
    bool windfury_totem = true;
    bool unleashed_rage = true;
    bool leader_of_the_pack = true;
    bool trueshot_aura = true;
    bool battle_squawk = false;
    bool improved_battle_shout = false;
    bool improved_blessing_of_might = false;
    bool improved_strength_of_earth = false;
    bool improved_grace_of_air = false;
    bool improved_mark_of_the_wild = false;

    // Consumables
    Food food;
    Flask flask;
    Elixir battle_elixir;
    Elixir guardian_elixir;
    WeaponOil weapon_oil;
    Drums drums;
    Potion potion;
    Potion first_potion;

    // Trinkets and gems
    Trinket trinket1 = TRINKET_NONE;
    Trinket trinket2 = TRINKET_NONE;
    MetaGem meta_gem = META_NONE;

    // Cooldowns
    bool bloodlust = true;
    bool death_wish = true;
    bool recklessness = false;
    bool bloodrage = true;
    bool sweeping_strikes = false;
    bool drums_friend = false;

    MainRotation main_rotation = MAIN_ROTATION_BT;
    bool use_heroic_strike = true;
    double heroic_strike_rage_threshold = 60;
    bool use_whirlwind = true;
    bool use_execute = true;
    double execute_phase_start = 20.0; // Boss health %

    std::vector<double> trinket1_t;
    std::vector<double> trinket2_t;
    std::vector<double> death_wish_t;
    std::vector<double> recklessness_t;
    std::vector<double> bloodrage_t;
    std::vector<double> bloodlust_t;
    std::vector<double> drums_t;
    std::vector<double> potion_t;

};
