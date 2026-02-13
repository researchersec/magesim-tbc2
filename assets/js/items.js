// TBC Warrior Items Database
// Stats: str (Strength), agi (Agility), sta (Stamina), ap (Attack Power)
//        crit (Crit rating), hit (Hit rating), haste (Haste rating)
//        exp (Expertise rating), armor_pen (Armor Penetration)

var ids = {
    // Trinkets
    DRAGONSPINE_TROPHY: 28830,
    BLOODLUST_BROOCH: 28830,
    ABACUS: 28288,
    TSUNAMI_TALISMAN: 30627,
    HOURGLASS: 28034,
    BERSERKERS_CALL: 23558,
    SLAYERS_CREST: 28121,
    BADGE_TENACITY: 28528,
    ICON_UNYIELDING_COURAGE: 28041,
    SHARD_CONTEMPT: 34472,
    MADNESS: 32505,
    BLACKENED_NAARU_SLIVER: 34427,
    ROMULOS_POISON_VIAL: 28579,
    BLOODMOON: 32492,
    CRUSADE: 31856,
    
    // Meta Gems
    RELENTLESS_EARTHSTORM: 32409,
    CHAOTIC_SKYFIRE: 34220,
};

var items = {
    weapon: [
        // Phase 5 - Sunwell
        { id: 34247, title: "Apolyon, the Soul-Render", str: 44, crit: 37, hit: 30, haste: 37, twohand: true, weapon_speed: 3.6, weapon_dmg_min: 471, weapon_dmg_max: 707, phase: 5 },
        { id: 34331, title: "Hand of the Deceiver", str: 30, crit: 26, hit: 19, weapon_speed: 2.6, weapon_dmg_min: 189, weapon_dmg_max: 352, phase: 5 },
        { id: 34346, title: "Mounting Vengeance", str: 30, crit: 26, armor_pen: 28, weapon_speed: 2.6, weapon_dmg_min: 189, weapon_dmg_max: 352, phase: 5 },
        
        // Phase 3 - Black Temple
        { id: 32837, title: "Warglaive of Azzinoth", agi: 33, sta: 28, ap: 528, haste: 44, weapon_speed: 2.8, weapon_dmg_min: 222, weapon_dmg_max: 414, phase: 3 },
        { id: 32332, title: "Torch of the Damned", str: 30, crit: 26, hit: 19, weapon_speed: 2.6, weapon_dmg_min: 189, weapon_dmg_max: 352, phase: 3 },
        { id: 32369, title: "Blade of Savagery", str: 30, crit: 26, exp: 19, weapon_speed: 2.6, weapon_dmg_min: 189, weapon_dmg_max: 352, phase: 3 },
        { id: 32262, title: "Syphon of the Nathrezim", str: 44, crit: 37, hit: 30, twohand: true, weapon_speed: 3.6, weapon_dmg_min: 471, weapon_dmg_max: 707, phase: 3 },
        
        // Phase 2 - SSC/TK
        { id: 30090, title: "World Breaker", str: 44, crit: 37, hit: 30, twohand: true, weapon_speed: 3.8, weapon_dmg_min: 521, weapon_dmg_max: 783, phase: 2 },
        { id: 30082, title: "Talon of Azshara", str: 30, crit: 26, hit: 19, weapon_speed: 2.6, weapon_dmg_min: 189, weapon_dmg_max: 352, phase: 2 },
        { id: 29996, title: "Rod of the Sun King", str: 30, crit: 26, weapon_speed: 2.6, weapon_dmg_min: 189, weapon_dmg_max: 352, phase: 2 },
        
        // Phase 1 - Karazhan/Gruul/Mag
        { id: 28773, title: "Gorehowl", str: 44, crit: 37, hit: 30, twohand: true, weapon_speed: 3.6, weapon_dmg_min: 471, weapon_dmg_max: 707, phase: 1 },
        { id: 28767, title: "The Decapitator", str: 44, crit: 37, twohand: true, weapon_speed: 3.6, weapon_dmg_min: 471, weapon_dmg_max: 707, phase: 1 },
        { id: 28729, title: "Terestian's Stranglestaff", str: 44, crit: 37, twohand: true, weapon_speed: 3.6, weapon_dmg_min: 471, weapon_dmg_max: 707, phase: 1 },
        { id: 28825, title: "Aldori Legacy Defender", str: 30, crit: 26, weapon_speed: 2.6, weapon_dmg_min: 189, weapon_dmg_max: 352, phase: 1 },
        { id: 28658, title: "Terestian's Stranglestaff", str: 30, crit: 26, weapon_speed: 2.6, weapon_dmg_min: 189, weapon_dmg_max: 352, phase: 1 },
        
        // Crafted/Dungeon
        { id: 30082, title: "Lionheart Champion", str: 44, crit: 37, twohand: true, weapon_speed: 3.6, weapon_dmg_min: 471, weapon_dmg_max: 707, q: "rare" },
        { id: 29348, title: "The Bladefist", str: 30, crit: 26, weapon_speed: 2.6, weapon_dmg_min: 189, weapon_dmg_max: 352, q: "rare" },
    ],
    
    off_hand: [
        // Dual Wield Off-hands
        { id: 34331, title: "Hand of the Deceiver", str: 30, crit: 26, hit: 19, weapon_speed: 2.6, weapon_dmg_min: 189, weapon_dmg_max: 352, phase: 5 },
        { id: 32837, title: "Warglaive of Azzinoth", agi: 33, sta: 28, ap: 528, haste: 44, weapon_speed: 2.8, weapon_dmg_min: 222, weapon_dmg_max: 414, phase: 3 },
        { id: 32332, title: "Torch of the Damned", str: 30, crit: 26, hit: 19, weapon_speed: 2.6, weapon_dmg_min: 189, weapon_dmg_max: 352, phase: 3 },
        { id: 30082, title: "Talon of Azshara", str: 30, crit: 26, hit: 19, weapon_speed: 2.6, weapon_dmg_min: 189, weapon_dmg_max: 352, phase: 2 },
    ],
    
    ranged: [
        { id: 34196, title: "Golden Staff of the Sin'dorei", str: 18, crit: 22, hit: 15, phase: 5 },
        { id: 32336, title: "Black Bow of the Betrayer", agi: 28, sta: 30, ap: 56, crit: 28, hit: 19, phase: 3 },
        { id: 30105, title: "Serpent Spine Longbow", agi: 28, sta: 30, ap: 56, crit: 28, phase: 2 },
        { id: 28772, title: "Sunfury Bow of the Phoenix", agi: 28, sta: 30, ap: 56, crit: 28, phase: 1 },
        { id: 29351, title: "Adamantite Figurine", agi: 20, sta: 24, ap: 40, crit: 20, q: "rare" },
    ],
    
    head: [
        { id: 34345, title: "Crown of Anasterian", str: 42, sta: 51, crit: 38, hit: 30, haste: 30, sockets: ["m", "r"], bonus: { str: 4 }, phase: 5 },
        { id: 32235, title: "Cursed Vision of Sargeras", str: 42, sta: 51, crit: 38, hit: 30, sockets: ["m", "r"], bonus: { str: 4 }, phase: 3 },
        { id: 30972, title: "Destroyer Greathelm", str: 42, sta: 51, crit: 38, sockets: ["m", "r"], bonus: { str: 4 }, phase: 2 },
        { id: 29021, title: "Helm of the Fallen Champion", str: 42, sta: 51, crit: 38, sockets: ["m", "r"], bonus: { str: 4 }, phase: 1 },
        { id: 32461, title: "Furious Gizmatic Goggles", str: 36, sta: 45, crit: 32, hit: 24, sockets: ["m", "b"], bonus: { str: 4 }, phase: 2, q: "rare" },
    ],
    
    neck: [
        { id: 34177, title: "Clutch of Demise", str: 19, sta: 24, crit: 25, haste: 25, phase: 5 },
        { id: 32260, title: "Choker of Endless Nightmares", str: 19, sta: 24, crit: 25, hit: 19, phase: 3 },
        { id: 30017, title: "Telonicus's Pendant of Mayhem", str: 19, sta: 24, crit: 25, phase: 2 },
        { id: 28509, title: "Worgen Claw Necklace", str: 19, sta: 24, crit: 25, phase: 1 },
        { id: 29381, title: "Choker of Vile Intent", str: 16, sta: 21, ap: 64, crit: 21, q: "rare" },
    ],
    
    shoulder: [
        { id: 34388, title: "Shoulders of the Forgotten Conqueror", str: 33, sta: 42, crit: 26, hit: 21, haste: 29, sockets: ["r", "y"], bonus: { str: 4 }, phase: 5 },
        { id: 32361, title: "Mantle of Darkness", str: 33, sta: 42, crit: 26, hit: 21, sockets: ["r", "y"], bonus: { str: 4 }, phase: 3 },
        { id: 30979, title: "Destroyer Shoulderguards", str: 33, sta: 42, crit: 26, sockets: ["r", "y"], bonus: { str: 4 }, phase: 2 },
        { id: 29023, title: "Pauldrons of the Fallen Champion", str: 33, sta: 42, crit: 26, sockets: ["r", "y"], bonus: { str: 4 }, phase: 1 },
    ],
    
    back: [
        { id: 34241, title: "Cloak of Unforgivable Sin", str: 26, sta: 33, crit: 32, phase: 5 },
        { id: 32323, title: "Shadowmoon Destroyer's Drape", str: 26, sta: 33, crit: 32, phase: 3 },
        { id: 30098, title: "Razor-Scale Battlecloak", str: 26, sta: 33, crit: 32, phase: 2 },
        { id: 28672, title: "Drape of the Dark Reavers", str: 26, sta: 33, crit: 32, phase: 1 },
        { id: 29994, title: "Thalassian Wildercloak", agi: 20, sta: 27, ap: 40, crit: 24, q: "rare" },
    ],
    
    chest: [
        { id: 34397, title: "Breastplate of Agony's Aversion", str: 40, sta: 51, crit: 40, hit: 33, haste: 27, sockets: ["r", "r", "y"], bonus: { str: 6 }, phase: 5 },
        { id: 32337, title: "Garments of Crashing Shores", str: 40, sta: 51, crit: 40, hit: 33, sockets: ["r", "r", "y"], bonus: { str: 6 }, phase: 3 },
        { id: 30975, title: "Destroyer Breastplate", str: 40, sta: 51, crit: 40, sockets: ["r", "r", "y"], bonus: { str: 6 }, phase: 2 },
        { id: 29019, title: "Breastplate of the Fallen Champion", str: 40, sta: 51, crit: 40, sockets: ["r", "r", "y"], bonus: { str: 6 }, phase: 1 },
    ],
    
    wrist: [
        { id: 34433, title: "Bracers of the Forgotten Conqueror", str: 20, sta: 27, crit: 28, haste: 28, sockets: ["r"], bonus: { str: 2 }, phase: 5 },
        { id: 32324, title: "Insidious Bands", str: 20, sta: 27, crit: 28, hit: 19, phase: 3 },
        { id: 30863, title: "Vambraces of Ending", str: 20, sta: 27, crit: 28, phase: 2 },
        { id: 28795, title: "Bladespire Warbands", str: 20, sta: 27, crit: 28, phase: 1 },
    ],
    
    hands: [
        { id: 34350, title: "Gauntlets of the Sunken Dead", str: 32, sta: 42, crit: 37, haste: 36, sockets: ["r", "y"], bonus: { str: 4 }, phase: 5 },
        { id: 32347, title: "Grips of Damnation", str: 32, sta: 42, crit: 37, hit: 27, sockets: ["r", "y"], bonus: { str: 4 }, phase: 3 },
        { id: 30970, title: "Destroyer Gauntlets", str: 32, sta: 42, crit: 37, sockets: ["r", "y"], bonus: { str: 4 }, phase: 2 },
        { id: 29020, title: "Gauntlets of the Fallen Champion", str: 32, sta: 42, crit: 37, sockets: ["r", "y"], bonus: { str: 4 }, phase: 1 },
    ],
    
    waist: [
        { id: 34485, title: "Forgotten Conqueror's Girdle", str: 29, sta: 39, crit: 29, hit: 24, haste: 29, sockets: ["r", "y"], bonus: { str: 4 }, phase: 5 },
        { id: 32346, title: "Girdle of the Lightbearer", str: 29, sta: 39, crit: 29, hit: 24, sockets: ["r", "y"], bonus: { str: 4 }, phase: 3 },
        { id: 30106, title: "Belt of One-Hundred Deaths", str: 29, sta: 39, crit: 29, sockets: ["r", "y"], bonus: { str: 4 }, phase: 2 },
        { id: 28779, title: "Girdle of the Endless Pit", str: 29, sta: 39, crit: 29, phase: 1 },
    ],
    
    legs: [
        { id: 34382, title: "Legplates of the Forgotten Conqueror", str: 40, sta: 51, crit: 40, hit: 33, sockets: ["r", "y", "b"], bonus: { str: 6 }, phase: 5 },
        { id: 32341, title: "Leggings of Divine Retribution", str: 40, sta: 51, crit: 40, hit: 33, sockets: ["r", "y", "b"], bonus: { str: 6 }, phase: 3 },
        { id: 30977, title: "Destroyer Legguards", str: 40, sta: 51, crit: 40, sockets: ["r", "y", "b"], bonus: { str: 6 }, phase: 2 },
        { id: 29022, title: "Legplates of the Fallen Champion", str: 40, sta: 51, crit: 40, sockets: ["r", "y", "b"], bonus: { str: 6 }, phase: 1 },
    ],
    
    feet: [
        { id: 34561, title: "Boots of the Forgotten Conqueror", str: 32, sta: 42, crit: 37, haste: 36, sockets: ["r", "y"], bonus: { str: 4 }, phase: 5 },
        { id: 32345, title: "Dreadboots of the Legion", str: 32, sta: 42, crit: 37, hit: 27, sockets: ["r"], bonus: { str: 2 }, phase: 3 },
        { id: 30969, title: "Destroyer Greaves", str: 32, sta: 42, crit: 37, sockets: ["r", "y"], bonus: { str: 4 }, phase: 2 },
        { id: 28810, title: "Boots of Utter Darkness", str: 32, sta: 42, crit: 37, phase: 1 },
    ],
    
    finger: [
        { id: 34361, title: "Stormrage Signet Ring", str: 19, sta: 24, crit: 25, haste: 25, phase: 5 },
        { id: 32497, title: "Stormrage Signet Ring", str: 19, sta: 24, crit: 25, hit: 19, phase: 3 },
        { id: 30052, title: "Ring of Lethality", str: 19, sta: 24, crit: 25, phase: 2 },
        { id: 28757, title: "Ring of a Thousand Marks", str: 19, sta: 24, crit: 25, phase: 1 },
        { id: 29301, title: "Band of the Ranger-General", agi: 16, sta: 21, ap: 64, crit: 21, q: "rare" },
    ],
    
    trinket: [
        { id: ids.DRAGONSPINE_TROPHY, title: "Dragonspine Trophy", ap: 40, phase: 1 },
        { id: ids.BLOODLUST_BROOCH, title: "Bloodlust Brooch", ap: 72, phase: 1 },
        { id: ids.ABACUS, title: "Abacus of Violent Odds", ap: 64, phase: 1 },
        { id: ids.TSUNAMI_TALISMAN, title: "Tsunami Talisman", ap: 84, phase: 2 },
        { id: ids.HOURGLASS, title: "Hourglass of the Unraveller", crit: 32, phase: 1 },
        { id: ids.BERSERKERS_CALL, title: "Berserker's Call", ap: 90, phase: 1 },
        { id: ids.SLAYERS_CREST, title: "Slayer's Crest", ap: 64, crit: 28, phase: 1 },
        { id: ids.BADGE_TENACITY, title: "Badge of Tenacity", sta: 30, phase: 1 },
        { id: ids.ICON_UNYIELDING_COURAGE, title: "Icon of Unyielding Courage", ap: 84, phase: 1 },
        { id: ids.SHARD_CONTEMPT, title: "Shard of Contempt", ap: 44, phase: 5 },
        { id: ids.MADNESS, title: "Madness of the Betrayer", armor_pen: 84, phase: 3 },
        { id: ids.BLACKENED_NAARU_SLIVER, title: "Blackened Naaru Sliver", ap: 44, phase: 5 },
        { id: ids.ROMULOS_POISON_VIAL, title: "Romulo's Poison Vial", ap: 80, phase: 1 },
        { id: ids.BLOODMOON, title: "Bloodmoon", ap: 80, phase: 3 },
        { id: ids.CRUSADE, title: "Darkmoon Card: Crusade", str: 8, phase: 2 },
    ],
    
    enchants: {
        weapon: [
            { id: 27984, title: "Mongoose", agi: 120, haste: 30 },
            { id: 27975, title: "Executioner", armor_pen: 840 },
            { id: 22749, title: "Potency", str: 20 },
            { id: 27968, title: "Major Striking", ap: 14 },
        ],
        head: [
            { id: 35447, title: "Glyph of Ferocity", str: 34, crit: 16, hit: 16 },
            { id: 35445, title: "Glyph of Power", str: 18, crit: 20, hit: 20 },
        ],
        shoulder: [
            { id: 28888, title: "Greater Inscription of the Blade", crit: 15, ap: 30 },
            { id: 28889, title: "Greater Inscription of Vengeance", ap: 30, crit: 15 },
            { id: 28910, title: "Greater Inscription of Warding", sta: 15 },
        ],
        chest: [
            { id: 27960, title: "Exceptional Stats", str: 6, agi: 6, sta: 6 },
            { id: 33990, title: "Major Resilience", sta: 15 },
        ],
        wrist: [
            { id: 27899, title: "Brawn", str: 12 },
            { id: 23802, title: "Major Strength", str: 9 },
            { id: 34002, title: "Assault", ap: 24 },
        ],
        hands: [
            { id: 25080, title: "Major Strength", str: 15 },
            { id: 33995, title: "Blasting", str: 10 },
            { id: 33996, title: "Assault", ap: 26 },
        ],
        legs: [
            { id: 35554, title: "Nethercobra Leg Armor", ap: 50, crit: 12 },
            { id: 35549, title: "Clefthide Leg Armor", sta: 30, agi: 10 },
        ],
        feet: [
            { id: 34007, title: "Cat's Swiftness", agi: 6 },
            { id: 27954, title: "Boar's Speed", sta: 9 },
            { id: 27951, title: "Surefooted", crit: 5, hit: 5 },
        ],
        back: [
            { id: 25084, title: "Major Agility", agi: 12 },
            { id: 34004, title: "Greater Agility", agi: 16 },
        ],
        finger: [
            { id: 27927, title: "Striking", ap: 2 },
            { id: 27926, title: "Strength", str: 2 },
        ],
    },
    
    gems: [
        // Red
        { id: 24027, title: "Bold Living Ruby", str: 8, color: "red" },
        { id: 24028, title: "Delicate Living Ruby", agi: 8, color: "red" },
        { id: 32193, title: "Bold Crimson Spinel", str: 10, color: "red" },
        { id: 32194, title: "Delicate Crimson Spinel", agi: 10, color: "red" },
        // Yellow
        { id: 24031, title: "Smooth Dawnstone", crit: 8, color: "yellow" },
        { id: 24032, title: "Rigid Dawnstone", hit: 8, color: "yellow" },
        { id: 24033, title: "Quick Dawnstone", haste: 8, color: "yellow" },
        { id: 32205, title: "Smooth Lionseye", crit: 10, color: "yellow" },
        { id: 32206, title: "Rigid Lionseye", hit: 10, color: "yellow" },
        // Blue
        { id: 24036, title: "Solid Star of Elune", sta: 12, color: "blue" },
        { id: 32200, title: "Solid Empyrean Sapphire", sta: 15, color: "blue" },
        // Orange
        { id: 24058, title: "Inscribed Noble Topaz", str: 4, crit: 4, color: "orange" },
        { id: 24059, title: "Potent Noble Topaz", str: 4, crit: 4, color: "orange" },
        { id: 32217, title: "Inscribed Pyrestone", str: 5, crit: 5, color: "orange" },
        // Green
        { id: 24062, title: "Jagged Talasite", crit: 4, sta: 6, color: "green" },
        { id: 32220, title: "Jagged Seaspray Emerald", crit: 5, sta: 7, color: "green" },
        // Purple
        { id: 24054, title: "Sovereign Nightseye", str: 4, sta: 6, color: "purple" },
        { id: 32212, title: "Sovereign Shadowsong Amethyst", str: 5, sta: 7, color: "purple" },
        // Meta
        { id: ids.RELENTLESS_EARTHSTORM, title: "Relentless Earthstorm Diamond", agi: 12, crit: 3, color: "meta" },
        { id: ids.CHAOTIC_SKYFIRE, title: "Chaotic Skyfire Diamond", crit: 12, crit_dmg: 3, color: "meta" },
    ],
    
    quicksets: {
        prebis: {
            title: "Pre-raid BiS",
            items: {
                head: 32461,
                neck: 29381,
                shoulder: 29023,
                back: 29994,
                chest: 29019,
                wrist: 28795,
                hands: 29020,
                waist: 28779,
                legs: 29022,
                feet: 28810,
                finger1: 28757,
                finger2: 29301,
                trinket1: ids.BERSERKERS_CALL,
                trinket2: ids.HOURGLASS,
                weapon: 28773,
                ranged: 29351,
            }
        },
    },
};

export default items;
