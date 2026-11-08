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

## UI font

All interface text uses **Not Jam Mono Clean 16** from
`assets/fonts/not_jam_mono_clean_16.bdf`. The CMake build enables raylib's
BDF loader (`SUPPORT_FILEFORMAT_BDF=1`); `uiText` falls back to raylib's
default font if the BDF is missing. Text is rendered at **2× the 16 px base
with integer scaling** and point filtering; the map side panel is widened to
fit the larger labels.

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

Combat screenshot (seconds = animation phase: `1.2` ride-in, `2.4` projectiles,
`2.9` explosions, `3.6` final report; last argument `0` = direct, `1` = ranged):

```powershell
port/build/nectaris.exe assets assets/maps/all_units_test.json --combatshot direct.png 2.4 0
port/build/nectaris.exe assets assets/maps/all_units_test.json --combatshot ranged.png 2.4 1
```

## Combat simulation

All battle maths lives in `port/src/combat.cpp` (declared in `combat.h`);
`model.h` holds the shared unit/mission structs and hex helpers. It is a
line-for-line port of the original:

* `FUN_0041b810` — attack/defence strengths, support fire, surround penalty
* `FUN_00420cd0` / `FUN_00421170` — variance rolls and HP loss
* `FUN_0040cb50` / `FUN_0040c770` — the original 16-bit LFSR
* `FUN_00415850` / `FUN_0040a460` — XP gain and rank tiers (`rank = xp / 4`)

Combat constants (variance bands, support/surround divisors, HP bonus, XP
awards) live in `assets/data/combat.json`; the unit/terrain tables are in
`assets/data/units.json` and `assets/data/terrain.json`. Regenerate the combat
config from the executable with:

```powershell
python tools/export_combat_config.py
```

The full pseudocode and register-level validation are in `RE_FINDINGS.md` §5.
Reproduce a specific battle state:

```powershell
port/build/nectaris.exe assets dump/_validation_mission.json --resolvetest 4 6 0 32024
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
* `units[].rank`: legacy alias for the damage tier (0..7); XP is `rank * 4`.
* `units[].xp`: optional 0..31; rank tier is `xp / 4` (matches the original).
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
* `ability_flags` are the original bit flags: `0x40` airborne, `0x20`
  transport, `0x08` indirect weapon, `0x04` move-after-attack.

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

Each unit lives in its own folder under `assets/units/unit_XX/`:

```
assets/units/unit_06/
  horizontal.png        32x32  legacy row-4 combat sprite
  vertical.png          32x32  world-map icon (rotated by facing)
  combat_alive.png      64x64  WEAPON DATA horizontal guide frame
  combat_diagonal.png   64x64  WEAPON DATA diagonal guide frame
  unit.json             stats + sprite metadata
```

The two 64×64 guide frames are extracted **raw from process memory**, not from
PrintWindow screenshots.  The authoritative source is the 8-bit framebuffer
(`DAT_00480f18`, dumped per type to `dump/guide/fb_type_XX.bin`);
`tools/extract_guide_frames_fb.py` keys out the guide-box background (palette
index 16) and writes `combat_alive.png` (horizontal) and
`combat_diagonal.png` (diagonal).

The art is not read directly from the map/battle sprite bank.  When the guide
opens, `FUN_004218c0` calls `FUN_0040cd80` to decompress per-type art into the
sprite bank (`DAT_00480f14`) and tile bank (`DAT_00481500`); the tile span and
layout depend on the art size.  For the standard 64×64 frames the bank holds
the four 32×32 quadrants `384..387` (horizontal) and `388..391` (diagonal) at
offsets `(4,0) (36,0) (4,32) (36,32)`, palette bank `0xE0`, stored bottom-up.
`tools/extract_guide_from_bank.py` reads those tiles while the guide is open
and saved the raw `dump/guide/bank_type_XX.bin` dumps; it reproduces the frames
exactly for the standard layout, while large units (e.g. HMB-2, SS-80) use a
different span, which is why the framebuffer pass remains the final source.

The sprite-list descriptors and the compressed resource directory were also
mapped (`tools/decode_spritelists.py`, `tools/decode_resources_lzss.py`), but
the guide frames themselves are decompressed into the framebuffer, not stored
as a static bank.

The WEAPON DATA stat block is written to each `unit.json` under `guide` and
aggregated in `assets/data/units_guide.json`:

```json
"guide": {
  "english_name": "M-77",
  "movement": 0,
  "defense": 80,
  "attack_ground": 0,
  "attack_air": 0,
  "range_ground": 0,
  "range_air": 0
}
```

The guide displays movement, defence, ground/air attack and ground/air range;
the displayed ranges are effective ranges (0 when that attack is 0).

Only blue sprites are stored; the enemy is the same sprite flipped and tinted
green at draw time, so no colour variants are
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
* **top bar**: both unit names and the yellow **HP boxes** (initial HP + 1).
* **middle**: the two original combat terrain close-ups, drawn at native
  aspect (terrain texture fills every pixel - no black bands). The PNGs are
  opaque (their transparent areas were pre-filled) and the two panels overlap
  by one pixel to hide the seam.
  Each side shows its **initial formation** using the per-unit horizontal
  sprite (`assets/units/unit_XX/horizontal.png`), blue for the player and the
  same sprite flipped + tinted green for the enemy.
* **ranged layout**: indirect/ranged battles open a wider black gap between the
  two terrain panels (the panels shrink slightly so the pair still fits the
  playfield); direct battles stay edge-to-edge.
* **ride-in**: vehicles roll in from the screen edges to their staggered 2×4
  formation slots at the original `0.05 + i×0.235 s` beats.
* **simultaneous fire**: both sides fire at once, using `vfx_00` cell (0,0)
  (the right-pointing missile) for every projectile and full-size `vfx_07`
  with a fade-out for every explosion. The killed vehicle slots (`finalCount ..
  initialCount-1`) are targeted exactly, so their explosions land on the right
  coordinates; all remaining shots scatter deterministically across the
  opposing panel. Shooting is 0.42 s long (1.2× the old 0.35 s); HP boxes drop
  to the surviving vehicle count at impact.
* **persistent report (no disappearing numbers)**: the bottom bar stays visible
  during and after the animation and shows the simulation inputs and resolve:
  * **attacker**: attack / own defence, support bonus, experience bonus,
    HP before → after, XP gained;
  * **simulation**: `SURROUND 0%` or `50%` with the defender's defence before
    and after the cut, terrain defence for both sides, damage to defender /
    attacker, and both variance multipliers;
  * **defender**: counter attack / defence, support, experience, HP before →
    after, XP gained.
* `SPACE` / click skips the animation to the result; a second press returns to
  the map.

Panel selection is the original algorithm (`FUN_00415790`): the first panel is
`table[remap[atk]*16 + remap[def] + 8]`, the second
`table[remap[def]*16 + remap[atk] + 0]` (indirect uses
`indirect[remap[p2]*2 + (first?1:0)]`).
Panels come from `assets/art/combat_terrain/panel_00..23.png` (RGBA, colour
keyed); selection uses `assets/data/combat_terrain.json`.

## Campaign maps and squads

The original campaign is 16 missions per chapter (`DAT_00495c52` = mission
0..15, `DAT_00495c6a` = chapter). Mission names are the 8-byte table at
`0x42a9c8` (REVOLT, ICARUS, ... NECTOR); map sizes are at `0x42a9a0/0x42a9b0`
(15x10 or 30x20). Mission 10 is **DARWIN** (index 9, 30x20).

`tools/export_campaign_missions.py` decodes each mission:

| data | source | format |
|---|---|---|
| terrain classes | `PTR_DAT_0047f750[mission]` (0x47f750) | LZSS, w*h bytes 0..13 |
| units | `PTR_DAT_0046b988[mission]` (0x46b988) | 3 bytes each: type\|owner, x, y, 0xff end |
| name / size | `0x42a9c8` / `0x42a9a0` | static |

Unit records with `x & 0xe0 != 0` are **garrison / reinforcement waves**:
the low 5 bits are the spawn cell, the high bits are the wave/group. They are
exported into the map's `garrison` array (field units stay in `units`).
Factories, HQ and bases are terrain classes 7, 8 and 11 and can hold these
garrisons, which is why late missions have many more squads than the visible
field units.

Outputs:

```
assets/maps/campaign_01_revolt.json ... campaign_16_nector.json
assets/maps/backdrops/campaign_XX_*.png # optional full-map backdrops
assets/data/campaign_missions.json      # names, sizes, field/garrison counts
dump/campaign/campaign_XX_*.png         # port-rendered full map screens
```

Each campaign JSON has `"background": "maps/backdrops/<name>.png"`.  The port
loads it if the file exists (point-filtered) and draws the semi-transparent
terrain hexes on top (`alpha = 96`), so the terrain class is still readable.
If the field is empty or the file is missing, the map falls back to opaque
terrain hexes.

The backdrops are **captured from the original game**, not rendered by the
port.  The original map screen composes the whole map from terrain tiles and
scales it to the viewport (no scrolling); `tools/orig_load_mission.py` loads a
mission via a remote call to `FUN_00405ad0`, points the mission's unit list at
an empty terminator so no units are placed, and captures the framebuffer.
The 480x320 map area is cropped to `assets/maps/backdrops/campaign_XX_*.png`;
factories/HQ/bases remain visible.  `--backdropshot` can still render a
port-style backdrop for maps without a capture:

```
nectaris.exe assets assets/maps/campaign_10_darwin.json --backdropshot map10.png
```

The font is snapped to integer multiples of its native pixel height
(`Not Jam Mono Clean 16`: 16, 32, 48, ...); glyph spacing is at least one
native pixel (1px at 1x, 2px at 2x).  The combat report falls back to 1x when
the 2x columns would not fit.

Example, mission 10 (DARWIN): 11 player + 8 enemy field units, plus 37 player
garrison/reinforcement units (7 wave groups).

The original per-mission backdrop tilemaps are also in `.data`:
`0x43aaa8[mission]` gives `(w<<4, h*11, offset)` into `0x4372a8`; each cell byte
indexes an 8x8 4bpp descriptor table at `0x43aae8` / `0x43f2a8`. The port
renders terrain colours from the decoded grid; exact original backdrop
rendering (palette/interlace) is still being matched.

## Implemented

* map loading, terrain move costs, hex adjacency (odd-column offset)
* selection, movement range (Dijkstra), **direct and indirect/ranged combat**
  with support fire, terrain defense, HP/rank scaling and the original random
  damage variance
* smooth movement interpolation, hit flash, floating damage numbers
* SFX playback (`assets/audio/se01..se36.wav`)
* enemy AI (advance + attack, ranged-aware), turn cycle, win/lose
* campaign sequencing (`N`) over the exported maps, including the 16 decoded
  campaign missions and their garrison squads
* combat self-test matrix

## TODO

* per-mission original backdrop rendering (tilemaps decoded; palette/interlace
  mapping still being matched)
* combat cutscene tile animations (the original uses a tile-cutscene VM, which
  this port deliberately does not replay)
* MIDI music (WAVs are wired; MIDI needs a synth)
* save/load and proper campaign objectives
