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

Two folders are involved:

* `port/build/` — the CMake working build. It holds object files, the built
  `nectaris.exe`, scratch screenshots and a **copy of `assets/`** (the
  `POST_BUILD` step copies it next to the exe so the folder runs standalone).
  `tools/test_port_turn.py` launches this exe.
* `release/` — the clean distribution payload (`nectaris.exe` + `assets/` +
  `map16.png`). `nectaris_port.zip` is created from this folder only, so it
  never contains build intermediates or test images.

Publish with:

```powershell
python tools/make_release.py
```

It rebuilds, copies the exe, **mirrors `assets/` into `release/` (deleting
stale files)** and rewrites `nectaris_port.zip`.  Editing assets without this
step leaves `release/` behind.

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
| LMB on own unit | select, or switch selection if one is already active |
| LMB on an enemy unit | inspect it in the side panel (info only, no orders; click again to close) |
| LMB on blue cell | move (a unit can move once per turn) |
| LMB on adjacent enemy | attack |
| LMB hover over a valid target | preview which allies (orange) and which enemy allies (red) would add support fire; the target gets a white ring, your unit stays yellow |
| LMB on a factory hex | view its stored units (any owner; arrows/LMB select, ESC close) |
| SPACE in an owned factory | deploy the selected stored unit |
| LMB on a deployment hex | place the deployed unit; blue = into a friendly transport |
| LMB own unit -> friendly transport | load it (transport must be in shift range) |
| LMB the same unit again | deselect |
| SPACE | deselect if a unit is selected, otherwise end turn (the computer turn then animates) |
| U | choose a target hex for the selected transport's cargo (animates out) |
| RMB drag / mouse wheel | pan the map (1.5 hexes of padding around the edges) |
| ESC | deselect |
| N | next map (test map first, then campaign 1-16) |
| B | previous map |
| R | restart mission |
| F1 | debug cell info |

## Transports

A transport carries exactly one unit formation. The cargo is hidden and inactive;
it is destroyed only when the whole transport formation is wiped out. Loading
uses the cargo unit's shift range (click the unit, then the transport); the unit
slides into the transport and is hidden only when the animation finishes.
Unloading
requires the cargo to have waited at least one full turn: press `U`, click a
highlighted adjacent hex, and the cargo icon animates from the transport to the
target. Only flat ground/pavement/factory/city/bridge/base hexes are valid drop
zones.

| transport | carries |
|---|---|
| NC-1 Mule | mines, infantry, motorcycles and field guns; never the HMB-2 Giant |
| C-61 Pelican | every unit except aircraft |

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

Map screenshot with an enemy unit opened for inspection (info-only side panel):

```powershell
port/build/nectaris.exe assets assets/maps/campaign_01_revolt.json --inspectshot inspect.png
```

Map screenshot with an attack preview (selected unit + hovered target + support roles):

```powershell
port/build/nectaris.exe assets assets/maps/campaign_01_revolt.json --previewshot preview.png
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
`assets/data/units.json` and `assets/data/terrain.json`. Projectile, muzzle,
hit/miss and per-unit death animations are in `assets/data/combat_vfx.json`
(frames refer to `assets/art/combat_effects/vfx_half_XX.png` and the assembled
`vfx_quad_XX.png` cells). The combat screen draws per-side support contributor
lists (icon, remaining count, contributed points), cumulative own+support
attack/defence totals, and one surrounding-hex diagram per side centred on that
side's own unit: blue/green = allied cell, red = enemy cell, orange = an allied
unit that is actually contributing, red frame + `SURR` = defender surrounded.
A strip of framed icons under the ring shows that side's participating
supporters, so allied support is always displayed on the allied side even when
those units stand next to the enemy. Regenerate
the combat config from the executable with:

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
  Movement follows the original table `0x42b248`: cost is
  `move_cost[terrain][unit movement class]`, with classes `0 tracked`,
  `1 wheeled`, `2 bike`, `3 infantry`, `4 air` (air ignores terrain and costs
  1). Cost bit `0x80` is impassable; cost `0x7f` consumes the unit's whole
  movement allowance and is only enterable with full movement left. Regenerate
  with `python tools/export_terrain_rules.py`.
* **Zone of control:** every living enemy unit projects ZOC over its six
  adjacent hexes (all unit types, including aircraft). A unit may enter an
  enemy ZOC hex but must stop there; a unit that starts its turn inside an
  enemy ZOC may move only one hex that turn. Combat surround uses the same ZOC.
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
  horizontal.png        32x32  side-view map icon (left-facing; mirrored when heading right)
  vertical.png          32x32  vertical world-map icon (unused by the port map)
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

* `assets/art/map_tiles/` — the original **map tile bank** (16x16 tiles,
  palette-keyed): `bank_sheet.png` shows all 2048 tiles with ids,
  `used_sheet.png` the 824 used by the map layout, `tile_XXXX_bN.png` one PNG
  per used tile/bank, `background.png` the layout composited at 8 px steps.
  Regenerate with `python tools/export_map_tiles.py` (source dumps:
  `dump/tilebank.bin`, `dump/tilemap.bin`, `dump/palette_bgr.bin`).
* `assets/art/combat_effects/vfx_sheet.png` — 16 combat VFX frames
  (`vfx_00..15.png`), extracted from battle-bank tiles 832–896. These drive the
  muzzle flash, projectile and explosions in the battle scene.
* `assets/art/combat_effects/effect_19x.png` - original effect frames (bank 6)
* `assets/art/combat_effects/splat.png` - shared wreck marker for destroyed
  units (the mine's dead frame, regenerated by `tools/export_splat.py`)
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
  sprite (`assets/units/unit_XX/horizontal.png`), left-facing by default and
  horizontally mirrored when the unit heads right; blue tint for the player and
  green for the enemy.
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
  to the surviving vehicle count at impact. A side that cannot return fire
  (ranged attack, artillery or a 0-attack unit) does not fire at all — the
  original sets the counter strength to 0 for indirect attacks.
* **hit / miss / destroy feedback**: only a side that actually lost HP flashes
  red. Surviving hits use the short hit flash (`vfx_half_25`); missed shots use
  a small, short-lived ground burst (`vfx_half_27`) so they never look like a
  kill. Destroyed formations play their per-class death animation, then leave a
  black **splat** (`assets/art/combat_effects/splat.png`, the mine's dead frame,
  one per unit lost in this exchange) on the ground for the result view.
* **persistent report (no disappearing numbers)**: the bottom bar stays visible
  during and after the animation and shows the simulation inputs and resolve:
  * **attacker**: attack / own defence, support bonus, experience bonus,
    HP before → after, XP gained;
  * **simulation**: `SURROUND 0%` or `50%` with the defender's defence before
    and after the cut, terrain defence for both sides, damage to defender /
    attacker, and both variance multipliers;
  * **defender**: counter attack / defence, support, experience, HP before →
    after, XP gained.
* **header groups**: team colour tab, then one group per side — unit name and
  formation box (the box is placed from the measured name width), a separator
  rule, then that side's support list (`ASUP` / `DSUP` + total, one icon and
  `xN +value` per **participating ally**, drawn in the side's own colour; the
  row reads `none` when nothing contributes, e.g. for ranged fire).  Both
  groups are mirrored around a centre divider so long names and long lists
  never collide.
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
port.  The original map screen shows a 15x10 viewport at 32px/cell and scrolls
when the cursor reaches the edge, so `tools/extract_full_maps.py`:

1. starts directly at the target mission by patching the new-game immediate
   `MOV byte ptr [0x495c52], 0` (VA `0x403d57`) and keeping `DAT_00495c52`
   patched through the intro,
2. points the mission's unit list at an empty terminator so no units load,
3. presses SPACE for action selection and walks the cursor to the viewport
   corners, capturing each 15x10 page,
4. stitches the pages at `origin * 32` into the full map.

Sizes: 15x10 -> 480x332, 15x20 -> 480x652, 30x10 -> 960x332, 30x20 -> 960x652.
The odd-q hex map draws an extra half row below the last cell row, so each
15x10 viewport is captured as 480x336 (the bottom 4 px are the UI border and are
trimmed): the map is `h*32 + 12` pixels tall instead of `h*32`. Without that the
bottom half-hex (e.g. a base on the last row of mission 5) is cut. Pages
overlap by 16 px and are stitched at `origin * 32`.
The port aligns the captured cell grid to the first hex centre and nudges it
half a step right/up, extending the destination half a cell so the odd-q
bottom/right edge is covered; the image is point-filtered.

The map structures are baked into the per-mission static backdrop tilemap
(`0x4372a8` -> descriptors `0x43aae8` -> 8x8 tiles), not a separate object
table, so the captures keep them.  From the strategy guide and the terrain
table:

* terrain 8/10: **base/HQ** — 35% defence; capturing the enemy base wins.
* terrain 11: **factory** — 0% defence; captured factories add units to your
  team and heal damaged units, and this is where the reinforcement/garrison
  waves spawn.
* the structures are drawn with owner palettes (white/blue player, yellow
  enemy, green/grey neutral), which is why there appear to be several types.

`--backdropshot` can still render a port-style backdrop for maps without a
capture:

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

## Loading missions in the original game

`orig_play_mission.py` (and the `play_mission.bat` wrapper) restarts the 1997
`game/Nec.exe`, patches the new-game initialiser immediate
`MOV byte ptr [0x495c52], 0` (VA `0x403d5d`) before the intro, and keeps the
mission index selected while the intro runs.  This uses the normal mission-start
path, so the cursor and selection state remain valid.  The script then exits and
leaves the game running so the mission can be played normally:

```powershell
python tools/orig_play_mission.py 1     # mission 1 (REVOLT)
python tools/orig_play_mission.py 16    # final mission (NECTOR)
python tools/orig_play_mission.py 3 --restart
play_mission.bat --list
```

The first argument is the 1-based campaign number.  The tool always restarts
`Nec.exe` for a clean load; switching mission in-place can leave the original
game's cursor state inconsistent.  `--no-preview` skips the framebuffer PNG
saved under `dump/orig_maps/`.

## Factories (decoded)

`tools/export_factories.py` writes `assets/data/factories.json` for all 16
campaign missions.  The static factory table is `0x47f7d0 + mission*0x10`:
up to eight `(x, y)` pairs, terminated by `0xff 0xff`.  The terrain class at
the factory cell gives its owner:

| class | owner |
|---|---|
| `0xb` (11) | player |
| `0xc` (12) | neutral |
| `0xd` (13) | enemy |

Reserve unit records (`xraw & 0xe0 != 0`) whose group (`xraw >> 5`) and `(x,y)`
match a factory are that factory's stored roster.  The 4x3 factory screen holds
up to 12 units.  Records with a group but a non-factory spawn cell are exported
separately as `reinforcements` (they are later arrival waves, not factory
contents).

Mission 10 **DARWIN** has seven factories:

| # | cell | owner | roster |
|---|---|---|---|
| 1 | (2,1) | player | FX-1, AX-87, T-79, CBX-1, NC-1 |
| 2 | (6,15) | neutral | AX-87, SG-4, MB-5, MB-4, MM107, C-61, SS-80, GX-77 |
| 3 | (13,10) | neutral | M-77, AX-87, C-61, MR-22 |
| 4 | (17,11) | neutral | EF-88, MM107, C-61, GX-77 |
| 5 | (20,4) | neutral | MR-22, C-61, SS-80, NC-1, GX-87 |
| 6 | (23,12) | enemy | EF-88, MM107, CBX-1, NC-1 |
| 7 | (28,2) | neutral | EF-88, SG-4, MR-22, C-61, M-77, MB-5, MM107 |

Factory mechanics in the original code:

* Factories do **not** produce new units; the roster is fixed by the mission.
  Captured factories only give you the units already inside.
* Moving any unit into a factory you own stores it and sets its HP to full
  (`FUN_004025f0`); it cannot be deployed again until the next turn.  This is
  the "replenish" behaviour, not a timed production queue.
* Only infantry (GX-77, GX-87, CBX-1) can capture a factory.

In the port, clicking a factory hex (with no unit selected) opens its 4x3
contents screen.  Any owner's factory can be inspected, matching the original
game.  Arrow keys or the mouse select a slot; the info panel shows the unit's
HP, movement, attack and defence.  `ESC` closes it.  For scripted previews,
`--factoryshot <png> <group>` renders a factory screen directly.

Deployment is a separate action from movement.  In a factory you own, select a
stored unit and press `SPACE`: the factory screen closes and the six adjacent
hexes are highlighted.  Green hexes are legal terrain for that unit; blue hexes
are empty friendly transports that can carry it.  Clicking a highlighted hex
spawns the unit there and removes it from the factory roster.  A unit deployed
into a transport becomes its cargo immediately, so a mine can be loaded onto a
Mule/Pelican parked next to the factory.  `ESC` cancels deployment.

## Implemented

* map loading, terrain move costs, hex adjacency (odd-column offset)
* selection, movement range (Dijkstra), **direct and indirect/ranged combat**
  with support fire, terrain defense, HP/rank scaling and the original random
  damage variance
* smooth movement interpolation, hit flash, floating damage numbers
* SFX playback (`assets/audio/se01..se36.wav`)
* enemy AI (advance + attack, ranged-aware), turn cycle, win/lose
* campaign sequencing (`N` next / `B` previous) over the test map, the 16 decoded
  campaign missions and the editor maps, including garrison squads; RMB drag
  pans the map
* transport loading/unloading (one cargo formation, cargo hidden and lost only
  with the transport; load slide-in and unload target selection with slide-out
  animation)
* factory tables and stored rosters exported to `assets/data/factories.json`
  (ownership, 12-slot capacity, repair mechanics)
* clickable factory contents screen in the port (any owner, 4x3 roster, unit
  stats, mouse/arrow selection)
* factory deployment: spawn a stored unit on an adjacent legal hex or directly
  into a friendly transport
* combat self-test matrix

## TODO

* per-mission original backdrop rendering (tilemaps decoded; palette/interlace
  mapping still being matched)
* combat cutscene tile animations (the original uses a tile-cutscene VM, which
  this port deliberately does not replay)
* MIDI music (WAVs are wired; MIDI needs a synth)
* save/load and proper campaign objectives
