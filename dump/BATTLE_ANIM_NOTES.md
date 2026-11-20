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

## Formation and entry (from captured frames)

Tracking sprite blobs per frame (`tools/track_battle.py`, left panel of a 6-unit
S-61 squad, panel-local 256x256 coordinates):

| blob centre | size | note |
|---|---|---|
| 72, 26 | 28x20 | rear row, smallest |
| 114, 34 | 28x20 | rear row |
| 170, 56 | 46x42 | middle |
| 73, 106 | 46x36 | middle |
| 112, 147 | 50x38 | front row, largest |
| 32, 153 | 50x38 | front row |

* The formation is drawn in **perspective**: front rows are ~1.8x larger than
  the rear rows (50 px vs 28 px).
* Slots sit on a diagonal: rear row shifted right, front row shifted left.
* Units enter **staggered from the outer edge** of their panel (left army from
  the left edge, right army from the right edge) and slide to their slot.
* Aircraft enter **diagonally from above the outer corner** (seen in
  `dump/unit_vfx/t09_d01`, right panel), not horizontally.

## Damage

* Killed slots are removed **one at a time** over several frames (the aircraft
  count in `t09_d01` drops 8 → 6 → 4 …), each with its own explosion, rather
  than the whole formation disappearing in one frame.
* The 8-step palette cycle runs again after the impact.

## Not confirmed

The unit-motion VM (`0x407e50` / `0x42af30` script table) is what would drive
scripted spins/dodges if a particular scene uses them.  The captured PC battles
show only edge entry + palette cycling + staggered destruction, so a full
"spin/dodge" set is not in these recordings; the VM opcode set is still
undecoded.  `tools/track_battle.py` can be pointed at new captures to check
other scenarios (ranged, air attacker, specific terrain pairs).

## Port implementation

* per-slot perspective scale (`0.62 + 0.45 * depth`)
* aircraft dive in from above the outer corner, ground units slide in
* staggered kill removal and staggered explosions/splats
* palette-style damage flash only on the side that lost HP
