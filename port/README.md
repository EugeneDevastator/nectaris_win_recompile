# Nectaris — portable engine (raylib)

A modern C++ port of the tactical core of **Nectaris (1997, Windows 95)**.
Cutscenes, the intro and the story VM are intentionally skipped. Everything the
game needs is plain JSON + PNG, so missions and units can be edited in a text
editor / image editor.

## Build

Requires CMake ≥ 3.20 and a C++17 compiler. The first configure fetches raylib
and nlohmann/json.

```powershell
cmake -S port -B port/build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release `
  -DCMAKE_C_COMPILER=C:/msys64/mingw64/bin/gcc.exe `
  -DCMAKE_CXX_COMPILER=C:/msys64/mingw64/bin/g++.exe
cmake --build port/build -j 8
```

## Run

```powershell
port/build/nectaris.exe assets assets/maps/all_units_test.json
```

`assets` is the asset directory; the second argument is the mission.
The test mission puts every unit type on the field for both sides.

Controls:

| input | action |
|---|---|
| LMB on own unit | select, show movement range |
| LMB on blue cell | move |
| LMB on adjacent enemy | attack |
| RMB / ESC | deselect |
| SPACE | end turn (enemy AI acts) |
| N | next campaign map |
| R | restart mission |
| F1 | debug cell info |

## Combat self-test

Prints a 24×24 attacker/defender damage matrix (open in a spreadsheet):

```powershell
port/build/nectaris.exe assets assets/maps/all_units_test.json --selftest > selftest.csv
```

Headless AI demo (prints 3 rounds of enemy actions, including ranged fire):

```powershell
port/build/nectaris.exe assets assets/maps/all_units_test.json --demo
```

Combat screenshot (last argument: `0` = direct layout, `1` = ranged layout):

```powershell
port/build/nectaris.exe assets assets/maps/all_units_test.json --combatshot direct.png 1.4 0
port/build/nectaris.exe assets assets/maps/all_units_test.json --combatshot ranged.png 1.4 1
```

## Mission format

`assets/maps/<name>.json`:

```json
{
  "name": "my_mission",
  "width": 15,
  "height": 10,
  "background": "",
  "terrain": [
    [1,1,1,1,1,1,1,1,1,1,1,1,1,1,1],
    [1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]
  ],
  "units": [
    {"type": 4, "owner": 0, "x": 2, "y": 8, "hp": 7, "rank": 0},
    {"type": 6, "owner": 1, "x": 10, "y": 2, "hp": 7, "rank": 0}
  ]
}
```

* `terrain` is `height` rows of `width` values (0..15). Each value indexes
  `assets/data/terrain.json` for defense and move cost.
* `units[].owner`: `0` = player, `1` = enemy.
* `units[].type`: index into `assets/data/units.json` (0..23).
* `units[].hp`: 0..7 (7 = full).
* `units[].rank`: 0..7 (damage multiplier tier).
* `background`: optional PNG path relative to `assets/`; empty = colored terrain.

Campaign maps exported from the original are `assets/maps/<name>.json`;
the editor's unit-deployment variants are `assets/maps/deploy_<name>.json`
(the two source folders reuse filenames, so they are namespaced).

Adding a mission = drop a JSON file in `assets/maps/` and pass it on the command
line. No rebuild needed.

## Unit format

`assets/data/units.json`:

```json
{
  "id": 4,
  "name": "T-79",
  "attack_ground": 70,
  "attack_air": 0,
  "defense": 50,
  "counter_ground": 1,
  "counter_air": 1,
  "movement": 4,
  "movement_class": 0,
  "ability_flags": 17,
  "max_hp": 7
}
```

* `movement` = movement points.
* `movement_class` selects the terrain cost row (0..3); `4` = air.
* `counter_ground` / `counter_air`: `1` = direct fire only, `2+` = indirect
  fire with that many range rings, `0` = cannot attack.
* `ability_flags` are the original bit flags (0x20 attack-capable, 0x40
  indirect, 0x80/0x10/0x01 special).

## Terrain (per-type folders)

```
assets/terrain/00_void … 15_x14/
  terrain.json       id, name, defense, move costs, panel ids
  battle_left.png    combat backdrop (attacker side)
  battle_right.png   combat backdrop (defender side)
  world_map.png      placeholder world-map texture
```

The 24 pair-specific combat panels remain in `assets/art/combat_terrain/`
(`panel_00..23.png`), decoded with the **live combat palette** captured from the
original while a battle was running (`dump/palette_battle.bin`, a LOGPALETTE
from `0x480f20`). The boot/default palette in `dump/palette_bgr.bin` gives
wrong (gray/green) terrain colours, so don't use it for the battle backdrops.
Transparent areas are filled by tiling the terrain vertically and diffusing any
interior holes, so no black bands appear in battle.

## Art (per-unit folders)

Each unit lives in its own folder under `assets/units/unit_XX/` and has exactly
three blue sprites:

```
assets/units/unit_06/
  horizontal.png   32x32  (row 4, tile 160+type) — battle sprite
  vertical.png     32x32  (rotated 90°) — world-map icon (rotated by facing)
  unit.json        stats + sprite metadata
```

Only two sprites per unit. Only the blue palette is stored; the enemy is the
same sprite flipped and tinted green at draw time, so no colour variants are
needed. The map icon is the vertical sprite rotated by the unit's facing angle;
all other directions are flips/tints of these two.

The port loads `combat_blue.png` / `combat_green_flip.png` for the battle scene.
To inspect every unit sprite, run:

```
port/build/nectaris.exe assets assets/maps/all_units_test.json --unitshot units.png
```

Other art:

* `assets/art/combat_effects/vfx_sheet.png` — 16 combat VFX frames
  (`vfx_00..15.png`), extracted from battle-bank tiles 832–896. These drive the
  muzzle flash, projectile and explosions in the battle scene.
* `assets/art/combat_effects/effect_19x.png` — original effect frames (bank 6)
* `assets/art/tiles_ui_bank0.png` — original 16×16 tile/font bank (upright)
* `assets/art/palette.json` — the game's 256-color palette

To add a unit type: add a JSON entry and paint its sprite at the next free index
starting at 128 (copy the sheet, paste a 32×32 sprite).

## Map

The tactical map is drawn as **flat-top hexagons** (odd-column offset), matching
the original's `FUN_00403490` adjacency. Movement range, attack range and mouse
picking all use the same hex geometry.

## Combat screen

Every resolved attack switches to a battle screen modeled on the original:

* **exact original geometry**: a **512×480 playfield centred in the 640×480
  screen** (black pillars each side). Top bar = 20% height, terrain field from
  30% to 83.3%, bottom bar = 16.7%. The combat field is exactly two
  **128×128 logical (256×256 pixel) panels** side by side.
* **top bar**: both unit names and the yellow **HP boxes** (value = HP + 1)
* **middle**: the two original combat terrain close-ups, drawn at native
  aspect (terrain texture fills every pixel - no black bands).
  Each side has **8 vehicles** using the per-unit horizontal sprite
  (`assets/units/unit_XX/horizontal.png`), blue for the player and the same
  sprite flipped + tinted green for the enemy; losses remove vehicles as HP drops.
* **ride-in**: vehicles drive in from the screen edges to their staggered 2×4
  formation slots, arriving one by one at the old materialise beats.
* **ranged layout**: indirect/ranged battles open a wider black gap between the
  two terrain panels (the panels shrink slightly so the pair still fits the
  playfield); direct battles stay edge-to-edge.
* **simultaneous combat**: both sides fire at the same moment, both take
  explosions and HP damage together (no attacker-then-counter sequence); the
  two sprites are drawn facing each other (player and enemy are both flipped).
* **bottom bar**: `ATK` / `DEF` boxes for both sides and the terrain-effect
  percentages.
* **animation**: muzzle flash (`193`) + projectile (`195`), the original
  **orange/white explosion (`194`) + flames (`196`)** on both sides at once,
  then both HP boxes drop; `SPACE` / click skips.

Panel selection is the original algorithm (`FUN_00415790`): the first panel is
`table[remap[atk]*16 + remap[def] + 8]`, the second
`table[remap[def]*16 + remap[atk] + 0]` (indirect uses
`indirect[remap[p2]*2 + (first?1:0)]`).
Panels come from `assets/art/combat_terrain/panel_00..23.png` (RGBA, colour
keyed); selection uses `assets/data/combat_terrain.json`.

## Implemented

* map loading, terrain move costs, hex adjacency (odd-column offset)
* selection, movement range (Dijkstra), **direct and indirect/ranged combat**
  with support fire, terrain defense, HP/rank scaling and the original random
  damage variance
* smooth movement interpolation, hit flash, floating damage numbers
* SFX playback (`assets/audio/se01..se36.wav`)
* enemy AI (advance + attack, ranged-aware), turn cycle, win/lose
* campaign sequencing (`N`) over the exported maps
* combat self-test matrix

## TODO

* per-mission backgrounds / auto-tiled terrain
* combat cutscene tile animations (the original uses a tile-cutscene VM, which
  this port deliberately does not replay)
* MIDI music (WAVs are wired; MIDI needs a synth)
* save/load and proper campaign objectives
