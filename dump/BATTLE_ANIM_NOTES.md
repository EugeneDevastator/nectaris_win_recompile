# Battle-scene animation findings

Investigated from `Nec.exe` (combat task `0x415580`) plus the captured frame
sets in `dump/combat_full/frames` and `dump/unit_vfx/*`.

## Scene setup

`0x415580` (combat screen entry) does:

1. clear the screen tilemap (`0x415b10`: 0x800 entries of sprite `0x3090`)
2. draw both terrain panels via `0x4156f0` → `FUN_00415790`
3. `0x415a70` / `0x415ac0` clear and expand the battle sprite bank
4. `0x415970` expands the two unit sprite sets (attacker palette 0x8b,
   defender 0x93)
5. `0x416320` copies the unit palette/state tables and schedules the animation
   callback `0x416390`

## What actually animates

* `0x416330` / `0x4163f0` schedule a **20-tick** callback.
* The callback (`0x416390`) calls `FUN_004164b0`, which recolours the 128-entry
  unit palette from tables `0x432310/0x432390/0x432410`, advancing an 8-step
  frame counter (`0x483490`).
* After 8 frames it clears `0x483491` and jumps back to the task loop.

So the "battle animation" is primarily a **palette/lighting cycle over a static
formation**, not a sprite flip-book.  The units are blitted into the panel
tilemap; the cycle makes them flash/brighten.

`0x416460` is the alternate (post-impact) driver: it sets the frame counter to
7 and runs the same 8-step palette cycle.

## Formation and entry (decoded from live memory)

The battle renderer keeps its sprite list in a fixed queue:

| address | contents |
|---|---|
| `0x4836f0` | bank 0: 256 entries x 8 bytes |
| `0x483ef0` | bank 1: same layout |

Entry layout (consumer at `0x4124d8`):

```
u16 x  -> screen X = (x + viewport_offset) * 2 - 32
u16 y  -> screen Y = y * 2 - 32
u16 palette/flags   (bit 0x40 = add 0x200 sprite base, bit 0x80 = flip)
u16 sprite          (high byte + 0x40 base, low bits select the size path)
```

Each 64x64 battle unit is four consecutive 16x16 sprite ids, so a unit group is
`(palette, sprite & ~3)` and its origin is the minimum x/y of the four chunks.

`tools/sample_battle_sprites.py` captures this queue from the running game at
~50 Hz; `tools/track_battle_sprites.py` prints the per-frame group origins.
The capture in `dump/battle_sprites_mem/frames` (306 samples) shows:

* Entry direction depends on the unit class: ground units slide in
  horizontally at a fixed row; aircraft rise/descend vertically from the panel
  edge with a diagonal drift; bikes use a shallow diagonal.  The per-group
  classification is printed by `tools/classify_entry.py`.
* Units are staggered (first group around sample 42, last around sample 60;
  formation complete near sample 130).
* The formation is drawn in **perspective**: the game uses separate scaled
  sprite variants per depth row (front rows ~1.8x the rear rows in the frames).
* After entry the unit positions are **completely static** through the attack.
  The groups that keep moving are effect/projectile sprites (palette 5/69),
  not units.
* No unit dodge or aircraft spin appears in this recording.

## Damage

* Killed slots are removed **one at a time** over several frames (the aircraft
  count in `t09_d01` drops 8 → 6 → 4 …), each with its own explosion, rather
  than the whole formation disappearing in one frame.
* The 8-step palette cycle runs again after the impact.

## Not confirmed

Scripted spins/dodges, if any scene uses them, would come from the unit-motion
VM (`0x407e50` / script table `0x42af30`), which is still undecoded.  The
memory sampler can be pointed at other scenarios (ranged, air attacker) to
check their queues directly.

## Entry paths (full curves)

`tools/extract_entry_paths.py` turns the captures into normalised paths: for
every unit group it records the X/Y sequence from first appearance to the
settled slot and stores offsets from the slot over time 0..1 in
`dump/entry_paths.json`.

`tools/build_entry_path_asset.py` reduces those to the control points shipped
in `assets/data/combat_entry_paths.json`:

```json
"ground": { "start": [72, 13], "mid": [35, 12], "end": [0, 0] },
"air":    { "start": [51, 21], "mid": [35, 11], "end": [0, 0] }
```

Offsets are original queue units (the blitter scales by 2; the panel is
256 units), so the port maps them with `offset_px = units * panelW / 128` and
mirrors X for the right-hand army.  The renderer interpolates a quadratic
curve through start → mid → slot, which reproduces the measured glide instead
of a straight line.

## Port implementation

* per-slot perspective scale (`0.62 + 0.45 * depth`)
* quadratic entry curves driven by `assets/data/combat_entry_paths.json`
  (ground horizontal, aircraft diagonal/vertical, bikes shallow diagonal)
* staggered kill removal and staggered explosions/splats
* palette-style damage flash only on the side that lost HP
