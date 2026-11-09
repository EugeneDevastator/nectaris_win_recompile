"""Build per-unit rotation sheets from the static original banks.

res13 sprite 6 == map sprite 70 (unit 6, rotation 0) exactly, so res13 is
rotation-major: sprites [u, u+16, u+32, u+48] are unit u's 4 rotations.
res14 has 24 sprites and is exported both ways for comparison.
"""
import os, json
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
D = os.path.join(ROOT, 'dump', 'art_resources')
OUT = os.path.join(ROOT, 'assets', 'art', 'combat_bank_original')
os.makedirs(OUT, exist_ok=True)
pal = open(os.path.join(ROOT, 'dump', 'battle_sprites', 'palette.bin'), 'rb').read()
PAL = [(pal[4 + i * 4], pal[5 + i * 4], pal[6 + i * 4]) for i in range(256)]
units = json.load(open(os.path.join(ROOT, 'assets', 'data', 'units.json'), encoding='utf-8'))['units']
names = {u['id']: u['name'] for u in units}


def load(rid):
    return open(os.path.join(D, 'res_%02d_sprites.bin' % rid), 'rb').read()


def sprite(raw, t, bank=9):
    im = Image.new('RGBA', (32, 32), (0, 0, 0, 0))
    px = im.load()
    for y in range(32):
        for x in range(32):
            v = raw[t * 1024 + y * 32 + x]
            if v:
                px[x, 31 - y] = PAL[((bank << 4) | (v & 15)) & 255] + (255,)
    return im


r13 = load(13)
r14 = load(14)

# units 0..15 from res13, rotation-major
sheet = Image.new('RGB', (4 * 140 + 170, 16 * 150 + 20), (20, 20, 30))
d = ImageDraw.Draw(sheet)
for u in range(16):
    y = 20 + u * 150
    d.text((4, y + 50), '%d %s' % (u, names.get(u, '?')), fill=(255, 230, 100))
    for r in range(4):
        t = u + r * 16
        im = sprite(r13, t).resize((128, 128), Image.NEAREST)
        x = 170 + r * 140
        sheet.paste(im, (x, y), im)
        d.text((x, y + 128), 'spr %d' % t, fill=(160, 160, 100))
sheet.save(os.path.join(OUT, 'units_res13_rotations.png'))
print('wrote units_res13_rotations.png', sheet.size)

# units 16..21 from res14, unit-major (4 consecutive)
sheet = Image.new('RGB', (4 * 140 + 170, 6 * 150 + 20), (20, 20, 30))
d = ImageDraw.Draw(sheet)
for i in range(6):
    u = 16 + i
    y = 20 + i * 150
    d.text((4, y + 50), '%d %s' % (u, names.get(u, '?')), fill=(255, 230, 100))
    for r in range(4):
        t = i * 4 + r
        im = sprite(r14, t).resize((128, 128), Image.NEAREST)
        x = 170 + r * 140
        sheet.paste(im, (x, y), im)
        d.text((x, y + 128), 'spr %d' % t, fill=(160, 160, 100))
sheet.save(os.path.join(OUT, 'units_res14_rotations.png'))
print('wrote units_res14_rotations.png', sheet.size)
