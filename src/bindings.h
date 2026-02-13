#ifdef __EMSCRIPTEN__

#include <emscripten.h>
#include <emscripten/bind.h>


std::shared_ptr<Config> allocConfig()
{
    return std::make_shared<Config>();
}

std::shared_ptr<Player> allocPlayer(std::shared_ptr<Config> config)
{
    return std::make_shared<Player>(config);
}

SimulationResult runSimulation(std::shared_ptr<Config> config, std::shared_ptr<Player> player)
{
    std::shared_ptr<Simulation> sim(new Simulation(config, player));
    player->ready();
    sim->logging = true;

    return sim->run();
}

SimulationsResult runSimulations(std::shared_ptr<Config> config, std::shared_ptr<Player> player, int iterations)
{
    std::shared_ptr<Simulation> sim(new Simulation(config, player));
    player->ready();

    return sim->runMultiple(iterations);
}

EMSCRIPTEN_BINDINGS(my_module) {

    emscripten::register_vector<double>("VectorDouble");

    emscripten::enum_<MainRotation>("MainRotation")
        .value("MAIN_ROTATION_BT", MAIN_ROTATION_BT)
        .value("MAIN_ROTATION_MS", MAIN_ROTATION_MS)
        .value("MAIN_ROTATION_SLAM", MAIN_ROTATION_SLAM)
        ;

    emscripten::enum_<Race>("Race")
        .value("RACE_HUMAN", RACE_HUMAN)
        .value("RACE_DWARF", RACE_DWARF)
        .value("RACE_NIGHT_ELF", RACE_NIGHT_ELF)
        .value("RACE_GNOME", RACE_GNOME)
        .value("RACE_DRAENEI", RACE_DRAENEI)
        .value("RACE_ORC", RACE_ORC)
        .value("RACE_UNDEAD", RACE_UNDEAD)
        .value("RACE_TAUREN", RACE_TAUREN)
        .value("RACE_TROLL", RACE_TROLL);

    emscripten::enum_<Trinket>("Trinket")
        .value("TRINKET_NONE", TRINKET_NONE)
        .value("TRINKET_BLOODLUST_BROOCH", TRINKET_BLOODLUST_BROOCH)
        .value("TRINKET_ABACUS", TRINKET_ABACUS)
        .value("TRINKET_TSUNAMI_TALISMAN", TRINKET_TSUNAMI_TALISMAN)
        .value("TRINKET_HOURGLASS", TRINKET_HOURGLASS)
        .value("TRINKET_BERSERKERS_CALL", TRINKET_BERSERKERS_CALL)
        .value("TRINKET_SLAYERS_CREST", TRINKET_SLAYERS_CREST)
        .value("TRINKET_BADGE_TENACITY", TRINKET_BADGE_TENACITY)
        .value("TRINKET_ICON_UNYIELDING_COURAGE", TRINKET_ICON_UNYIELDING_COURAGE)
        .value("TRINKET_DRAGONSPINE_TROPHY", TRINKET_DRAGONSPINE_TROPHY)
        .value("TRINKET_SHARD_CONTEMPT", TRINKET_SHARD_CONTEMPT)
        .value("TRINKET_MADNESS", TRINKET_MADNESS)
        .value("TRINKET_BLACKENED_NAARU_SLIVER", TRINKET_BLACKENED_NAARU_SLIVER)
        .value("TRINKET_ROMULOS_POISON_VIAL", TRINKET_ROMULOS_POISON_VIAL)
        .value("TRINKET_BLOODMOON", TRINKET_BLOODMOON)
        .value("TRINKET_CRUSADE", TRINKET_CRUSADE)
        ;

    emscripten::enum_<MetaGem>("MetaGem")
        .value("META_NONE", META_NONE)
        .value("META_RELENTLESS_EARTHSTORM", META_RELENTLESS_EARTHSTORM)
        .value("META_CHAOTIC_SKYFIRE", META_CHAOTIC_SKYFIRE)
        ;

    emscripten::enum_<Food>("Food")
        .value("FOOD_NONE", FOOD_NONE)
        .value("FOOD_GRILLED_MUDFISH", FOOD_GRILLED_MUDFISH)
        .value("FOOD_RAVAGER_DOG", FOOD_RAVAGER_DOG)
        .value("FOOD_ROASTED_CLEFTHOOF", FOOD_ROASTED_CLEFTHOOF);

    emscripten::enum_<Flask>("Flask")
        .value("FLASK_NONE", FLASK_NONE)
        .value("FLASK_RELENTLESS_ASSAULT", FLASK_RELENTLESS_ASSAULT)
        .value("FLASK_FORTIFICATION", FLASK_FORTIFICATION)
        .value("FLASK_CHROMATIC_WONDER", FLASK_CHROMATIC_WONDER)
        ;

    emscripten::enum_<Elixir>("Elixir")
        .value("ELIXIR_NONE", ELIXIR_NONE)
        .value("ELIXIR_MAJOR_AGILITY", ELIXIR_MAJOR_AGILITY)
        .value("ELIXIR_MAJOR_STRENGTH", ELIXIR_MAJOR_STRENGTH)
        .value("ELIXIR_MASTERY", ELIXIR_MASTERY)
        .value("ELIXIR_MAJOR_FORTITUDE", ELIXIR_MAJOR_FORTITUDE)
        ;

    emscripten::enum_<Drums>("Drums")
        .value("DRUMS_NONE", DRUMS_NONE)
        .value("DRUMS_OF_WAR", DRUMS_OF_WAR)
        .value("DRUMS_OF_BATTLE", DRUMS_OF_BATTLE);

    emscripten::enum_<WeaponOil>("WeaponOil")
        .value("OIL_NONE", OIL_NONE)
        .value("OIL_ADAMANTITE_WEIGHTSTONE", OIL_ADAMANTITE_WEIGHTSTONE);

    emscripten::enum_<Potion>("Potion")
        .value("POTION_NONE", POTION_NONE)
        .value("POTION_HASTE", POTION_HASTE)
        .value("POTION_INSANE_STRENGTH", POTION_INSANE_STRENGTH);

    emscripten::enum_<LogType>("LogType")
        .value("LOG_NONE", LOG_NONE)
        .value("LOG_SPELL", LOG_SPELL)
        .value("LOG_MELEE", LOG_MELEE)
        .value("LOG_RAGE", LOG_RAGE)
        .value("LOG_BUFF", LOG_BUFF)
        .value("LOG_DEBUG", LOG_DEBUG);

    emscripten::class_<Config>("Config")
        .smart_ptr<std::shared_ptr<Config>>("Config")
        .property("duration", &Config::duration)
        .property("duration_variance", &Config::duration_variance)
        .property("rng_seed", &Config::rng_seed)
        .property("avg_spell_dmg", &Config::avg_spell_dmg)
        .property("additional_data", &Config::additional_data)
        .property("targets", &Config::targets)

        .property("mh_weapon_speed", &Config::mh_weapon_speed)
        .property("mh_low_dmg", &Config::mh_low_dmg)
        .property("mh_high_dmg", &Config::mh_high_dmg)
        .property("oh_weapon_speed", &Config::oh_weapon_speed)
        .property("oh_low_dmg", &Config::oh_low_dmg)
        .property("oh_high_dmg", &Config::oh_high_dmg)
        .property("dual_wield", &Config::dual_wield)

        .property("sunder_armor", &Config::sunder_armor)
        .property("expose_armor", &Config::expose_armor)
        .property("curse_of_recklessness", &Config::curse_of_recklessness)
        .property("faerie_fire", &Config::faerie_fire)
        .property("blood_frenzy", &Config::blood_frenzy)
        .property("mangle", &Config::mangle)
        .property("gift_of_arthas", &Config::gift_of_arthas)
        .property("improved_hunters_mark", &Config::improved_hunters_mark)

        .property("battle_shout", &Config::battle_shout)
        .property("commanding_shout", &Config::commanding_shout)
        .property("blessing_of_kings", &Config::blessing_of_kings)
        .property("blessing_of_might", &Config::blessing_of_might)
        .property("mark_of_the_wild", &Config::mark_of_the_wild)
        .property("strength_of_earth", &Config::strength_of_earth)
        .property("grace_of_air", &Config::grace_of_air)
        .property("windfury_totem", &Config::windfury_totem)
        .property("unleashed_rage", &Config::unleashed_rage)
        .property("leader_of_the_pack", &Config::leader_of_the_pack)
        .property("trueshot_aura", &Config::trueshot_aura)
        .property("battle_squawk", &Config::battle_squawk)
        .property("improved_battle_shout", &Config::improved_battle_shout)
        .property("improved_blessing_of_might", &Config::improved_blessing_of_might)
        .property("improved_strength_of_earth", &Config::improved_strength_of_earth)
        .property("improved_grace_of_air", &Config::improved_grace_of_air)
        .property("improved_mark_of_the_wild", &Config::improved_mark_of_the_wild)

        .property("food", &Config::food)
        .property("flask", &Config::flask)
        .property("battle_elixir", &Config::battle_elixir)
        .property("guardian_elixir", &Config::guardian_elixir)
        .property("weapon_oil", &Config::weapon_oil)
        .property("drums", &Config::drums)
        .property("potion", &Config::potion)
        .property("first_potion", &Config::first_potion)

        .property("trinket1", &Config::trinket1)
        .property("trinket2", &Config::trinket2)
        .property("meta_gem", &Config::meta_gem)

        .property("bloodlust", &Config::bloodlust)
        .property("death_wish", &Config::death_wish)
        .property("recklessness", &Config::recklessness)
        .property("bloodrage", &Config::bloodrage)
        .property("sweeping_strikes", &Config::sweeping_strikes)
        .property("drums_friend", &Config::drums_friend)

        .property("main_rotation", &Config::main_rotation)
        .property("use_heroic_strike", &Config::use_heroic_strike)
        .property("heroic_strike_rage_threshold", &Config::heroic_strike_rage_threshold)
        .property("use_whirlwind", &Config::use_whirlwind)
        .property("use_execute", &Config::use_execute)
        .property("execute_phase_start", &Config::execute_phase_start)

        .property("trinket1_t", &Config::trinket1_t)
        .property("trinket2_t", &Config::trinket2_t)
        .property("death_wish_t", &Config::death_wish_t)
        .property("recklessness_t", &Config::recklessness_t)
        .property("bloodrage_t", &Config::bloodrage_t)
        .property("bloodlust_t", &Config::bloodlust_t)
        .property("drums_t", &Config::drums_t)
        .property("potion_t", &Config::potion_t)
        ;

    emscripten::function("allocConfig", &allocConfig);

    emscripten::value_object<Stats>("Stats")
        .field("strength", &Stats::strength)
        .field("agility", &Stats::agility)
        .field("stamina", &Stats::stamina)
        .field("crit", &Stats::crit)
        .field("hit", &Stats::hit)
        .field("haste", &Stats::haste)
        .field("expertise", &Stats::expertise)
        .field("armor_pen", &Stats::armor_pen)
        .field("attack_power", &Stats::attack_power)
        ;

    emscripten::class_<Player>("Player")
        .smart_ptr<std::shared_ptr<Player>>("Player")
        .property("stats", &Player::stats)
        .property("race", &Player::race)
        .function("setStats", &Player::setStats)
        .function("loadTalentsFromString", &Player::loadTalentsFromString);

    emscripten::function("allocPlayer", &allocPlayer);

    emscripten::value_object<SimulationResult>("SimulationResult")
        .field("dmg", &SimulationResult::dmg)
        .field("t", &SimulationResult::t)
        .field("dps", &SimulationResult::dps)
        .field("log", &SimulationResult::log)
        .field("spells", &SimulationResult::spells)
        ;

    emscripten::value_object<SimulationsResult>("SimulationsResult")
        .field("min_dps", &SimulationsResult::min_dps)
        .field("avg_dps", &SimulationsResult::avg_dps)
        .field("max_dps", &SimulationsResult::max_dps)
        .field("iterations", &SimulationsResult::iterations)
        .field("histogram", &SimulationsResult::histogram)
        .field("all_results", &SimulationsResult::all_results)
        .field("stats", &SimulationsResult::stats)
        ;

    emscripten::function("runSimulation", &runSimulation);
    emscripten::function("runSimulations", &runSimulations);
}

#endif
