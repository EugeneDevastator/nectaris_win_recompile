"""Render the real combat-sim unit sprites (spritebank index 64+type)."""
import json, os, sys
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
D = os.path.join(ROOT, 'dump', 'battle_sprites')
b = open(os.path.join(D, 'spritebank.bin'), 'rb').read()
pal = open(os.path.join(D, 'palette.bin'), 'rb').read()
PAL = [(pal[4 + i * 4], pal[5 + i * 4], pal[6 + i * 4]) for i in range(256)]
units = json.load(open(os.path.join(ROOT, 'assets', 'data', 'units.json'), encoding='utf-8'))['units']
names = {u['id']: u['name'] for u in units}


def tile(idx, bank):
    im = Image.new('RGBA', (32, 32), (0, 0, 0, 0))
    px = im.load()
    off = idx * 1024
    if off + 1024 > len(b):
        return im
    for y in range(32):
        for x in range(32):
            v = b[off + y * 32 + x]
            if v:
                px[x, 31 - y] = PAL[((bank << 4) | (v & 15)) & 255] + (255,)
    return im


for bank, tag in ((9, 'blue'), (10, 'green')):
    cols, cell = 8, 140
    rows = (23 + cols - 1) // cols
    sheet = Image.new('RGB', (cols * cell, rows * cell), (20, 20, 30))
    d = ImageDraw.Draw(sheet)
    for t in range(23):
        im = tile(64 + t, bank).resize((128, 128), Image.NEAREST)
        x = (t % cols) * cell + 6
        y = (t // cols) * cell + 18
        sheet.paste(im, (x, y), im)
        d.text((x, y - 14), '%d:%s' % (t, names.get(t, '?')), fill=(255, 255, 0))
    out = os.path.join(D, 'combat_units_%s.png' % tag)
    sheet.save(out)
    print('wrote', out, sheet.size)
