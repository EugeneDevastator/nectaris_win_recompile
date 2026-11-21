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

Each 64x64 battle unit is four consecutive 16x16 sprite ids on a 16-unit
grid.  **Several units share the same sprite base**, so grouping by
`(palette, sprite & ~3)` merges them; group the four chunks by position.

The queue is rebuilt every game frame from a live **sprite-object list** at
`0x482c30` (66 nodes x 0x18 bytes).  That list is the authoritative source of
the animation:

```
+0x0c  int32  X (integer part in the low word)
+0x10  int16  Y
+0x09  u8     current animation script/frame
+0x14  u8     layer/side tag (1 = left units, 3 = right units)
```

The queue builder (`0x412f40`) adds the sprite-frame descriptor offsets
(byte0 = dy, byte1 = dx, byte2 = sprite, byte3 = flags) to the object X/Y:
the sprite position is literally `object position + frame offset`.

Measured object -> screen mapping (640x480 frame, panels 256x256 at x=64/320,
y=96):

```
screenX = 2 * objectX - 32
screenY = 2 * objectY - 128
```

## Headless capture

`tools/capture_battle_objects.py` runs the game on a **private Windows
desktop** (nothing appears on screen) and records the object table, both
sprite-queue banks and optionally the framebuffer for every distinct game
frame.  `tools/build_entry_curves.py` turns a capture into
`assets/data/combat_entry_curves.json`.

Measured with `dump/object_runs/objraw.json`:

* All units are **parked at the panel edge** (left X=16/24, right X=300-320)
  when the battle scene opens.  They are invisible until they cross the edge.
* Each slot starts at its own time (0.0 - 1.9 s after the scene opens) and
  slides in **linearly**.  Durations vary 0.5 - 2.3 s and all slots **arrive
  together** at ~2.3 s, so later slots travel shorter paths.
* Left-side slots move (+X) and slightly vertically (their object Y changes);
  right-side slots move (-X) at a constant Y.
* After the entry the units sit still, then the fire phase adds a short
  lunge/recoil before the final positions.
* The port plays these recorded curves directly; the artificial
  `arriveT/stagger` model and the quadratic curve are gone.

## Editable perspective / sprite config

`assets/data/combat_perspective.json` is loaded at startup (no rebuild needed):

```json
{
  "speed": 1.1,                      // global entry-speed multiplier
  "left": [
    {"slot": [0.031, 0.680], "sprite": -1, "sprite_end": -1, "scale": 1.0},
    ...
  ],
  "right": [ ... ]
}
```

* `sprite` — directional frame to draw. `-1` = **automatic**: the frame is
  chosen from the unit's current movement vector (hex order E, NE, NW, W, SW,
  SE). Set a fixed index (0-5) to pin a direction.
* `sprite_end` — optional frame once the slot has settled (`-1` keeps the
  entry frame). Useful to force the original's fixed facing after arrival.
* `scale` — multiplier on the depth-based scale (`1.0` = measured default).
  Perspective is applied **during the movement**: the sprite size follows the
  unit's current depth row, so it grows/shrinks along the recorded path.
* `slot` — settled panel position; editing it moves the unit and its whole
  recorded path by the same offset.
* `speed` — multiplies the playback speed of the captured entry curves.

The frames themselves come from the extracted unit art streams in
`assets/art/unit_frames/unit_XX_frame_NN_combat_blue.png` (player) and
`..._combat_green.png` (enemy), so they are original art and are recoloured
variants of the same source. Edit/replace those PNGs to change a pose.

`tools/measure_perspective.py` regenerates the defaults from the curves (it
overwrites the file, so keep a copy of manual edits).

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
in `assets/data/combat_entry_paths.json`.  Paths are keyed by **unit type**
when sampled, with class fallbacks:

```json
"types": {
  "4":  { "start": [51, 9],  "mid": [24, 9]  },   // T-79, horizontal
  "2":  { "start": [58, 20], "mid": [37, 12] },   // AX-87, diagonal
  "18": { "start": [107, 0], "mid": [40, 0]  },   // GX-77 infantry
  "1":  { "start": [76, 0],  "mid": [24, 0]  }    // FX-1
},
"classes": { "ground": {...}, "air": {...}, "bike": {...} }
```

Sampled so far: types 1, 2, 3, 4, 5, 6, 9, 15, 18, 19, 20 (FX-1, AX-87, EF-88,
T-79, PT-6, S-61, HMB-2, AAG-4, GX-77, GX-87, CBX-1) using
`tools/sample_battle_sprites.py <atk> <def> <tag>`.  Artillery types need a
ranged setup (move the attacker's SoA cell two hexes away); the sampler's
distance argument exists but the scripted attack menu does not yet confirm a
ranged target, so they still fall back to the class curves.

## Per-frame capture (`tools/capture_one_run.py`)

The 50 Hz sampler can miss the fastest part of an entry.  `capture_one_run.py`
starts a tight background sampler *before* the attack keys and records every
distinct queue snapshot (one per game frame, ~30 fps), deduplicated, into
`dump/one_run/<tag>.json`.  It confirmed the plane entry directly:

```
p4/s112  field-X 260 -> 256 -> 252 -> 248 ... -> 193, field-Y constant 146
```

i.e. a horizontal slide at a fixed row, ~67 queue units (≈0.5 panel width),
easing out over ~1.5 s.  No vertical drop and no movement during the firing
window — the firing phase only spawns effect sprites.  `tools/overlay_queue.py`
draws these trajectories over a captured frame for visual checks.

## Aircraft animation (validated capture)

A plane-only battle (`python tools/sample_battle_sprites.py 1 1 planes`) was
captured and re-checked with `tools/overlay_queue.py` and
`tools/overlay_screen_paths.py`, which draw the queue trajectories over the
captured frame.  The lines show:

* aircraft enter **horizontally at their slot row**, like ground units; the
  earlier "vertical entry" impression came from their slots being near the top
  of the panel, not from a different path,
* after parking they are static (no hover animation),
* no firing motion was captured — the attack phase only spawns effect sprites,
  so no attack lunge/hover is applied in the port.

The port therefore uses the same type/terrain entry curves for air units and
only clips the battle scene to the panels.

## Long-run capture matrix

`tools/capture_battle_matrix.py` sweeps attacker type × defender type × terrain
(grass/forest/mountain/city/road/base) through the real attack flow.  It:

* builds 210 jobs (attackers that can hit the chosen defender domain),
* patches unit types and the terrain under both units before the attack,
* samples the sprite queue at ~50 Hz into
  `dump/battle_matrix/jobs/<aXX_dYY_tZZ>/queue.jsonl`,
* writes `dump/battle_matrix/state.json` after every job, so an interrupted or
  crashed run resumes where it stopped (`--limit`, `--retries`, `--fresh`,
  `--only` control a run).

`tools/analyze_matrix.py` converts each queue into `motion.json`:

```json
{"units": [{"pal": 4, "base0": 128, "orientations": [128],
            "samples": [{"t": 0.54, "x": 91, "y": 255, "base": 128, "orient": 0}, ...]}]}
```

`base` is the unit's four-chunk sprite base and `orient` the index of that base
in the unit's orientation list, so each captured position can be mapped back to
the corresponding impostor sprite variant.  In the PC captures so far the unit
pose is constant (`orientations` has one entry); rotation variants would show
up as extra bases.

Run it for as long as you like:

```powershell
python tools/capture_battle_matrix.py            # all remaining jobs
python tools/capture_battle_matrix.py --limit 20 # one batch
python tools/analyze_matrix.py                   # refresh motion.json
```

## Port implementation

* per-slot perspective scale (`0.62 + 0.45 * depth`)
* quadratic entry curves driven by `assets/data/combat_entry_paths.json`,
  keyed by unit type **and terrain** (with type/class fallbacks); the glide
  uses the measured ~1.3 s duration and 0.09 s stagger so the motion is visible
* staggered kill removal and staggered explosions/splats
* palette-style damage flash only on the side that lost HP

Long-run capture: `tools/capture_battle_matrix.py` (resumable, 210
type/terrain jobs) + `tools/analyze_matrix.py` (per-position motion and sprite
orientation index for impostor mapping).
