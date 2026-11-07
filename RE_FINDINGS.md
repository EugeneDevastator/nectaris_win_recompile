# Nectaris (1997) — reverse-engineering findings

Everything below is verified against `Nec.exe` (PE32 i386, image base 0x400000)
via Ghidra 12.2 + live-process memory dumps. Reproduce with the scripts in `tools/`.

---

## 1. Runtime model

| item | value |
|---|---|
| logical resolution | **320×240** (everything is drawn at 2×) |
| framebuffer | 704×544, 8 bpp, **bottom-up DIB** at `DAT_00480f18` |
| visible region | 640×480 at source offset (32,32), blitted by `StretchDIBits` |
| palette | LOGPALETTE at `0x480f20`; BITMAPINFO palette at `0x481538` (B,G,R,0) |
| palette layout | 16 banks × 16 colors; pixel index = `(bank << 4) \| nibble` |
| frame tick | `timeGetTime`, 20 ms |

Draw calls:

* `FUN_00411b30(x, y, tileId, colorBank)` — 16×16 terrain/UI tile from `DAT_00481500`
* `FUN_00411c50(x, y, spriteId, colorBank)` — 32×32 sprite from `DAT_00480f14`
* `FUN_00411df0` — same, horizontally flipped

### Input

* Mouse is **polled** in `FUN_00412bb0`: `GetCursorPos` → `ScreenToClient` → `/2`,
  then `GetKeyState(1)` / `GetKeyState(2)` for left/right.
  Bits are OR-ed into `DAT_004834b6` (0x20 left, 0x40 right).
* Keyboard is **message-driven** in the WndProc `FUN_00410bd0`:
  `WM_KEYDOWN`/`WM_KEYUP` set/clear bits of `DAT_0042fb78`.
  Bits: Up=0x01, Down=0x02, Left=0x04, Right=0x08, Space/Ctrl=0x20,
  Shift=0x40, F1=0x10, F2=0x80. Numpad 2/4/6/8 map to the arrows.
* A key must be held for at least one 20 ms tick, otherwise a down+up in the
  same frame is invisible.

`tools/fakeinput/` contains a working virtual-input system:
`winmm.dll` proxy → hooks `GetKeyState`/`GetCursorPos`/`SetCursorPos` in the game,
driven through shared memory by `control.py`. The physical cursor never moves.

---

## 2. Art pipeline (fully decoded)

All art is stored as **4-bpp** data in a 72 KB atlas at `0x484730`
(initialised at runtime; read it from the live process).

* `FUN_00412680(id, count)` expands **8×8 4-bpp** tiles to 16×16 8-bpp
  into `DAT_00481500` (512 KB = 2048 tiles).
* `FUN_00412750(id, count)` expands **16×16 4-bpp** sprites to 32×32 8-bpp
  into `DAT_00480f14` (1 MB = 1024 sprites).
* Both read from `&DAT_00484730 + (id >> 4)*0x20` (tiles) or `(id >> 6)*0x80` (sprites);
  the low bits of `id` are the palette bank.
* Palette-bank offsets are the table `DAT_0042fb98` = `{0x00,0x10,...,0xF0}`.

### Render list

Sprites to draw each frame live at `DAT_004836f0` (two switching 0x800 buffers,
selector `DAT_004836e8`). 8-byte entries:

```
+0  u16 x        (logical screen, ×2)
+2  u16 y
+4  u16 info     bits 0-2: palette bank + 8
                 bit 6:   sprite += 0x200
                 bit 7:   horizontal flip
                 bits 8-15: sprite high bits
+6  u16 extra    bits 8-15: sprite low byte offset
```

`sprite = (info >> 8) + 0x40 + (extra & 0xff00)`

Dumped examples: `dump/spritebank_b13.png`, `dump/atlas.png`,
`dump/screen.png` (exact 640×480 reconstruction from the live framebuffer).

Unit sprites used by the status/combat UI live at **sprite index `128 + type`**
in the 32×32 bank (rows 4+ of `units_bank08/09.png`).

The map background is a 16×16-tile image blitted at **8 px steps** (overlapping),
which is why the exact screen reconstruction needs the overlap simulation in
`tools/render_bg_overlap.py`.

---

## 3. Unit state

56 unit slots, structure-of-arrays:

| global | layout |
|---|---|
| `DAT_004957a0[i]` | bit7 owner (0/0x80), bit6 flag, low5 **unit type** |
| `DAT_004957e0[i]` | low3 **HP** (0..7, starts 7), high bits **rank/XP** |
| `DAT_00495ab0[i]` | bits5-7 state, low5 **x** |
| `DAT_00495af0[i]` | bit7 ?, bit6 target/attack flag, bit5 acted, low5 **y** |
| `DAT_00495820[cell]` | occupancy: `owner \| slot`, 0xFF = empty |
| `DAT_00481a80[cell]` | terrain (stride 32) |
| `DAT_00495c02/03` | map width / height |
| `DAT_00495c63` | current player (0 or 0x80) |

A destroyed unit has all four arrays set to `0xFF`.

---

## 4. Unit tables

Export: `dump/tables.json`, `dump/units.csv` (`tools/export_tables.py`).

Per-type tables are **24 bytes each** (types 0..23), at:

| table | address |
|---|---|
| attack vs ground | `0x42b1a0` |
| attack vs air | `0x42b1b8` |
| defense | `0x42b1d0` |
| counter flag (ground) | `0x42b1e8` |
| counter flag (air) | `0x42b200` |
| **movement points** | `0x42b218` |
| movement class (4 = air) | `0x42b230` |
| ability flags | `0x42b288` |

Auxiliary tables:

* `0x42b158[8]` — HP-tier damage multiplier `100,105,110,120,130,140,160,200`
* `0x42b160[16]` — terrain defense `5,0,20,30,0,40,0,0,35,0,35,0,...`
* `0x42b248[16][4]` — terrain move cost by movement class, 0xFF = impassable
* `0x42a530[256]` — raw map byte → terrain byte
* `0x42b2a0` — 8-byte unit names (types 0..23; types 24..31 are creature names
  and do not appear in the 24-entry stat tables)

---

## 5. Combat formula (`FUN_0041b810`)

```
A = attacker slot, D = defender slot, mode = DAT_00481dbc (0 / 1 / 0x80)

tA = unit_type[A]; tD = unit_type[D]
hA = hp[A]; hD = hp[D]
mvA = movement_class[tA]; mvD = movement_class[tD]

atkVs(x, y) = (mv_y == 4) ? atk_air[t_x] : atk_ground[t_x]

canA = (mvD == 4) ? counter_air[tA] : counter_ground[tA]
canD = (mvA == 4) ? counter_air[tD] : counter_ground[tD]

if (mode == 0):
    baseA = (canA < 2) ? atkVs(A, D) : 0
    baseD = (canD == 1) ? atkVs(D, A) : 0
else:
    baseA = (canA >= 2) ? atkVs(A, D) : 0
    baseD = 0

defA = defense[tA]; defD = defense[tD]
terA = terrain_defense[terrain[A]]
terD = terrain_defense[terrain[D]]

# support fire over the 6 neighbours
supportA = sum( atkVs(N, D) * (hp[N] + 1) ) / ((hA + 1) * 2)
supportD = sum( defense[type[N]] * (hp[N] + 1) ) / ((hD + 1) * 2)

atkStrength = min(100, baseA + supportA)
defStrength = min(100, baseD + defD + terD + supportD)
if (terrain_flags[D] == 0x3f): defStrength /= 2

dmgToD = (hA + 1) * hp_mult[rank[A]] * ((100 - defStrength) * atkStrength / 100) / 100
counter = hp_mult[rank[D]] * ((100 - min(100, defA + terA)) * baseD / 100) / 100
dmgToA = (hD + 1) * counter
```

Then `FUN_00420cd0` applies a random multiplier to each side
(`FUN_00421170`, 0.2×..4.0×, integer `/10`) and computes new HP:

```
newHP = (bonus + hp * 100 - damage) / 100      // integer division
bonus = 50 if hp >= 2 else 0
if (newHP > 8) newHP = 0                        // destroyed
```

`mode` is 1 for a player-initiated attack, 0x80 for an AI/ambush attack,
0 for a plain resolution. It selects the initiative branch above.

**Correction (verified by runtime):** the third argument is actually
`hexDistance(attacker, defender) - 1`:

* `0` = adjacent / direct fire: normal exchange + counterattack.
* `>0` = indirect fire: only units whose `counter_ground/air >= 2` may attack,
  they do so only at rings `2..range`, and the defender does **not** counter.

That matches `counter_ground/air` acting as the range for indirect units
(SG-4 = 5, MR-22 = 4, SS-80 = 6, MM107 = 5 vs air).

### RNG

`FUN_0040c770` → `(rand_byte * 100) >> 8` (0..99).
`FUN_0040cb50` is a 16/24-bit LFSR mixing `DAT_00495740/41/42`.

`FUN_00421170` maps a 0..99 roll to the damage multiplier:

| roll | multiplier (÷10) |
|---|---|
| 20..24 | 15 (1.5×) |
| 60..64 | 20 (2.0×) |
| 80..82 | 2 (0.2×) |
| 98..99 | 40 (4.0×) |
| else | (150 − roll) / 10 |

---

## 6. Combat flow (state machine)

`DAT_00483270` is the per-frame state function pointer.

| state | role |
|---|---|
| `0x420c10` | confirm target, set target slot + mode, call resolver |
| `0x420cd0` | `FUN_0041b810` + RNG, compute new HP, set next state |
| `0x420e20` | apply HP via `FUN_00420ff0`, play sounds, animate HP bars |
| `0x421090` | 60-frame result delay, then end turn (`0x403c00`) |
| `0x4210f0` | return to command menu / deselect |

HP-bar/death animation is driven by `DAT_00495aa1/aa2` (counters decremented
one per frame) and `FUN_00416860` / `FUN_0041d220` (status panel draws).
`FUN_00405ca0` picks the explosion/impact SFX from the HP difference.

### Combat screen

`FUN_004156f0` / `FUN_00415790` draw the battle background as two 16×16-tile
panels at `0x180` and `0x190` in the screen tile map:

* panel pointers: `PTR_DAT_00431148` (24 entries, `.data`)
* each panel is **LZSS-compressed** (256-byte ring, 1 control bit, 8-bit
  literal / 8-bit offset + 4-bit length) decoded by `FUN_00418bb0` /
  `FUN_00418bf0` / `FUN_00418c80`
* terrain→panel tables: `DAT_00432198` (16-byte remap), `DAT_004321a8` (16×16,
  **128 bytes**), `DAT_00432228` (indirect, **16 bytes**)
* caller `0x4156f0` passes:
  * first panel  = `FUN_00415790(0x180, atkTerrain, defTerrain, 8)`
  * second panel = `FUN_00415790(0x190, defTerrain, atkTerrain, 0)`
  * so direct index = `remap[p2]*16 + remap[p3] + param4` and
    indirect index = `remap[p2]*2 + (param4 >> 3)`
* palette index `0x18` (bank 1, entry 8 = magenta) is the **colour key**;
  blank (0) pixels are also transparent.  Panels must be composited over black.
* screen geometry: the game is 640×480 with a **512×480 playfield centred**
  (64 px black pillars).  Top bar 96 px, panels 256×256 at (64,144) and
  (320,144), bottom bar 80 px.
* `FUN_00417420` positions the two unit **name** text objects at tile (0x40,0x88)
  and (0x68,0x88); `FUN_004175a0` draws the stat text (6 rows).  Note
  `FUN_0040c850` is a *formatted text blitter* (`%2d`, `%3d`, `USER`, `COMP`),
  not a sprite composer.

All 24 panels are extracted to `assets/art/combat_terrain/panel_XX.png`
(RGBA, colour-keyed) with the mapping in `assets/data/combat_terrain.json`.

### Effect sprites

Sprite indices `192..199` are the impact/explosion frames.  They use **palette
bank 6** (white / gold / orange / dark red), not bank 13:

| frame | use |
|---|---|
| 192 | white checkered disc |
| 193 | muzzle-flash burst |
| 194 | main explosion starburst (white core, orange spikes) |
| 195 | projectile / shell blob |
| 196 | flame / debris |
| 197 | square debris |
| 198-199 | UI buttons |

Extracted to `assets/art/combat_effects/effect_192..199.png`.

### Combat animation driver

`FUN_004163f0` schedules `0x416460` every 20 ticks.  The callback:

* decrements `DAT_00483492`; on underflow resets it to `DAT_0048327f`, calls
  `FUN_004164b0` (recolour the 128-entry unit palette from tables
  `0x432310/0x432390/0x432410` using the 8-step frame `DAT_00483490`), then
  decrements the frame counter and calls `FUN_00412a20`
  (`DAT_0042fb94 = 0`) to force a redraw
* after 8 frames (`DAT_00483280`) it ends by jumping back to the task loop

So the original battle "animation" is a **palette/lighting cycle over a static
formation**, not a sprite flip-book.  However, a real captured battle shows the
vehicles **appearing one by one** over ~2 s before the damage resolves; the port
reproduces that materialisation, the staggered 2×4 formation and the HP-box
behaviour from `ref/session/frame_00207..00230.png`.

The strategic/tactical map icons and the battle sprites come from the same
sprite bank (the battle candidates matched indices `164..180`); the port uses
`160 + type` for the battle side-view sprite.

### Forcing the combat screen for tooling

With the tactical map loaded, point the current task callback
(`*(DAT_00482330) + 8`) at `0x415580` after setting:

* `0x48327b` attacker slot, `0x483274` defender slot
* `0x48327c` attacker type
* `0x481db3` / `0x481db4` attacker/defender terrain
* `0x481dbc` indirect flag (0 = direct)

The game allocates a new task and renders the real combat screen.  Implemented in
`tools/force_combat.py`.  Note the map sprite icons come from a separate object
list, so writing the SoA coordinates alone does not move the icon, but the
combat resolver itself reads the SoA.

### Cutscene / unit-motion animation

Unit movement and scripted battle scenes run on the task system at `DAT_00482330`
(callback at `+8`), with a bytecode interpreter:

* task setup: `FUN_00407dd0`
* per-frame callback: `0x407e50` → dispatch to `0x407bf0`, `0x406d00`, `0x406d70`
* script table: `0x42af30` — entries `[id][len][ (opcode,arg) ... ]`
* current script id: `DAT_004822d5`; script lookup: `FUN_00407ec0`

This VM is what moves units through scripted battle sequences. Decoding its full
opcode set is the remaining task for *precisely* replaying the combat animation.

---

## 7. Tools in this repo

```
tools/ghidra_scripts/DumpDecomp.java   decompile all 705 functions
tools/dump_mem.py                      read decoded art/palette/state from live process
tools/dump_gameplay.py                 drive to map with virtual input, then dump
tools/render_art.py                    render banks/atlas/screen to PNG
tools/export_tables.py                 unit/terrain tables -> JSON + CSV
tools/decode_combat_panels.py          LZSS combat panels -> RGBA PNGs
tools/dump_combat_tables.py            terrain remap/panel tables -> JSON
tools/force_combat.py                  hijack the map task into the combat screen
tools/battle_live.py                   drive a real attack and capture frames
tools/battle_capture2.py               colour-based unit selection experiment
tools/fakeinput/control.py             virtual mouse/keyboard driver
tools/fakeinput/proxy.c                in-process input hook DLL (winmm proxy)
```

To re-run the whole asset/state extraction:

```
python tools/dump_gameplay.py
python tools/render_art.py
python tools/export_tables.py
```
