# Original map tiles

Extracted from the game's own tile bank — not screenshots.

The executable expands 8x8 4-bpp tiles into a 2048-tile, 16x16 8-bpp bank at
`DAT_00481500` (`FUN_00412680`). The map screen lays those tiles out with
`FUN_00411b30`: a 64x32 table of entries (`tile id | (bank-1) << 11`) where each
16x16 tile is blitted at 8 px steps, so neighbouring tiles overlap and the last
one wins.

Sources (live dumps, not shipped):

| file | contents |
|---|---|
| `dump/tilebank.bin` | 2048 x 16x16 tile bank (256 bytes each) |
| `dump/tilemap.bin` | 64x32 layout entries, little-endian u16 |
| `dump/palette_bgr.bin` | 256-colour VGA palette (BGRX) |

Palette index 0 is the transparency key. The palette bank comes from the
layout entry: `index = ((bank << 4) | (pixel & 0x0f)) & 0xff` with
`bank = (entry >> 11) + 1`.

## Files here

* `tile_XXXX_bN.png` — one 16x16 tile with palette bank N (RGBA). 824 tiles are
  used by the layout; a handful appear with two banks and are exported twice.
* `bank_sheet.png` — the whole 2048-tile bank at 2x, labelled every 8 tiles.
* `used_sheet.png` — only the tiles used by the layout, 2x, labelled.
* `background.png` — the layout composited at 8 px steps (520x264): the map
  landscape on the left, the side-panel strip on the right.
* `manifest.json` — `tile -> {bank, count}` for every used tile.

Regenerate everything with:

```powershell
python tools/export_map_tiles.py
```

The terrain art (soil, paths, rock, forest, buildings) starts around tile 1297;
tiles 144-1295 in the used sheet are the font and UI icons that share the same
bank.
