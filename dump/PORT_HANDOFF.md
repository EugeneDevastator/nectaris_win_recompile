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
assets/units/unit_06_s61/frames/impostor_00.png ... impostor_NN.png
```

Folder names carry the unit name (`unit_02_ax87`, `unit_06_s61`, ...);
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
* HMB-2 Giant (type 9) is a 64x64 combat sprite drawn at **2x** normal unit
  size; its manifest entry uses `"frame_scale": 1.0` and its perspective slot
  is a full 64x64 cell.  Frames taller than the normal 32 px source are shifted
  up by half their extra height so the Giant keeps the same ground line as the
  other units and does not hang over the bottom report.
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
* controls: four rows, 36 px apart, anchored 160 px above the bottom; key
  labels right-aligned in a 78 px column, descriptions start at +94 px

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
