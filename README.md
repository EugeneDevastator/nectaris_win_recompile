# Nectaris / Military Madness — modern C++ port

A faithful, data-driven port of the tactical core of **Nectaris (1997, Windows
95)** — known as **Military Madness** outside Japan — built with C++17 and
[raylib](https://www.raylib.com/).

The project keeps the original game's units, maps, combat maths, map art and
combat sprites, but makes everything editable as plain JSON and PNG. Cutscenes,
the intro and the story VM are intentionally out of scope.

![World map](docs/screenshots/release_map16.png)

## Features

- Original 24 unit types with StrategyWiki display names
  (`M-77 Trigger`, `S-61 Bison`, `HMB-2 Giant`, `GX-77 Charlie`, ...).
- Campaign missions 1–16 plus editor/extra maps and an all-units test map.
- Hex movement, terrain costs/defence, ZOC, support fire, surround, XP/ranks,
  transports, factories, deployment and repair.
- Combat screen with the original canvas/panel layout, support diagrams,
  palette-exact sprites, impostor direction frames, entry paths and the
  original VFX frame organization.
- World map UI with faction palettes, star ranks, factory ownership markers,
  factory capture, pan-with-units and a large readable footer.
- Original sound effects for the battle simulation, shots, misses, destruction
  and factory capture, with simultaneous multi-unit death explosions.
- Selection overlay: additive reachable highlights and black impassable
  terrain, so the legal ground is readable at a glance.
- Every asset is replaceable: `assets/data/*.json`, `assets/units/*`,
  `assets/maps/*.json`, `assets/art/*` and `assets/fonts/*`.

## Quick start

Requirements: Windows, CMake ≥ 3.20, a C++17 compiler (MinGW-w64 is what the
release uses) and Python 3 for the asset/release tools. The first configure
fetches raylib 5.5 and nlohmann/json.

```powershell
cmake -S port -B port/build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build port/build -j 8

port\build\nectaris.exe assets assets\maps\campaign_01_revolt.json
```

Or run the packaged build:

```powershell
Expand-Archive nectaris_port.zip -DestinationPath release_run
release_run\nectaris.exe release_run\assets release_run\assets\maps\campaign_01_revolt.json
```

Build, mirror the release payload and rewrite `nectaris_port.zip` with:

```powershell
python tools/make_release.py
python tools/test_port_turn.py
```

## Controls

| Input | World map |
|---|---|
| LMB | select / move / attack / open factory |
| Double-click a unit on a factory | open that factory's panel |
| RMB drag, wheel | pan the map |
| SPACE | deselect, or end turn |
| R / N / B | restart / next / previous mission |
| F1 | debug overlay |

Combat: click/SPACE advances or skips to the result; F1 toggles the wireframe
and impostor direction labels.

## Project layout

```
assets/            editable game data, art, maps, fonts, unit folders
port/              C++ sources and CMake project
port/build/        working build + local asset copy (generated)
release/           clean distribution payload (exe + assets)
tools/             asset extraction, packing, release and test scripts
dump/              reverse-engineering captures, decompilation and handoff docs
docs/              longer notes; docs/screenshots/ holds development captures
ref/               original-game reference images/captures
ui_mocks/          UI design references (not runtime assets)
game/              original game files used by the capture tools
```

## Documentation

- [`dump/PORT_HANDOFF.md`](dump/PORT_HANDOFF.md) — current handoff: build,
  impostors, palettes, VFX, world-map UI, unit folders, known gotchas.
- [`dump/COMBAT_SPRITES_NOTES.md`](dump/COMBAT_SPRITES_NOTES.md) — combat
  sprite/VFX investigation history.
- [`docs/PORTING_NOTES.md`](docs/PORTING_NOTES.md) — original porting log.
- [`docs/RE_FINDINGS.md`](docs/RE_FINDINGS.md) — reverse-engineering findings.
- [`port/README.md`](port/README.md) — detailed build/run and transport notes.

## Credits

Nectaris / Military Madness is a Hudson Soft / Turbo Technologies property.
This is a non-commercial fan preservation/porting project. Original game data
and extracted art remain the property of their respective owners.

Unit display names are sourced from the
[StrategyWiki Military Madness unit list](https://strategywiki.org/wiki/Military_Madness/Units).
