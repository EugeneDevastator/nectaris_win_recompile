# Nectaris (1997, Win95) — porting analysis

Status: installer unpacked, binary identified, original **running natively**, assets triaged.

## 1. What the installer actually contains

`inst/` is an **InstallShield 3** package:

| file | role |
|---|---|
| `SETUP.EXE` / `_SETUP.DLL` / `_INST32I.EX_` | 16/32-bit InstallShield engine |
| `SETUP.INS` / `setup.pkg` | compiled install script + file table (Shift-JIS names) |
| `_setup.lib` | compressed installer support files |
| `data.z` | **the payload** (InstallShield Z, PKWARE DCL "implode") |

Payload extracted with `putara/isdecomp` (built with mingw in `tools/isdecomp`):

```
isdecomp.exe -x -d <abs path>\game <abs path>\inst\data.z
```

Result: **96 files / 4 folders / 3.96 MB** in `game/`.

`data.z` magic: `13 5D 65 8C 3A 01 02 00`, 96 entries, 1,034,428 bytes compressed.

Contents:

```
Nec.exe                 1,332,224   game
UNITEDIT.exe              359,936   map/unit editor
SOUND\*.MID                    45   standard MIDI files
MAPDATA\エディット\*.NMD       40   campaign maps
MAPDATA\ユニット配置\*.NMD      9   unit-deployment variants
```

Note: the two `MAPDATA` folder names are Shift-JIS. isdecomp wrote the raw bytes, and on
this machine (ACP 1252) the on-disk names are exactly what the game's ANSI calls will
produce, so the game finds them. On a Japanese-locale machine you'd rename them to
`エディット` / `ユニット配置`.

## 2. The game binary (this is the key finding)

```
Nec.exe: PE32, machine i386 (0x014C), 6 sections, GUI subsystem
ImageBase 0x00400000, SizeOfImage 0x15A000
.text  0x2732B (160 KB)   .data 0x56200 (352 KB)   .rsrc 0xBA718 (764 KB)
Debug: CodeView NB10 -> C:\WINDOWS\USER\NEC2\RELEASE\Nec.pdb  (release build, no symbols shipped)
```

**It is 32-bit Win32, not 16-bit/DOS.** On modern Windows (WoW64) it launches and runs
with no shims. Verified: 640×480 window, title + attract animation render correctly
(screenshots `shot1.png`, `shot_t1.png`, `shot_t2.png`).

Imports (static): `KERNEL32, USER32, GDI32, comdlg32, ADVAPI32, WINMM`.
Loaded dynamically: `DDRAW.DLL` (`DirectDrawCreate`), `DSOUND.DLL` (`DirectSoundCreate`).

Rendering is a classic **8-bit paletted software framebuffer blitted with GDI**:
`CreatePalette`, `SelectPalette`, `RealizePalette`, `StretchDIBits`, `GetSystemPaletteEntries`.
Audio: `sndPlaySoundA` for WAV resources, MCI for MIDI.

So there is nothing to "fake" to get it running — it already runs. Hooking is only
useful as a *debugging / asset-capture oracle*.

## 3. Assets

- **Sound effects**: 36 `WAVE` resources inside `Nec.exe` (`SE01..SE36`), already complete
  RIFF files, 22050 Hz / 8-bit / mono PCM. Extracted to `game/extracted/wav/`
  (`tools/extract_wav.py`). They convert directly to OGG/WAV.
- **Music**: 45 standard `.MID` files, playable/convertible as-is.
- **Maps**: `.NMD` is simple:
  ```
  0x00  "NEC MAP V1.00   "   (16 bytes, also V1.01)
  0x10  width  (u8)
  0x11  height (u8)
  0x12..  width*height bytes, 1 tile per cell
  ```
  Verified: 16×11 + 19 = 195 bytes; 32×22 + 19 = 723 bytes. The `ユニット配置`
  variants append deployment records (e.g. 13 units × 3 bytes).
  Some larger files still need their extra header/records confirmed.
- **Graphics**: **not** in resources and **not** stored as DIBs. There are zero
  `BITMAPINFOHEADER`s in `.data`; the art is custom-packed/compressed blobs in
  `.data` (entropy ~7.8 in the image region). Locating the decoder is the main
  remaining RE task.

## 4. Review of the proposed plan

1. *Unpack installer* — done, trivial with existing tools.
2. *Hook the API to launch it* — **unnecessary**. It runs natively. Keep hooking only
   as a tool to trace/dump buffers.
3. *Decode all machine code to asm and run it from a C++ wrapper* — this is writing an
   emulator/static recompiler. Disassemblers already produce the asm; hand-writing it
   is the wrong direction. Running x86 inside a modern 64-bit process needs an actual
   CPU emulator (e.g. Unicorn), plus a PE loader and ~150 Win32 stubs. Doable here
   because the API surface is small, but it produces an emulator, not a modern port.
4. *Convert all asm to C++ step by step* — mechanically lifting 160 KB of 1997 MSVC
   code gives unmaintainable code full of raw globals and x86 idioms. Better to use
   Ghidra's decompiler and **reimplement** function by function, using the running
   original as an oracle.
5. *Convert art/sound* — sound is easy; art is the real work.

## 5. Recommended approach: oracle-driven clean reimplementation

**Phase A — asset pipeline (deterministic, do first)**
1. WAV → OGG, MIDI → OGG via a synth (fluidsynth/timidity) or ship a MIDI synth.
2. `.NMD` loader + converter to JSON/binary.
3. Crack the `.data` graphics format:
   - easiest: build a small **32-bit hook DLL** that intercepts `StretchDIBits` /
     `SetDIBitsToDevice` / `CreatePalette` and dumps source pointers + palettes while
     playing → yields decoded sprites and the palette directly;
   - or find the decoder in Ghidra (the blobs are contiguous in `.data`, so one
     function likely expands them into a framebuffer).

**Phase B — reference harness**
- Keep the original runnable as the behavioural oracle.
- Capture frames + input traces; use them as golden tests.

**Phase C — engine in C++ / raylib**
- Mirror the original architecture: a 640×480 8-bit paletted software framebuffer,
  uploaded to one raylib texture. Use raylib for window/input/audio, not for redrawing
  the art.
- Reimplement in slices: map load → renderer → input/UI → combat rules → AI → save/load.
- Decompile only what must be exact (damage/terrain tables, PRNG, AI heuristics) with
  Ghidra.

**Phase D — modern layer**
- Window scaling, controller, settings, asset repacking.

This gives a portable, maintainable C++/raylib game. Full static recompilation should
be reserved for isolated routines, not the whole binary.

## 6. Licensing

Freeware distribution does not automatically grant redistribution of extracted
assets. Ship engine + converters, and keep original art/audio/maps out of public
builds unless the license clearly allows it.

## 7. Repo state

```
inst/                     pristine installer files
game/                     extracted game (runnable)
game/extracted/wav/       36 extracted SFX
tools/isdecomp/           InstallShield 3 extractor (source + built exe)
tools/peinfo.py           PE sections/imports/resources/debug dump
tools/extract_wav.py      WAVE resource extractor
tools/*.py                strings / DIB / map / menu probes
shot*.png                 screenshots of the running original
```
