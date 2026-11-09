"""Build the correct original rotation sheet: res13 is stride-23 (unit index)."""
import os, json
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
D = os.path.join(ROOT, 'dump', 'art_resources')
OUT = os.path.join(ROOT, 'assets', 'art', 'combat_bank_original')
pal = open(os.path.join(ROOT, 'dump', 'battle_sprites', 'palette.bin'), 'rb').read()
PAL = [(pal[4 + i * 4], pal[5 + i * 4], pal[6 + i * 4]) for i in range(256)]
units = json.load(open(os.path.join(ROOT, 'assets', 'data', 'units.json'), encoding='utf-8'))['units']
names = {u['id']: u['name'] for u in units}
r13 = open(os.path.join(D, 'res_13_sprites.bin'), 'rb').read()
r14 = open(os.path.join(D, 'res_14_sprites.bin'), 'rb').read()
STRIDE = 23


def sprite(raw, t):
    if t < 0 or (t + 1) * 1024 > len(raw):
        return None
    im = Image.new('RGBA', (32, 32), (0, 0, 0, 0))
    px = im.load()
    used = False
    for y in range(32):
        for x in range(32):
            v = raw[t * 1024 + y * 32 + x]
            if v:
                used = True
                px[x, 31 - y] = PAL[((9 << 4) | (v & 15)) & 255] + (255,)
    return im if used else None


cols = 4
cell = 140
rows = 23
sheet = Image.new('RGB', (cols * cell + 180, rows * cell + 20), (20, 20, 30))
d = ImageDraw.Draw(sheet)
manifest = []
for u in range(23):
    y = 20 + u * cell
    d.text((4, y + 55), '%2d %s' % (u, names.get(u, '?')), fill=(255, 230, 100))
    ents = []
    for r in range(cols):
        t = STRIDE * r + u
        im = sprite(r13, t)
        src = 'res13'
        if im is None and r == 3 and u < len(r14) // 1024:
            im = sprite(r14, u)
            src = 'res14'
            t = u
        x = 180 + r * cell
        if im:
            big = im.resize((128, 128), Image.NEAREST)
            sheet.paste(big, (x, y), big)
            d.text((x, y + 128), '%s spr %d' % (src, t), fill=(160, 160, 100))
            ents.append({'rot': r, 'src': src, 'sprite': t})
        else:
            d.text((x + 30, y + 55), '-', fill=(90, 90, 90))
    manifest.append({'unit': u, 'name': names.get(u, '?'), 'rotations': ents})
sheet.save(os.path.join(OUT, 'combat_units_original_atlas.png'))
json.dump(manifest, open(os.path.join(OUT, 'units_manifest.json'), 'w'), indent=1)
print('wrote combat_units_original_atlas.png', sheet.size)
