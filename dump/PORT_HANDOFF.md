# Nectaris port - handoff notes

Consolidated notes for the current port state: live-memory capture, combat
impostors, palette recolouring and the world-map interface.  Older, more
detailed investigations live in `dump/COMBAT_SPRITES_NOTES.md` and
`dump/BATTLE_ANIM_NOTES.md`.

All art is generated from the original game's data or captured live from its
memory.  Nothing is copied from screenshots; captures are only used to verify
alignment, palette indices and animation timing.

---

## Build, test, release

```powershell
cmake --build port/build -j 8
cd port/build; .\nectaris.exe assets ../../assets/maps/campaign_01_revolt.json --shot shot.png
python tools/test_port_turn.py        # player -> animated AI -> player
python tools/make_release.py          # release/assets + nectaris_port.zip
```

* `release/assets` is a mirror of `assets/`; `release/*.exe` and the locked
  `release/nectaris_old.exe` are ignored by git.
* Screenshots in this environment are high-DPI: the PNG is ~2293x1125 while the
  logical screen is 1468x720.  The actual image content sits at
  `y = 215..1115`; crop that band before judging layout.  (`GetScreenWidth()`
  is the reliable layout size, not the PNG size.)
* Hidden-window screenshot modes: `--shot`, `--combatshot`, `--unitshot`,
  `--backdropshot`, `--factoryshot`, `--inspectshot`, `--previewshot`.

---

## Combat impostors (live-memory sprites)

### Capture

`tools/capture_battle_objects.py` runs the original game on the private
`NecHeadless` Windows desktop and records the object table, queue and frames at
full game-frame resolution.  The game must be foreground on that desktop for
input.  Nothing is shown on the user's desktop.

Mapping derived from the captures:

* queue -> screen: `screenX = 2*qX + 32`, `screenY = 2*qY - 32`
* object -> screen: `screenX = 2*objX - 32`, `screenY = 2*objY - 128`

`tools/build_entry_curves.py` turns an object run into the per-slot entry
curves in `assets/data/combat_entry_curves.json`.

### Sprite sheets / atlas

Per-unit frames are editable PNGs:

```
assets/units/unit_06_s61_bison/frames/impostor_00.png ... impostor_NN.png
```

Folder names carry the code and human name (`unit_02_ax87_eagle`,
`unit_06_s61_bison`, ...), sourced from StrategyWiki's Military Madness unit
list (`M-77 Trigger`, `S-61 Bison`, ...).  `units.json` and each `unit.json`
use the full display name (`AX-87 Eagle`); `tools/update_unit_display_names.py`
performs the migration and the packers/renamers use the same code_name suffix.
Each folder also carries `horizontal_green.png` and `horizontal_used.png`
(world-map palette banks 10/12); `tools/export_map_unit_states.py` regenerates
them from map row-4 tile `160+type`.
`tools/rename_unit_folders.py` and `tools/export_unit_folders.py` keep the
number/name mapping in sync.

`tools/pack_unit_impostors.py` builds:

* `assets/art/unit_impostors_atlas.png` - runtime atlas
* `assets/data/unit_impostors.json` - rects, per-unit `dirs` / `flips` tables,
  optional `frame_scale`

The packer preserves hand-edited `dirs` / `flips` / `frame_scale` entries.

### Directions

* The entry displacement determines the compass sector; each unit has its own
  sector -> frame table (`dirs`) and mirror flags (`flips`).
* When the formation settles, both armies use the **E** frame and the right
  side is drawn mirrored, so they face each other.
* Aircraft (**FX-1, AX-87, EF-88, C-61**) share one frame pattern:
  `0 = E`, `1 = right/down 45 deg`, `2 = right/down 60 deg`,
  `3 = right/up 45 deg`, `4 = right/up 60 deg`, `5 = roll (unused)`.
  Their tables are `dirs = [0,1,2,1,0,3,4,3]` and
  `flips = [0,0,0,1,1,1,0,0]` (SW/NW/W mirror the right-facing frame;
  straight S/N use the 60-degree frames).
* HMB-2 Giant (type 9) is a 64x64 combat sprite drawn at **2x** normal unit
  size; its manifest entry uses `"frame_scale": 1.0` and its perspective slot
  is a full 64x64 cell.  Frames taller than the normal 32 px source are shifted
  up by half their extra height so the Giant keeps the same ground line as the
  other units and does not hang over the bottom report.
* Giant (HMB-2) and Atlas (SS-80) have `"side_facing": true`.  They ignore the
  entry sector and always use frame 0 with the side mirror, so they roll in
  horizontally from their own side instead of turning toward the slot.
* `NECTARIS_DEBUG_WIRE=1` (or F1 in combat) draws the wireframe, group bounds
  and per-unit labels:
  `L f0   s0` / `R f0<  s4` = side, frame, mirrored, compass sector.

### raylib flip gotcha (root cause of several art bugs)

`DrawTexturePro` flips a texture when the **source width is negative**, but it
keeps the source x and only negates the width internally.  The x must remain at
the **left edge** of the atlas rect:

```cpp
// correct: samples this frame and mirrors it in place
src = {rect.x, rect.y, -rect.w, rect.h};
// wrong: shifts the sample one frame to the right of the atlas
src = {rect.x + rect.w, rect.y, -rect.w, rect.h};
```

### Enemy palette

Frames are authored in the player's blue palette.  The computer side is
recoloured with a shader driven by `assets/data/unit_palette_lut.json`, which
holds exact per-unit blue -> green palette pairs extracted from the game art
(`tools/build_palette_lut.py`).  A luminance ramp is used as a fallback.  The
player uses shader amount 0, so its art is untouched.

---

## Combat VFX organization

The live battle queue (`dump/unit_vfx/*/q_*.json`) shows two distinct VFX
groups, which determine the clean export in `assets/art/combat_effects/`:

* **small** - cells `0..31` of resource 23.  These are complete 32x32 effects
  used directly by the queue: projectiles (frames 4 / 18), muzzle flashes
  (14-17, 19, 21-23), hit (25) and small bursts.
  Export: `small/vfx_small_00..31.png`.
* **big** - true double-size 64x64 frames: `32, 36, 40, 48, 52`, composed from
  four consecutive cells in row-major TL, TR, BL, BR order.
  Export: `big/vfx_big_32..52.png`.
  The killed-unit explosion sequence is `32, 36, 40` (set in
  `assets/data/combat_vfx.json`).  The blast is drawn with a **bottom-centre
  pivot** on the unit's ground line (`to.y + 0.5 * us`), so it rises upward
  from the ground instead of being centred over the dead sprite.
* **packed** - `44, 56, 60` are 64x64 groups that are actually four separate
  32x32 sprites packed in a 2x2 square.  `44` is the **miss** sheet; `56` and
  `60` are particle/shard sheets.  Export: `packed/vfx_packed_44..60.png`.
  The miss animates one cell at a time in the original order **BR, TR, TL, BL**
  (frames `47,45,44,46`), 0.10 s per frame, selected by
  `"miss_sheet": 44` + `"miss": [47,45,44,46]` in `combat_vfx.json`.
  **All non-killed slots** use this miss sheet; the big `32,36,40` explosion is
  reserved for destroyed HP sprites, so a normal exchange no longer shows the
  same large explosion on every survivor.

The old `vfx_00..15` sheets composed each group as
`[(32,0), (0,0), (32,32), (0,32)]`, i.e. rolled 50% along U; `vfx_08` was the
same cells as `vfx_quad_32` with that roll.  Those exports, the intermediate
`vfx_pair_*`, the legacy sheets and contact sheets have been pruned.

`vfx_atlas.png` is the single reference atlas (640x512 at 2x): one row of the
five big frames, one row of the three packed sheets, then the 32 small 32x32
cells in 8x4.  `vfx_atlas.json` holds the rects; `manifest.json` lists the
groups and roles.  `tools/export_combat_vfx.py` regenerates all three mirrors
from resource 23 and the captured fire palette
(`dump/attack2/palette_explosion_02.bin`, bank 13).

Runtime paths: `art/combat_effects/small/vfx_small_%02d.png`,
`big/vfx_big_%02d.png` and `packed/vfx_packed_%02d.png` (start-frame name).
The named `effect_192..199.png` files and `splat.png` stay at the root.

### Combat exchange timing

* `fireT = 2.55 s`, travel `fireDur = 0.42 s`.
* A side's shots are spread over **0.5 s**: shot `i` leaves at
  `fireT + 0.5*i/(n-1)` and arrives `fireDur` later.  The last arrival is
  `fireT + 0.5 + fireDur = 3.47 s`.
* Miss/scatter shots are scheduled first; the last `killed` shots all leave
  together at the end of the window.  Kill projectiles therefore arrive after
  all misses, and every destroyed slot's explosion starts **simultaneously**
  on that final arrival.
* The big death frames step every **0.18 s** (was 0.09) and fade over 0.55 s,
  so the destruction blast lasts about twice as long.  The first frame is
  additionally held for **0.45 s** (`death_first_hold_seconds` in
  `combat_vfx.json`, five original 0.09 s frames) before it advances.
* Roll-in playback is 1.2x (`combat_perspective.json` `speed`); `speed` now
  correctly scales playback time instead of canceling itself out.
* Parked aircraft bob slightly up/down (`sin(t*9 + slot*1.7) * us*0.05`).
* The furthest formation row uses 0.85x its old scale (0.612 vs 0.72 at depth
  0); the nearest row is unchanged at 1.52.
* Each impact effect starts at that shot's arrival.  The killed sprite is
  hidden and its wreck splat appears at the same arrival; the death explosion
  starts then too.
* Death explosion size is `us * 2.40 * 0.80 = us * 1.92` (0.8x the previous
  size), with the bottom-centre pivot on the unit ground line.
* The first click skips to `4.30 s`, after the last explosion has played.

### Combat audio

Sounds are the original resource WAVs in `assets/audio/`, mapped by role in
`assets/data/audio.json` (no rebuild needed).  `docs/audio/soundboard.html`
plays all 36 and exports a corrected mapping.

| role | file | trigger |
| --- | --- | --- |
| battle entry (roll-in) | `se33` then `se34` | enter/roll-in; same pair for every matchup |
| direct volley | `se28.wav` | first frame after `fireT` |
| heavy / indirect volley | `se29.wav` | same, when the attacker is ranged |
| big destruction | `se14.wav` | the common kill arrival, once per side |
| miss | `se08.wav` | first miss impact arrival (provisional) |
| factory capture | `se13.wav` | `captureFactoryAt()` (provisional) |
| unit select | `se01.wav` | map/UI confirm |
| cursor move | `se02.wav` | map cursor |

The original queues an SE number in `DAT_0042fb90`; `FUN_00412890` sets it and
the frame loop calls `FUN_0040b260(id-1, 0)`.  Confirmed ids: `0x1c` (SE28) and
`0x1d` (SE29) at `0x41433f` / `0x413eaf` (combat formation firing), `0x0e`
(SE14) in the destruction frame animation, and `0x22` (SE34) at `0x413531`
just before the nine-step battle-screen roll-in.

The roll-in sequence is fixed: `se33` at `0x4134cf`, then `se34` at `0x413531`,
then combat music (`0x36`) — there is no unit-type branch.  Infantry-vs-infantry
and air-vs-air therefore use the same pair; the only other combat movement
blips are the generic sprite-VM sounds (`se21`/`se25`/`se26`/`se27`).

Set `NECTARIS_SFX_LOG=1` to print every triggered role and its resolved file.
Sound playback is guarded by `IsSoundValid`, so headless/no-audio runs work.

---

## World-map UI (mock-based, Nov 2026)

Source design: `ui_mocks/nectarisUntitled.png`.  The mock is a layout/style
reference only; it is never used as runtime art.  The map viewport keeps the
original game art untouched.

The screen is a flat navy field divided by thin slate-blue rules.  There are no
orange header bands or recessed wells in this iteration.

### Palette (sampled from the mock)

| role | RGB |
| --- | --- |
| background | 19, 33, 45 |
| frame / rules | 75, 100, 122 |
| ink / icon wells | 8, 16, 26 |
| body text | 185, 193, 202 |
| labels | 160, 171, 182 |
| values / white | 245, 248, 252 |
| masthead cream | 245, 221, 158 |
| name / key gold | 255, 232, 140 |
| active cyan | 64, 255, 255 |
| enemy green | 110, 246, 92 |
| ready green | 90, 210, 110 |
| player blue | 61, 172, 252 |
| dim hint | 130, 143, 156 |

### Layout model

All values are logical screen pixels at 1468x720 (the window is created from
`mapW + 620` and `max(mapH, 720)`):

* sidebar width: `clamp(33% of screen, 420, 600)` (484 at 1468; widened so the
  doubled unit-property text and both stat columns fit)
* outer window rule: 2 px, inset 1 px
* map-area rule: 2 px, inset 6 px; 4 px between the rule and the viewport
* map viewport: 34 px terrain strip reserved at the bottom
* map content is centred in the viewport when `loadMission` sets
  `gMapNeedsCenter` (initial load, N/B, R) or the viewport size changes; user
  panning is preserved
* the map is clipped with `BeginScissorMode` and the cursor hit-test also
  requires the cursor to be inside the viewport
* sidebar padding 26 px, content width `sideW - 52`
* controls: four rows, 40 px apart, anchored 172 px above the bottom; key
  labels right-aligned in a 100 px column, 32 px descriptions start at +116 px

### Typography

`uiText` snaps to integer multiples of the 16 px BDF font:

| requested size | rendered |
| --- | --- |
| 10-11 | 16 px |
| 12-19 | 32 px |
| 20-27 | 48 px |

The UI deliberately uses only 10 (small), 15 (`32 px` values/titles) and 20
(48 px masthead).

The world-map text uses the native regular weight (no synthetic bold pass).
Unit properties (labels *and* values) are set at **32 px**, double the old
16 px block; ATK/CTR use compact `a/b` values so each column stays aligned.

### Components

* masthead: `NECTARIS` 48 px cream, centred; 2 px slate rule below
* mission line: uppercase mission name (underscores become spaces) in 32 px
  body text with `NN / MM` right-aligned.  When a long test-map name would
  collide with the number, the number drops to 16 px instead of overlapping.
* turn state: `PLAYER ACTIVE` cyan / `ENEMY ACTIVE` green, 32 px
* forces: 14 px blue/green squares, 32 px labels, 32 px white counts
* unit card:
  * 66 px ink icon well with a 2 px frame (enemy icons green-tinted)
  * name in 32 px gold, `ALLIED/ENEMY / READY/ACTED` in 16 px green or dim
  * HP pips plus `n / 7`; player pips blue, computer pips green
  * flat 2-column stat grid at **32 px**: ATK, CTR / DEF, MOVE / RANK, XP /
    TERRAIN, TYPE; labels and values share the same size and baseline.  On
    narrow deploy maps (`innerW < 420`) the long labels shorten to `TERR` and
    `GND` so the columns cannot collide.
  * two context hint lines (orders, cargo/unload, transport, read-only enemy)
  * no wells or bands; rules separate groups
* controls: flat gold key labels (`SPACE`, `R`, `N / B`, `LMB`) with 16 px
  descriptions
* terrain strip: 2 px rule and 16 px body text; on hover it shows a colour
  swatch, terrain name, coordinates and `DEF +n%`
* space margins: navy with a deterministic starfield and two soft nebula
  washes; the map itself carries the terrain colours, so only a very faint hex
  grid (alpha 0.14) is drawn over it

### World-map interactions / fixes

* **Panning** applies the same-frame pan delta to every unit's `px/py` and to
  in-flight animation endpoints, so the army moves with the map instead of
  trailing it while the map is dragged.
* **Footer** uses 32 px text in both places: the bottom terrain strip (48 px
  tall) and the four controls rows (40 px apart), twice the old 16 px text.
* **Factory capture**: infantry only (`GX-77 Charlie`, `GX-87 Kilroy`,
  `CBX-1 Panther`).  Ending movement on an undefended factory owned by the
  other side changes ownership and terrain colour; the stored roster remains
  available to the new owner.  Verified on RAMSEY group 3 at `2,9`: GX-77 at
  `3,11` reaches it at cost 3 with 3 movement.
* **Factory team indicator** is a quarter-cell coloured square at the
  top-right of the factory hex: blue player, red computer, grey neutral.
* **Double-click** a unit standing on a factory hex to open the factory panel;
  a single click keeps the normal unit select/inspect behavior.
* **Selection highlight**: reachable cells are drawn with additive cyan
  (bright terrain brightens), while terrain the selected unit cannot enter is
  covered with a 45% dark-gray veil (40,42,48); not pure black.  Air units skip
  the impassable pass.
* **Map unit palettes** use exact world-map banks, not a tint: bank 9 blue
  (`horizontal.png`), bank 10 green (`horizontal_green.png`), bank 12 used-up
  grey (`horizontal_used.png`), all generated from map row-4 tile `160+type`.
  Flipped variants are built at load just like the blue icon.

### Key code

* `port/src/main.cpp` - `drawHexMap`, the `BeginDrawing` map block, `uiText`
* `port/src/model.h` - hex/unit definitions (odd-column offset)
* `assets/data/terrain.json` - terrain names, defence percentages, move costs
* `assets/maps/<mission>.json` - `background` points at the original map art
* `assets/art/map_tiles/background.png` - generic fallback map background

Reference renders: `port/build/nobold_crop.png` (REVOLT, empty card) and
`port/build/nobold_card_crop.png` (all-units map, enemy card with the 32 px
property grid).

## Debug / test switches

| switch | effect |
| --- | --- |
| F1 | combat wireframe + impostor direction labels |
| `NECTARIS_DEBUG_WIRE=1` | force F1 overlay on for screenshots |
| `--hextest` | verify hex centre -> pixel -> hex round-trip |
| `--inspectshot` | open the first enemy unit in the world-map card |
| `--previewshot` | select an adjacent target and show support preview |
| `python tools/test_port_turn.py` | end-to-end turn/turn-handover test |

---

## Known gotchas

* Never use a screenshot or capture PNG as runtime art; regenerate from data.
* The raylib flip bug above also applies to debug probes: the x stays left.
* The window is high-DPI; screen coordinates and PNG pixels are different.
* `tools/test_port_turn.py` uses `PrintWindow`, which cannot read the OpenGL
  surface on this setup and often saves a white window image; trust its
  stdout/PASS result, and use `--shot` (raylib `TakeScreenshot`) for pixels.
* `loadMission` resets pan to zero and sets `gMapNeedsCenter`; the map UI
  consumes that flag, so restarting or switching missions always re-centres.
* `release/nectaris_old.exe` can stay locked; it is excluded from the zip.
