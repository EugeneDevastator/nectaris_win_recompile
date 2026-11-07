"""Reassemble 64x64 battle sprites from four 32x32 chunks.

User observation: within a sprite the chunks are ordered
    upper-right, upper-left, bottom-right, bottom-left.
Try that plus flip variants and render a sheet.
"""
import os, json, sys
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
b = open(os.path.join(ROOT, 'dump', 'spritebank_battle.bin'), 'rb').read()
pal = json.load(open(os.path.join(ROOT, 'assets', 'art', 'palette.json')))
PAL = [(p['r'], p['g'], p['b']) for p in pal]


def tile(idx, bank, flip):
    im = Image.new('RGBA', (32, 32), (0, 0, 0, 0))
    px = im.load(); off = idx * 1024
    if off + 1024 > len(b):
        return im
    for y in range(32):
        for x in range(32):
            v = b[off + y * 32 + x]
            if v:
                px[x, 31 - y if flip else y] = PAL[((bank << 4) | (v & 0xF)) & 0xFF] + (255,)
    return im


def compose(base, bank, flip, order):
    im = Image.new('RGBA', (64, 64), (0, 0, 0, 0))
    dst = {'UR': (32, 0), 'UL': (0, 0), 'BR': (32, 32), 'BL': (0, 32)}
    for k, q in enumerate(order):
        t = tile(base + k, bank, flip)
        im.paste(t, dst[q], t)
    return im


def sheet(start, count, bank, name, flip=True,
          order=('UR', 'UL', 'BR', 'BL'), cols=8, cell=128):
    rows = (count + cols - 1) // cols
    out = Image.new('RGB', (cols * cell, rows * (cell + 16)), (12, 12, 20))
    d = ImageDraw.Draw(out)
    for i in range(count):
        base = start + i * 4
        im = compose(base, bank, flip, order).resize((cell - 4, cell - 4), Image.NEAREST)
        x = (i % cols) * cell; y = (i // cols) * (cell + 16)
        out.paste(im, (x + 2, y + 16), im)
        d.text((x + 2, y), str(base), fill=(255, 255, 0))
    path = os.path.join(ROOT, 'dump', name)
    out.save(path)
    print('wrote', path, out.size)


# try the user's order with and without per-tile vertical flip
sheet(576, 41, 6, 'comp_ur_ul_br_bl_b6_flip.png', True)
sheet(576, 41, 6, 'comp_ur_ul_br_bl_b6_noflip.png', False)
sheet(576, 41, 9, 'comp_ur_ul_br_bl_b9_flip.png', True)
# other common orders for comparison
sheet(576, 16, 6, 'comp_ul_ur_bl_br_b6_flip.png', True, ('UL', 'UR', 'BL', 'BR'))
sheet(576, 16, 6, 'comp_ur_ul_bl_br_b6_flip.png', True, ('UR', 'UL', 'BL', 'BR'))
