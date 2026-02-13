# TBC Mage Sim to Warrior Sim Conversion

This document outlines the complete conversion from a TBC Mage simulator to a TBC Warrior simulator.

## Major Changes

### 1. Resource System
- **Mage**: Mana-based (0-max mana, regenerates over time)
- **Warrior**: Rage-based (0-100 cap, generated from damage dealt)

### 2. Stats System (src/stats.h)
**Removed:**
- intellect, spirit, mp5
- spell_power, spell_power_arcane, spell_power_frost, spell_power_fire

**Added:**
- strength, agility, stamina
- expertise, armor_pen
- attack_power

### 3. Combat Mechanics
**Mage:**
- Spell casting with cast times
- Spell hit/crit/miss
- Spell damage coefficients
- Mana costs

**Warrior:**
- Melee auto-attacks (main hand + off-hand)
- White damage + yellow damage (abilities)
- Attack hit/crit/miss/dodge/parry/glance
- Rage generation from damage dealt
- Weapon swing timers
- Normalized vs non-normalized attacks

### 4. Abilities (src/spell.h)
**Removed all mage spells:**
- Arcane Blast, Arcane Missiles, Frostbolt, Fireball, Scorch, Fire Blast, Pyroblast, etc.

**Added warrior abilities:**
- Bloodthirst (Fury 41-point talent)
- Mortal Strike (Arms 31-point talent)
- Whirlwind (Fury baseline)
- Execute (execute phase <20% health)
- Heroic Strike (rage dump)
- Slam (Arms spec)

### 5. Talents (src/talents.h)
**Removed:**
- All 3 mage talent trees (Arcane, Fire, Frost)

**Added:**
- Arms tree: Mortal Strike, Sweeping Strikes, Impale, Deep Wounds, Two-Handed Spec, weapon specializations
- Fury tree: Bloodthirst, Flurry, Rampage, Enrage, Dual Wield Spec, Death Wish, Precision, Cruelty
- Protection tree: (mostly unused for DPS)

### 6. Buffs (src/buff.h)
**Removed:**
- Arcane Power, Icy Veins, Combustion, Clearcast, Presence of Mind
- Mage-specific trinket buffs

**Added:**
- Death Wish (20% damage, 30s duration)
- Recklessness (100% crit, 15s duration)
- Bloodrage (rage generation)
- Rampage (5% crit buff)
- Flurry (30% attack speed, 3 charges)
- Enrage (damage and crit bonus)
- Warrior-specific trinket buffs (Dragonspine Trophy, Shard of Contempt, etc.)

### 7. Debuffs (src/debuff.h)
**Removed:**
- Fire Vulnerability, Winter's Chill

**Added:**
- Sunder Armor (armor reduction, 5 stacks)
- Expose Armor (armor reduction)
- Curse of Recklessness (3% damage increase)
- Blood Frenzy (4% physical damage increase)
- Faerie Fire (armor reduction)

### 8. Configuration (src/config.h)
**Removed:**
- Spell rotation options (AB, AM, FB, SC, FIB, FRB, AE)
- Mana regen rotation
- Evocation, Innervate, Mana Tide, Vampiric Touch
- Spell-specific buffs (Arcane Intellect, Divine Spirit, Mage Armor)
- Mage set bonuses (Tirisfal, Tempest, Spellfire, Spellstrike)

**Added:**
- Weapon configuration (main hand + off-hand speed and damage)
- Dual wield toggle
- Warrior rotation (Bloodthirst, Mortal Strike, Slam)
- Heroic Strike rage threshold
- Execute phase configuration
- Warrior buffs (Battle Shout, Windfury Totem, Strength of Earth, Unleashed Rage)
- Bloodrage configuration

### 9. Player Stats (src/player.h)
**Removed:**
- Mana calculation (base_mana, maxMana(), spiritManaPerSecond())
- Spell power from intellect
- Mage racial bonuses (spell hit, intellect)

**Added:**
- Attack power from strength (2 AP per strength)
- Crit from agility (1% per 25 agility)
- Racial expertise bonuses (Humans with swords/maces, Orcs with axes, Dwarves with maces)
- Warrior stat scaling

### 10. Simulation (src/simulation.h)
**Major changes:**
- Removed spell casting queue and mana regen events
- Added melee swing timer events (main hand + off-hand)
- Rage generation from damage dealt (dmg / 274.7 * 7.5)
- Attack table: miss → dodge → parry → glance → crit → hit
- Glancing blows (24% chance, ~75% damage)
- Flurry charge consumption on swings
- Deep Wounds bleed application on crits
- Execute phase detection (boss health <20%)
- Heroic Strike queuing system
- Bloodrage rage generation (10 initial + 1 per second for 10 seconds)

### 11. Damage Calculation
**Mage:**
```cpp
dmg = spell_base_dmg + (spell_power * coefficient)
dmg *= buff_multipliers * debuff_multipliers
```

**Warrior:**
```cpp
weapon_dmg = random(weapon_low, weapon_high)
if (normalized)
    dmg = weapon_dmg + (AP / 14.0) * normalized_speed
else
    dmg = weapon_dmg + (AP / 14.0) * weapon_speed
dmg += bonus_dmg
dmg *= buff_multipliers * debuff_multipliers
```

### 12. Consumables
**Removed:**
- Mana potions, Fel Mana Potion
- Destruction Potion (spell crit)
- Mana gems
- Spell power food

**Added:**
- Haste Potion (400 haste rating)
- Insane Strength Potion (120 strength)
- Strength/Agility food
- Adamantite Weightstone

### 13. Trinkets
**Removed all mage trinkets:**
- Serpent Coil, Silver Crescent, Quagmirran's Eye, Lightning Capacitor, etc.

**Added warrior trinkets:**
- Bloodlust Brooch, Dragonspine Trophy, Shard of Contempt
- Madness of the Betrayer, Blackened Naaru Sliver
- Tsunami Talisman, Hourglass of the Unraveller
- Slayer's Crest, Badge of Tenacity

### 14. Rotation Logic
**Mage:**
- Priority: Arcane Blast spam with mana management
- Filler rotation when low mana
- Evocation when OOM
- Cooldown usage (Arcane Power, Icy Veins, Combustion)

**Warrior:**
- Priority: BT/MS on cooldown → Whirlwind → wait
- Heroic Strike when rage > threshold
- Execute spam in execute phase
- Cooldown usage (Death Wish, Recklessness, Bloodrage)
- Auto-attacks running continuously

## Files Modified

1. **src/stats.h** - Complete rewrite for warrior stats
2. **src/common.h** - Updated enums and constants
3. **src/talents.h** - Complete rewrite for warrior talents
4. **src/spell.h** - Complete rewrite for warrior abilities
5. **src/buff.h** - Complete rewrite for warrior buffs
6. **src/debuff.h** - Complete rewrite for warrior debuffs
7. **src/cooldown.h** - Updated for warrior cooldowns
8. **src/dot.h** - Updated for Deep Wounds and Rend
9. **src/config.h** - Complete rewrite for warrior configuration
10. **src/state.h** - Updated for rage instead of mana
11. **src/player.h** - Complete rewrite for warrior stats and racials
12. **src/simulation.h** - Complete rewrite for melee combat
13. **src/bindings.h** - Updated WebAssembly bindings
14. **src/main.cpp** - Minor cleanup
15. **readme.md** - Updated title
16. **package.json** - Updated name and description
17. **index.html** - Updated title and script name
18. **Makefile** - Updated output filename

## Key Formulas

### Rage Generation
```cpp
rage = damage_dealt / 274.7 * 7.5
// Off-hand generates 50% rage
```

### Attack Power
```cpp
AP = base_AP + (strength * 2) + buffs
```

### Crit Chance
```cpp
crit = base_crit + (agility / 25.0) + talents + buffs
```

### Weapon Damage
```cpp
// Normalized (special attacks)
dmg = weapon_dmg + (AP / 14.0) * normalized_speed

// Non-normalized (white hits)
dmg = weapon_dmg + (AP / 14.0) * weapon_speed
```

### Attack Table (Boss, Level 73)
1. Miss: 9% base (27% dual wield) - hit rating
2. Dodge: 6.5% - expertise
3. Parry: 14% - expertise
4. Glance: 24% (white hits only)
5. Crit: based on stats
6. Hit: remainder

### Swing Speed
```cpp
swing_time = weapon_speed / haste_multiplier
// Flurry: 1.0 + (0.05 * talent_points)
// Bloodlust: 1.3
// Windfury Totem: 1.16
```

## Testing Recommendations

1. Verify rage generation from white hits
2. Test Flurry charge consumption
3. Verify Deep Wounds application and stacking
4. Test execute phase transition
5. Verify Heroic Strike queuing
6. Test dual wield vs 2H damage
7. Verify all trinket procs
8. Test cooldown timings (Death Wish, Bloodrage, Recklessness)
9. Verify attack table (miss/dodge/parry/glance/crit/hit rates)
10. Test Bloodthirst vs Mortal Strike rotations

## Known Limitations

1. Frontend (Vue.js) not updated - still shows mage UI
2. No Slam rotation implementation (basic structure only)
3. Sweeping Strikes not implemented
4. Rend not implemented
5. Weapon proc effects not implemented (e.g., Mongoose, Executioner)
6. Armor penetration not fully implemented
7. Boss armor value not configurable
8. No stance dancing mechanics
9. Rage decay not implemented (minor impact in combat)
10. Windfury Totem extra attacks not implemented (simplified to haste)

## Next Steps for Full Functionality

1. Update Vue.js frontend to show warrior stats and abilities
2. Implement Slam rotation logic
3. Add weapon proc system
4. Implement boss armor and armor penetration calculations
5. Add more warrior-specific trinkets and set bonuses
6. Implement Sweeping Strikes cleave damage
7. Add rage decay out of combat
8. Implement proper Windfury Totem extra attack procs
9. Add more rotation options (Slam spec, Execute spam)
10. Implement stance mechanics if needed

## Build Instructions

Backend (C++ → WebAssembly):
```bash
make
```

Frontend (Vue.js):
```bash
npx mix
```

Run locally:
```bash
python3 -m http.server
```

Then open http://localhost:8000 in your browser.
