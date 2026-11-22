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
* HMB-2 (type 9) uses full 64x64 cells; its manifest entry has
  `"frame_scale": 0.5`.
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

## World-map UI (Nov 2026 iteration)

The map screen follows the original's chrome: steel-teal panel, orange header
bands, recessed navy wells, orange corner brackets on a dark map bezel.

### Palette (sampled from original UI data)

| role | RGB |
| --- | --- |
| ink / outline | 4, 20, 36 |
| panel steel | 57, 90, 90 |
| panel highlight | 90, 123, 123 |
| navy well / map bezel | 8, 57, 90 |
| steel text | 156, 156, 156 |
| pale text | 222, 222, 222 |
| orange band | 222, 90, 0 |
| peach accent | 255, 189, 123 |
| player accent | 123, 189, 255 |
| computer accent | 140, 210, 140 |

### Layout model

All values are logical screen pixels at 1468x720 (the window is created from
`mapW + 620` and `max(mapH, 720)`):

* sidebar width: `clamp(27% of screen, 360, 520)` (396 at 1468)
* map frame: 5 px inset, 8 px gutter before the sidebar
* map viewport: frame inset 5, bottom reserved for the 28 px terrain strip
* map content is centred in the viewport when `loadMission` sets
  `gMapNeedsCenter` (initial load, N/B, R) or the viewport size changes; user
  panning is preserved
* the map is clipped with `BeginScissorMode` and the cursor hit-test also
  requires the cursor to be inside the viewport
* sidebar padding 18 px, content width `sideW - 36`
* footer: 4 beveled keycaps anchored 116 px from the bottom

### Typography

`uiText` snaps to integer multiples of the 16 px BDF font:

| requested size | rendered |
| --- | --- |
| 10-11 | 16 px |
| 12-19 | 32 px |
| 20-27 | 48 px |

The UI deliberately uses only 10 (small), 15 (`32 px` values/titles) and 20
(48 px masthead).

### Components

* title plate: orange 52 px band, `NECTARIS` 48 px in ink, mission number in a
  recessed navy 50x36 plate
* section header: orange 20 px band with 1 px peach top and 1 px ink bottom
* wells: navy fill, dark top/left and light bottom/right bevel
* turn row: `PLAYER` in blue / `ENEMY` in green, garrison size in the band
* forces: player blue and computer green markers with large counts
* unit card: 190 px recessed card with map icon (green tint for enemy), name,
  owner/ready state, HP pips, rank stars, two columns of stats (ATK G/A,
  CTR G/A, DEFENCE, MOVE, RANK, HP) and context hints.  Enemy units are
  read-only.
* controls: beveled keycaps `SPACE`, `R`, `N / B`, `LMB` with descriptions
* terrain strip: bottom of the map frame; hovering a hex shows a colour
  swatch, terrain name, coordinates and `DEF +n%`
* space margins: deep navy with a deterministic starfield, two soft nebula
  blobs; the map art itself carries the terrain colours, so only a very faint
  hex grid (alpha 0.14) is drawn over it

### Key code

* `port/src/main.cpp` - `drawHexMap`, the `BeginDrawing` map block, `uiText`
* `port/src/model.h` - hex/unit definitions (odd-column offset)
* `assets/data/terrain.json` - terrain names, defence percentages, move costs
* `assets/maps/<mission>.json` - `background` points at the original map art
* `assets/art/map_tiles/background.png` - generic fallback map background

A reference render is `port/build/map_ui_final3_crop.png` (REVOLT, 15x10).

---

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
