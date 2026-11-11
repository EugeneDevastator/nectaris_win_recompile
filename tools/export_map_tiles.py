"""Export the original map tile bank (16x16 tiles) to editable PNGs.

The executable expands 8x8 4-bpp tiles into a 2048-tile, 16x16 8-bpp bank at
DAT_00481500 (FUN_00412680).  `dump/tilebank.bin` is a live dump of that bank,
`dump/tilemap.bin` is the 64x32 background layout (11-bit tile id + 3-bit
palette bank) and `dump/palette_bgr.bin` is the matching VGA palette.

Rendered exactly like FUN_00411b30: each layout entry draws a 16x16 tile at
8 px steps (so tiles overlap and the last one wins).

Outputs (assets/art/map_tiles/):
  tile_XXXX_bN.png    one PNG per (tile, palette bank) used by the layout
  bank_sheet.png      the whole 2048-tile bank, 2x, labelled every 8 tiles
  used_sheet.png      only the tiles the map layout uses, 2x
  background.png      the layout composited (map + side UI strip), 520x264
  manifest.json       tile -> bank/usage table

Usage:
  python tools/export_map_tiles.py
"""
import collections
import json
import os
import struct

from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
TILEBANK = os.path.join(ROOT, 'dump', 'tilebank.bin')
TILEMAP = os.path.join(ROOT, 'dump', 'tilemap.bin')
PALETTE = os.path.join(ROOT, 'dump', 'palette_bgr.bin')
OUT = os.path.join(ROOT, 'assets', 'art', 'map_tiles')

TILE = 16
BANK_TILES = 2048
TILE_BYTES = TILE * TILE
LAYOUT_W, LAYOUT_H = 64, 32
STEP = 8


def load_palette():
    raw = open(PALETTE, 'rb').read()
    return [(raw[i * 4 + 2], raw[i * 4 + 1], raw[i * 4]) for i in range(256)]


def load_layout():
    raw = open(TILEMAP, 'rb').read()
    entries = []
    for i in range(len(raw) // 2):
        e = struct.unpack_from('<H', raw, i * 2)[0]
        entries.append((e & 0x7FF, (e >> 11) + 1))
    return entries


def tile_image(bank_data, tile, palbank, palette):
    """16x16 RGBA tile; palette index 0 is the transparency key."""
    base = tile * TILE_BYTES
    img = Image.new('RGBA', (TILE, TILE), (0, 0, 0, 0))
    px = img.load()
    for y in range(TILE):
        for x in range(TILE):
            v = bank_data[base + y * TILE + x]
            if v == 0:
                continue
            r, g, b = palette[((palbank << 4) | (v & 0x0F)) & 0xFF]
            px[x, y] = (r, g, b, 255)
    return img


def labelled_sheet(tiles, bank_for, bank_data, palette, cols, path, label_step):
    scale = 2
    cell = TILE * scale
    rows = (len(tiles) + cols - 1) // cols
    sheet = Image.new('RGBA', (cols * cell, rows * cell), (24, 24, 32, 255))
    for i, tile in enumerate(tiles):
        img = tile_image(bank_data, tile, bank_for(tile), palette)
        img = img.resize((cell, cell), Image.NEAREST)
        sheet.alpha_composite(img, ((i % cols) * cell, (i // cols) * cell))
    draw = ImageDraw.Draw(sheet)
    for i, tile in enumerate(tiles):
        if i % label_step:
            continue
        draw.text(((i % cols) * cell + 1, (i // cols) * cell + 1),
                  str(tile), fill=(255, 255, 0, 255))
    sheet.save(path)


def main():
    bank_data = open(TILEBANK, 'rb').read()
    if len(bank_data) != BANK_TILES * TILE_BYTES:
        raise SystemExit('unexpected tile bank size %d' % len(bank_data))
    palette = load_palette()
    layout = load_layout()
    os.makedirs(OUT, exist_ok=True)

    usage = collections.Counter(t for t, _ in layout)
    banks = collections.defaultdict(collections.Counter)
    for t, b in layout:
        banks[t][b] += 1
    primary = {t: banks[t].most_common(1)[0][0] for t in usage}

    exported = 0
    for tile in sorted(usage):
        for palbank in sorted(banks[tile]):
            img = tile_image(bank_data, tile, palbank, palette)
            img.save(os.path.join(OUT, 'tile_%04d_b%d.png' % (tile, palbank)))
            exported += 1

    labelled_sheet(list(range(BANK_TILES)),
                   lambda t: primary.get(t, 1), bank_data, palette,
                   64, os.path.join(OUT, 'bank_sheet.png'), 8)
    used = sorted(usage)
    labelled_sheet(used, lambda t: primary[t], bank_data, palette,
                   32, os.path.join(OUT, 'used_sheet.png'), 4)

    # Layout composite (FUN_00411b30): 16x16 tiles blitted at 8 px steps.
    W, H = LAYOUT_W * STEP + STEP, LAYOUT_H * STEP + STEP
    bg = Image.new('RGBA', (W, H), (0, 0, 0, 255))
    for row in range(LAYOUT_H):
        for col in range(LAYOUT_W):
            tile, palbank = layout[row * LAYOUT_W + col]
            bg.alpha_composite(tile_image(bank_data, tile, palbank, palette),
                               (col * STEP, row * STEP))
    bg.save(os.path.join(OUT, 'background.png'))

    manifest = {
        'source': 'Nec.exe DAT_00481500 map tile bank (FUN_00412680)',
        'tilebank': 'dump/tilebank.bin',
        'layout': 'dump/tilemap.bin (64x32 entries: id | (bank-1)<<11)',
        'palette': 'dump/palette_bgr.bin',
        'tile_size': [TILE, TILE],
        'tiles_total': BANK_TILES,
        'tiles_used': len(usage),
        'png_exported': exported,
        'background_size': [W, H],
        'used': {
            str(t): {'bank': primary[t], 'count': usage[t]}
            for t in used
        },
    }
    with open(os.path.join(OUT, 'manifest.json'), 'w', encoding='utf-8') as f:
        json.dump(manifest, f, indent=1)
    print('tiles used %d, png exported %d, background %dx%d'
          % (len(usage), exported, W, H))


if __name__ == '__main__':
    main()
