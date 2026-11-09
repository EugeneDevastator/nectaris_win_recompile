"""Render raw 32x32 spritebank entries grouped by unit, looking for rotations."""
import os, json
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


# 4 consecutive entries for each unit type: 64 + t*4 .. 67 + t*4
cols, cell = 4, 96
sheet = Image.new('RGB', (23 * 4 * 50, 4 * 100), (20, 20, 30))
d = ImageDraw.Draw(sheet)
for t in range(23):
    for k in range(4):
        idx = 64 + t * 4 + k
        im = tile(idx, 9).resize((48, 48), Image.NEAREST)
        x = (t * 4 + k) * 50 + 2
        y = k * 100 + 16
        sheet.paste(im, (x, y), im)
        d.text((x, y - 12), '%d' % idx, fill=(255, 255, 0))
    d.text((t * 200 + 2, 2), names.get(t, '?'), fill=(120, 255, 120))
sheet.save(os.path.join(D, 'rotations_check.png'))
print('wrote rotations_check.png', sheet.size)
