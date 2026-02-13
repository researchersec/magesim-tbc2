# Frontend Conversion Guide: Mage to Warrior

This document outlines all the changes needed to convert the frontend from displaying mage stats to warrior stats.

## Overview

The frontend is built with Vue.js and consists of:
- **App.vue** (4405 lines) - Main application component
- **items.js** (894 lines) - Item database
- **constants.js** - Enums and constants
- **simulation.js** - WebAssembly interface
- **helpers.js** - Utility functions
- Various Vue components for UI elements

## Key Changes Required

### 1. Stats Display (App.vue - Sidebar)

**Current Mage Stats:**
```javascript
- Intellect
- Spirit
- Mp5
- Spell power
- SP Arcane / SP Frost / SP Fire
- Crit (%)
- Hit (%)
- Haste (%)
```

**New Warrior Stats:**
```javascript
- Strength
- Agility
- Stamina
- Attack Power
- Crit (%)
- Hit (%)
- Haste (%)
- Expertise
- Armor Penetration
```

**Location in App.vue:** Lines ~80-130 (final_stats table)

### 2. Stat Weights / EP Calculations

**Current:**
```javascript
- Intellect (EP)
- Spirit (EP)
- Mp5 (EP)
- Spell power (EP)
- SP Arcane/Frost/Fire (EP)
- Crit rating (EP)
- Hit rating (EP)
- Haste rating (EP)
```

**New:**
```javascript
- Strength (EP)
- Agility (EP)
- Stamina (EP)
- Attack Power (EP)
- Crit rating (EP)
- Hit rating (EP)
- Haste rating (EP)
- Expertise rating (EP)
- Armor Penetration (EP)
```

**Location:** Lines ~140-220 (ep-stats section)

### 3. Item Table Headers

**Current Columns:**
- Spell power
- Crit rating
- Hit rating
- Haste rating
- Intellect
- Spirit
- Mp5

**New Columns:**
- Strength
- Agility
- Stamina
- Attack Power
- Crit rating
- Hit rating
- Haste rating
- Expertise
- Armor Pen

**Location:** Lines ~300-350 (item table headers)

### 4. Configuration Panel

Need to update the config panel to show warrior-specific options:

**Remove:**
- Spell rotation options (AB, AM, FB, SC, etc.)
- Mana regen rotation
- Evocation settings
- Innervate settings
- Mana Tide
- Vampiric Touch
- Spell-specific buffs (Arcane Intellect, Divine Spirit, Mage Armor, Molten Armor)
- Spell debuffs (Misery, Curse of Elements, Fire Vulnerability, Winter's Chill)

**Add:**
- Weapon configuration (Main Hand / Off-Hand speed and damage)
- Dual Wield toggle
- Rotation selection (Bloodthirst / Mortal Strike / Slam)
- Heroic Strike settings (rage threshold)
- Execute phase settings
- Bloodrage configuration
- Warrior buffs (Battle Shout, Windfury Totem, Strength of Earth, Unleashed Rage)
- Physical debuffs (Sunder Armor, Expose Armor, Curse of Recklessness, Blood Frenzy, Faerie Fire)

### 5. Items Database (items.js)

The entire items.js file needs to be replaced with warrior gear:

**Current:** Mage items (cloth, spell power, intellect, spirit)
**New:** Warrior items (plate, strength, agility, attack power)

**Item Slots:**
- weapon → Main Hand (2H or 1H)
- off_hand → Off-Hand weapon (for dual wield)
- ranged → Ranged weapon (gun/bow/thrown)
- head, neck, shoulder, back, chest, wrist, hands, waist, legs, feet, finger1, finger2, trinket1, trinket2

**Stats to track:**
```javascript
{
    id: number,
    title: string,
    str: number,      // Strength
    agi: number,      // Agility
    sta: number,      // Stamina
    ap: number,       // Attack Power
    crit: number,     // Crit rating
    hit: number,      // Hit rating
    haste: number,    // Haste rating
    exp: number,      // Expertise rating
    armor_pen: number, // Armor penetration
    sockets: array,   // Socket colors
    bonus: object,    // Socket bonus
    phase: number,    // Content phase
    q: string,        // Quality (epic/rare/uncommon)
    twohand: boolean, // Is 2H weapon
    weapon_speed: number, // Weapon speed
    weapon_dmg_min: number,
    weapon_dmg_max: number,
}
```

### 6. Enchants

**Remove:** Spell power enchants
**Add:** Warrior enchants
- Weapon: Mongoose, Executioner, Potency, Major Striking
- Head: Glyph of Ferocity, Glyph of Power
- Shoulder: Greater Inscription of the Blade/Vengeance
- Chest: Exceptional Stats
- Bracers: Brawn, Major Strength
- Gloves: Major Strength, Blasting, Assault
- Legs: Nethercobra Leg Armor, Clefthide Leg Armor
- Boots: Cat's Swiftness, Boar's Speed, Surefooted

### 7. Gems

**Remove:** Spell power gems
**Add:** Warrior gems
- Red: Strength gems (Bold Living Ruby, etc.)
- Yellow: Crit/Hit/Haste gems
- Blue: Stamina gems
- Meta: Relentless Earthstorm Diamond, Chaotic Skyfire Diamond

### 8. Consumables Display

**Update:**
- Food: Grilled Mudfish (+20 Agi), Ravager Dog (+20 Str), Roasted Clefthoof (+20 Str)
- Flask: Relentless Assault (+120 AP), Fortification (+500 Sta)
- Elixir: Major Agility (+35 Agi), Major Strength (+35 Str)
- Weapon Oil: Adamantite Weightstone (+28 AP)
- Potion: Haste Potion, Insane Strength Potion

### 9. Buff/Debuff Display

**Update buff list:**
- Battle Shout
- Blessing of Might
- Blessing of Kings
- Mark of the Wild
- Strength of Earth Totem
- Grace of Air Totem
- Windfury Totem
- Unleashed Rage
- Leader of the Pack
- Trueshot Aura
- Bloodlust/Heroism

**Update debuff list:**
- Sunder Armor (5 stacks)
- Expose Armor
- Curse of Recklessness
- Faerie Fire
- Blood Frenzy
- Mangle

### 10. Simulation Results Display

**Update spell/ability names in results:**
- Remove: Arcane Blast, Frostbolt, Fireball, etc.
- Add: Bloodthirst, Mortal Strike, Whirlwind, Execute, Heroic Strike, Melee (MH), Melee (OH)

**Update result stats:**
- Remove: "Evocated at", "Filler at", "Wasted haste"
- Keep: DPS, Damage, Combat log, Timeline, Spells breakdown

### 11. Character Paperdoll

Update the character display to show:
- Plate armor visuals
- 2H weapon or Dual Wield weapons
- Warrior-appropriate gear

### 12. Talent String Parser

Update to parse warrior talent strings (Arms/Fury/Protection) instead of mage talents (Arcane/Fire/Frost).

Format: `XX/XX/XX` where each number represents points in that tree.

### 13. Profile Import/Export

Update profile system to handle warrior stats and configuration.

### 14. Comparison Tool

Update item comparison to calculate DPS differences based on warrior stats (Strength, Agility, AP) instead of spell power.

### 15. Meta Gem Requirements

Update meta gem requirement checking:
- Relentless Earthstorm: Requires more blue gems than red
- Chaotic Skyfire: Requires 2 blue gems

## Implementation Priority

1. **High Priority (Core Functionality):**
   - Stats display (sidebar)
   - Item table columns
   - Constants.js (already done)
   - Basic configuration panel

2. **Medium Priority (Usability):**
   - Items database
   - Enchants list
   - Gems list
   - Consumables display

3. **Low Priority (Polish):**
   - Character paperdoll
   - Talent string parser
   - Profile import/export
   - Comparison tool refinements

## Code Locations

### App.vue Structure:
```
Lines 1-50: Template start, modals
Lines 50-150: Sidebar (stats, EP, results)
Lines 150-300: Gear selection interface
Lines 300-500: Item table
Lines 500-700: Enchants table
Lines 700-900: Gems/sockets
Lines 900-1500: Character paperdoll
Lines 1500-2000: Configuration panel
Lines 2000-2500: Modals (log, timeline, spells)
Lines 2500-4405: JavaScript (data, methods, computed)
```

### Key Methods to Update:
- `finalStats()` - Calculate final warrior stats
- `runEP()` - Calculate stat weights for warrior stats
- `formatStats()` - Format stat display
- `activeItems()` - Filter items by slot
- `equipToggle()` - Equip item and recalculate stats

### Key Data Properties:
- `player.stats` - Change from mage stats to warrior stats
- `config` - Update configuration options
- `items` - Replace with warrior items database
- `enchants` - Replace with warrior enchants
- `gems` - Update gem database

## Testing Checklist

After making changes, test:
- [ ] Stats display correctly in sidebar
- [ ] EP calculations work for warrior stats
- [ ] Items filter and display correctly
- [ ] Enchants show warrior-appropriate options
- [ ] Gems show strength/agility/stamina options
- [ ] Configuration panel shows warrior options
- [ ] Simulation runs and returns results
- [ ] Combat log shows warrior abilities
- [ ] Item comparison calculates correctly
- [ ] Profile save/load works
- [ ] Meta gem requirements check correctly

## Notes

- The WebAssembly module (warriorsim.js) is already updated via the backend conversion
- The simulation.js file may need minor updates to match new stat names
- Consider keeping the UI layout similar to minimize changes
- Focus on functionality first, polish later
- Test thoroughly after each major change

## Quick Start

To make the frontend minimally functional:

1. Update stats display in sidebar (lines 80-130 of App.vue)
2. Update item table headers (lines 300-350)
3. Create basic warrior items database
4. Update configuration panel to show warrior options
5. Test simulation runs

This will give you a working warrior sim with basic functionality. Polish and additional features can be added incrementally.
