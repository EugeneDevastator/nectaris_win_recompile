# Combat sprite investigation — handoff notes (end of day)

## Goal
Find the real sprites used by the combat simulator (rotations + scale/animation frames),
not the stretched map sprites.

## What is confirmed
- `res13` is the **map sprite page**: res13 sprite 6 == live map sprite index 70 (S-61) at IoU 1.00;
  res13 sprite 18 == map index 82 (GX-77) at IoU 1.00.
- `res14` is **6 units × 4 rotations** (24 valid sprites, 32x32), destination spritebank `128..191`
  (descriptor `p3=32`).
- `res23` = VFX (explosions). `res28`/`res29` = combat battlefield background tiles.
- The executable only makes three large art allocations:
  - framebuffer `GlobalAlloc(0x5d801)` -> pointer at `0x480f18`
  - tile bank `GlobalAlloc(0x80001)` -> pointer at `0x481500`
  - sprite bank `GlobalAlloc(0x100001)` -> pointer at `0x480f14`
  There is no separate large unit-animation bank in the game's own allocator.
- Sprite bank layout during combat (mission 6):
  - `128..159` / `128..191`: generated combat unit block (changes per combat state).
  - `160..183`: copy of the 23 map sprites (verified: slots 160..183 match bank sprites 64..86 exactly).
  - `192..255`: damaged/scaled variants (2 rotations × 4 scaling variants + smoke, as spotted).
  - `256..383`: more scaled variants / effect fragments.
- The generated block does **not** match any decoded resource page byte-for-byte:
  `dump/ai_turn2/sb_156.bin` block `128..191` md5 `5407c67293`
  vs res14 `21722e9201`, res40 `2bd4a9e431`. So it is produced at runtime.

## Watchdog findings (important)
- Hardware breakpoints do **not** fire from 64-bit Python into the 32-bit (WOW64) game,
  even after `Wow64SetThreadContext` (Dr registers read back correctly but never trigger,
  including an exec BP on the sprite blitter). Do not rely on Dr0..Dr3.
- A **page-protection watchdog works**: set the sprite-bank region to `PAGE_READONLY`
  with `VirtualProtectEx`; writes raise `EXCEPTION_ACCESS_VIOLATION`. Single-step over the
  write, then re-arm. Implemented in `tools/watch_guard.py`.
- First caught write:
  - fault address ~ slot 168 (`spritebank + 0x29FE1`)
  - EIP `0x412850`, inside `FUN_00412750` (sprite expander)
  - stack return addresses: `0x41c2a0` (FUN_0041c260), `0x4216c1`, `0x403a44`, `0x4216b0`,
    `0x413105`, `0x411a9a`, `0x4112dd`
  - `FUN_0041c260` expands `0x17` (23) sprites from the atlas into spritebank slot `0xA0` (160)
    -> this is the **map sprite copy**, not the combat generator.
- Debug sessions leave the game exited/crashed after detach; relaunch mission 6 for each run.

## Captured artifacts
- `dump/decoded_blue/` — all 59 resources decoded with combat-blue palette bank 9; `index.png`.
- `dump/decoded_blue_contig/` — type-0 pages as contiguous bitmaps (no tile gaps).
- `dump/ai_turn2/` — mission 6 AI-attack capture: 419 frames + 81 full `sb_*.bin` snapshots.
  - `blocks/block_9f01e50b.png` — the clean combat-unit block spotted by the user.
  - `blocks/pairs_9f.png` — 32 consecutive 64x32 chunk-pairs (clearest view of units).
  - `blocks/rot_u16_23.png` — 2 rotations x 4 scaling variants (as identified by user).
  - `blocks/block_acbb75fe.png` — AP/map units + splash parts (irrelevant).
- `assets/art/combat_pairs/` — 32 clean + 32 damaged pair sprites, 64x32, blue/green/red/gray.
- `assets/art/combat_units_real/` — chassis/rotation sheets (grouping still unconfirmed).
- `assets/art/tutorial/` — 39 tutorial pages (mostly fully typed; some duplicates).
- `dump/tutorial_pages_contact.png`, `dump/manual_pages/` (raw captured pages).
- `dump/watchdog/guard_hit_000_*` — code bytes + register/stack memory at the caught write.
- `dump/banks/` — 92 raw large memory regions (raw; user said not directly useful).

## Useful tools
- `tools/watch_guard.py` — page-protection write watchdog (current approach).
- `tools/watch_spritegen.py` — hardware-BP attempt + shared debug structs (README: HW BPs don't fire).
- `tools/advance_ai_turn2.py` — end turns with DOWN x3 + SPACE, capture frames + sprite bank.
- `tools/capture_manual_pages.py` — captures tutorial pages from the framebuffer.
- `tools/render_all_banks_blue.py` — decode every resource with palette bank 9.
- `tools/render_region_sheet.py` — render a raw dumped region as tile cells.
- `tools/export_combat_pairs.py`, `tools/export_combat_chassis.py` — current sprite exports.
- `tools/export_tutorial_pages.py` — tutorial page picker/exporter.

## Next steps (tomorrow)
1. Re-run `watch_guard.py` on a fresh mission 6 and make sure the skip-turn input actually starts a
   combat (watch state `0x483270`: map = `0x416700`, combat = `0x4217xx`). The first run only caught
   the map-sprite copy; the fake-input key state is now set correctly in the input thread.
2. Narrow the guard to `spritebank+0x20000 .. +0x28000` (slots 128..159) to ignore the map copy at 160+.
3. On a hit, log:
   - full stack walk (return addresses -> function names),
   - the caller's resource id/arguments (the caller of `FUN_0040cd80` / `FUN_00412750`),
   - dump the source atlas buffer `0x484730` before/after,
   so we can see where the original pixels come from.
4. If the generator reads the atlas, capture `0x484730` (128 KB) at the hit and compare with the
   known resource slices to identify the source resource id.
5. Alternative sources to inspect: `game/UNITEDIT.exe` (unit editor may contain clean unit art)
   and per-unit pages `res26..res58` (large coherent bitmaps; `res32`, `res45..47` are scenes).
6. Once the true 128..159 writer is identified, extract the clean source (likely 32x24/32x32 art)
   and regenerate `assets/art/combat_units_real` with correct rotation grouping.

## Day 2 findings (Nov 9) — BREAKTHROUGH
- The combat unit is a **2x2 quadrant composition** of four 32x32 spritebank slots!
  - For the attacker, slots 128..131 are the four quadrants; composing them at
    (0,0),(32,0),(0,32),(32,32) yields a clean 64x64 sprite (content ~38x30).
  - `tools/export_state_units.py` exports groups g0..g5 as 64x64 PNGs for bank 9 (blue)
    and bank 10 (green).
- Group 0 in a captured combat state is the **attacker's unit** (S-61 in the forced
  S-61 vs EF-88 run) and is written at combat start.
- Groups 1..5 in the block are **res14's 4-rotation units** (each slot is a whole 32x32
  rotation, not a quadrant) — the same art used on the map/menus.
- The generated combat art is NOT distorted: earlier renders used the wrong palette
  (the captured materialisation palette is dark) and the wrong composition. With the
  stable battle palette (dump/battle_sprites/palette.bin, bank 9/10) it is clean.
- `dump/block_changes/01_db71d13f/` holds the full generated state; `attacker_grid_b9.png`
  / `attacker_grid_b10.png` show the composed S-61 in blue/green.
- `dump/combat_chassis_contact.png` shows the exported units.

## Combat VFX tracing (Nov 10)
- Captured the original combat sprite queue (`0x4836f0`) during forced attacks
  per unit pair (`tools/capture_unit_vfx.py`, output `dump/unit_vfx/tXX_dYY/`).
- Effect sprites live in resource 23 at sprite-bank slots 320..383 (and the
  +0x200 page); the queue entry encodes x/y/flags/sprite. Effect frame = sprite
  slot - 320 (or -0x200-320 for the second page).
- **Projectiles are side-based**, not per unit type: left/player shots use frame
  `4`, right/computer shots use frame `18`. Muzzle flashes: left `14,15,16,23`,
  right `17,19,21,22`.
- **Explosions/deaths differ by defender class** (quad = four consecutive
  halves, 64x64):
  - air: quads `32,36`
  - infantry / bike: quads `32,36,44`
  - vehicle / artillery / heavy: quads `32,36,44,48,52,56`
  - small hit flash: half `25`; miss spark: half `27`.
- Quad layout is row-major (TL,TR,BL,BR), verified against live frame f027.
- Editable config: `assets/data/combat_vfx.json` (global side frames + classes +
  per-unit overrides). The port loads it in `loadData()` and uses
  `small/vfx_small_XX.png` / `big/vfx_big_XX.png` in `drawCombatScreen`.
- `tools/export_combat_vfx.py` exports the clean 32-cell / 8-big-frame split;
  the old rolled `vfx_00..15` sheets are gone (so `vfx_08` no longer exists as
  a bad copy of `vfx_quad_32`).

## Combat backgrounds + VFX palette (Nov 9)
- Backgrounds: the 24 terrain close-ups are 16x16 tilemaps (256x256) at the
  pointer table `0x431148`, drawn from the **live combat tile bank**
  (`dump/tilebank_battle.bin`) with the combat palette.  **Per-tile palette
  bank = `1 + ((entry >> 12) & 1)`** (bit 12, not bit 11).  Bank 2 turns
  palette index `0x18` into the dark pad/road grey (123,123,123) and `0x1C`
  into the lighter front-face grey (156,156,156); forcing bank 1 made 0x18
  bright (156) and punched holes when it was wrongly keyed as transparency.
  Only palette index 0 (sky/empty) is transparent.  Verified byte-exact
  against live combat frame `dump/combat_full/frames/f005.png` (panel 19 = left
  half, panel 6 = right half, diff 0).
  `tools/export_combat_backgrounds.py` writes `panel_00..23` as RGBA to
  `assets/art/combat_backgrounds/` (canonical), `combat_terrain/`,
  `combat_terrain_raw/`, `dump/panels_live/`, and the release/port mirrors.
  `combat_backgrounds/gallery.png` shows them over a checkerboard.
- VFX: resource 23 (64 x 32x32 halves = 32 x 64x32 frames, spritebank slots
  320..383). During firing the game loads a dedicated **fire ramp into palette
  bank 13** (captured live as `dump/attack2/palette_explosion_02.bin`):
  `1=white 2=yellow 3=gold 4=orange 5..7=grey`; index 0 = transparent.
  `tools/capture_explosion_palette.py` polls the palette during a forced attack
  and saves snapshots; `palette_explosion_02.bin` is the explosion frame.
  Live queues split the 64 cells into three clean groups:
  - **small** cells `0..31`: projectiles, muzzle flashes, hit and small bursts
    -> `small/vfx_small_00..31.png` (32x32).
  - **big** true 64x64 frames `32,36,40,48,52` -> `big/vfx_big_32..52.png`.
    Killed-unit explosion is `32,36,40`.
  - **packed** 64x64 sheets `44,56,60` (four separate 32x32 sprites in a 2x2
    square) -> `packed/vfx_packed_44..60.png`.  `44` is the miss burst
    (`"miss_quad": 44` in `combat_vfx.json`).
  `tools/export_combat_vfx.py` writes:
  - `vfx_atlas.png` + `vfx_atlas.json` (640x512 at 2x: big row, packed row,
    small cells 8x4 below)
  - `manifest.json` (groups + roles + death/miss sequence)
  - `effect_192..199.png` (named single effects)
  - `splat.png`, `palette_fire.bin`
  The old `vfx_00..15` sheets composed four cells as
  `[(32,0),(0,0),(32,32),(0,32)]` - a 50% U roll - so `vfx_08` did not match
  `vfx_quad_32`; those, `vfx_pair_*`, `vfx_sheet*`, `vfx_contact` and the
  invalid quads before 32 have been removed.  All mirrored to release/port
  build `assets/art/combat_effects/`.

## Combat sprites decoded (Nov 9, earlier)
- Correction: the `count` field in a command is `(cmd>>5)+1` and runs **1..8**, not 1..4.
  It is a chunk-run length (each chunk 128 bytes), so it copies that many 16x16
  chunks to consecutive sprite slots. There is **no pre-drawn scaled art** in the
  resource streams; the combat simulation scales the 64x64 frames at draw time.
- `tools/export_scaled_atlas.py` builds
  `assets/art/unit_frames/unit_frames_atlas_scaled_combat_blue.png`: one row per
  unit, columns 16/32/64/128 px, nearest-neighbour, combat-blue palette.
  Exact in-game zoom steps would need the original draw-time scaler ported.

## Release strips (Nov 9)
- `tools/export_unit_strips.py` builds one horizontal strip per unit from the
  combat-blue frames and writes it to:
  `release/assets/units/unit_XX/combat_frames.png` +
  `port/build/assets/units/unit_XX/combat_frames.png`, with `combat_frames.json`
  giving frame x/y/w/h and the palette tag.
- Combat-blue palette only, per user request.

- The live **combat palette differs from the world-map palette** in 187 entries
  (`dump/attack2/palette_stable.bin`, captured a few seconds into a combat).
  Banks: combat 9/10 = blue, 11/12 = green; map bank 10 = green, 11 = red etc.
- `tools/unit_frames.py` supports `PAL`, `BANK`, `TAG` env vars.
- Exported three full atlases (110 frames, all 23 units):
  - `unit_frames_atlas_map.png` (world-map palette, bank 9)
  - `unit_frames_atlas_combat_blue.png` (live combat palette, bank 9)
  - `unit_frames_atlas_combat_green.png` (live combat palette, bank 11)
  - individual frames: `unit_XX_frame_NN_<tag>.png`
- The count 1-3 commands are scaled variants using a bitplane scaler
  (`FUN_0040d590` expands 4bpp to 4 bitplanes; `FUN_00416bf0`/`FUN_00416d20` draw them).
  These can be reproduced by scaling the full frames; exact pre-drawn variants need a
  port of that bitplane scaler.

- Corrected LZSS verified against the game (`tools/lz_reference.py`).
- Script format + chunk unpack implemented in `tools/unit_frames.py`:
  - directory index = 26 + unit_type
  - script = [block_count] + per block [cmd_count][cmds]
  - each block = 512 bytes = four 16x16 4bpp sprites; each sprite is two 8x16 halves
    (left rows at y*4, right rows at 0x40+y*4), expanded 2x
  - cmd: count = (cmd>>5)+1; count==4 is the full-resolution unit (mode 4);
    counts 1-3 are the scaled-variant modes (different drawing path).
- Exported **110 full-resolution frames** for all 23 units:
  `assets/art/unit_frames/` (+ `unit_frames_atlas.png`).
- Remaining refinement: some units' four chunks are 4 rotations rather than 2x2
  quadrants; and the count 1-3 scaled variants use FUN_00416bf0/FUN_00416d20.

- The original Python LZSS was wrong. Reimplemented exactly from `FUN_00418bb0` /
  `FUN_00418bf0` / `FUN_00418c80` in `tools/lz_reference.py`.
- Verified byte-for-byte against the game's own decompressor called in-process
  (`tools/call_decompressor.py`): for directory index 67 the stub output and
  `lz_reference(67)` are identical (`80 00 80 00 a0 05 d8 1b ...`).
- Unit art for GX-77 comes from that stream (directory index 67).
- The stream begins with 16-bit words (`80 00 80 00 a0 05 ...`); the interpreter's
  `FUN_0040da20` writes 16-bit words into the atlas, so the data is word-based,
  not plain 4bpp bytes — the remaining step is decoding this 16-bit chunk format.
- Script format (already decoded):
  `[block_count]` then per block `[cmd_count][cmd...]`, each block = 512 bytes,
  cmd: idx = cmd&0x1f, count = (cmd>>5)+1, slot = 64 + idx*4.

- Per-unit resources decompress large: `res_044` (GX-77) = 98,304 bytes = 192 x 512-byte units.
- `res_027` ~98 KB, `res_032` ~160 KB. This is where the many frames live (not the 8-40 byte scripts).
- The 性能 (WEAPON DATA) screen opens correctly for a selected blue unit and confirms IDs/descriptions
  (GX-77 = light powered-suit infantry). It does not, however, leave extra frames in the sprite bank
  at capture time.
- Rendering the 98 KB stream as plain 32x32 4bpp chunks gives noise; the stream is a custom
  encoded format consumed by the interpreter, not raw chunks.
- Direct remote calls to the interpreter (`FUN_0041c090` + `0x41c0f0`) crash the game even when
  the process is suspended, so it needs the real task context.
- Next: port `0x41c0f0` + `FUN_004169d0` + the LZSS decompressor to Python and decode the unit
  streams offline, or invoke the interpreter through the game's task loop.

- The combat chunks are generated from a **static per-unit script table in Nec.exe**:
  - table VA `0x42fee8`, 32 pointers (one per unit type 0..22)
  - scripts live at `0x42fce8..0x42fee8`, 8–40 bytes each
  - **15 unique scripts**; several unit types share one (e.g. 4/5/6, 7/8, 18/19)
- Interpreter chain:
  - `FUN_0041c090` picks unit index from `0x483274`/`0x48327b`,
    calls `FUN_0041c200` (loads script via `FUN_0040cd50(type+0x1a)`,
    sets `DAT_00482330+0x30` = script pointer), installs callback `0x41c0f0`
  - `0x41c0f0` reads script commands; for each it calls `FUN_004169d0`,
    which expands chunks into spritebank slots starting at `64 + idx*4`
    (slot base word `0x1000` -> `>>6` -> 64).
- Exported raw scripts + hex + unit mapping:
  `assets/data/combat_unit_scripts/` (`manifest.json`, `script_XXXXXXXX.bin`).
- Directly remote-calling `FUN_0041c090` then `0x41c0f0` crashes the game
  (needs the proper task context), so next step is to invoke it through the
  game task loop or port the small script format to Python.
- Also harvested from real combats (user-confirmed correct):
  `assets/art/combat_chunks/units/units_atlas.png` (67 composed 64x64 units)
  and `assets/art/combat_chunks/library/` (267 raw chunks).

- The runtime block 128..159 is NOT all combat art: it interleaves **combat-sim chunks** with
  inserted **map sprites**. User identified the pattern for the GX-77 capture:
  `128-135 sim chunks`, `136-139 map sprites`, `140-147 sim chunks`, `148-151 map sprites`,
  `152-159 sim chunks`.
- Sim chunks compose 2x2 into a 64x64 combat unit: `(128,129 top) (130,131 bottom)` etc.
  With the stable battle palette these are clean vehicle/infantry sprites.
- Map sprites can be filtered exactly by hashing against res13/res14 sprites.
- Tools:
  - `tools/export_combat_chunks.py` — annotate one capture (map vs sim)
  - `tools/harvest_sim_chunks.py` — 267 unique raw chunks -> `assets/art/combat_chunks/library/`
  - `tools/harvest_sim_units.py` — 67 unique composed units -> `assets/art/combat_chunks/units/`
- `assets/art/combat_chunks/units/units_atlas.png` is the current combat-sim unit gallery.

- User confirmed `res13`/`res14` are MAP units, not combat-sim sprites. The clean combat
  sprites are the runtime-captured **`assets/art/combat_units_real/combat_type_XX_{blue,green}.png`**
  (64x64, composed 2x2 from slots 128..131), which the user verified in
  `dump/combat_type_contact.png`.
- `dump/combat_chassis_contact.png` (res14 2x2 groups) is NOT combat art.
- Missing types from the first enumeration (AI never attacked): 0, 9, 17, 18, 19, 20.
  Recapturing with `TMO=140`.

Force each unit type as the attacker by patching all enemy types (`PATCH_ENEMY_TYPE=T`)
in mission 6, skip turns, capture the first generated state, and export slots 128..131
as the 64x64 combat sprite for type T. `tools/export_state_units.py` + `capture_block_change.py`
already do the pieces; needs a loop over T=0..22.

- Calling the decoder for each candidate resource and hashing block 128..191:
  - `res13` → state `f236bf67` (64K) / `21722e92` (slots 128..159)
  - `res14` → state `573b0175` (64K, matches captured `sb_071`) 
  - `res21` → `fda98b13`, `res40` → `2bd4a9e4`
  - the real combat state `5407c672` (user's `block_9f01e50b`) matches **none** of them.
- New tool `tools/capture_block_change.py` polls slots 128..159 and dumps the full memory
  state (spritebank, atlas, tilebank, palette, unit arrays, frame) on every change.
  Captured 12 states in `dump/block_changes/`; transitions happen in game state `0x420e20`.
- The change pattern is: the game rewrites 4 slots at a time (one unit's rotation set):
  `128..131`, then `132..135`, ... i.e. combat units "materialise" one by one.
- Generated sprites are NOT:
  - 2x2 expansions of atlas 4bpp tiles (no match in the atlas),
  - rotations/flips of the map sprites (best IoU < 0.5).
- Unit-type patch experiment WORKS: patching all player units to type 3 and all enemies to type 6
  changes the generated block (new states `46a70fc6`, `a823cd92`, `d0bede6f`), so the generated
  art depends on the fighting unit types. `tools/patch_unit_type.py` supports `--type`, `--x`, `--y`;
  `capture_block_change.py` reads `PATCH_TYPE` / `PATCH_ENEMY_TYPE` env vars.
- Position patching a unit did NOT take (the map did not move it), so force combats by patching
  *types only* and letting the AI attack, not by relocating units.
- Debugger watchdog status: hardware BPs don't work (WOW64 + 64-bit Python). Page-protection
  write watch works, but driving the game while debugged often ends in dialog state `0x4216b0`
  and the process exits before reaching the combat state `0x420e20`, so it has not caught the
  generator yet. Prefer the non-debug `capture_block_change.py` path.

## Next concrete step
Enumerate unit types with the type patch: for each type T, patch players to T and enemies to a
fixed type, run capture_block_change, and record which 4-slot group changes. That maps slots to
unit types and gives the generated combat art for every type. Then extract/catalogue them.

