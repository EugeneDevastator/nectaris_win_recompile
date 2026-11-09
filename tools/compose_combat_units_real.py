"""Compose the 64x64 combat unit sprites from a captured combat spritebank state.

Groups 233..255 place four 32x32 chunks (bank indices 128+4t .. 131+4t) into a
64x64 unit sprite.  The chunk table and offsets come from dump/sprite_groups.json.
"""
import os, json, sys
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
D = os.path.join(ROOT, 'dump', 'ai_turn2')
OUT = os.path.join(ROOT, 'assets', 'art', 'combat_units_real')
os.makedirs(OUT, exist_ok=True)
STATE = sys.argv[1] if len(sys.argv) > 1 else 'sb_156.bin'
b = open(os.path.join(D, STATE), 'rb').read()
pal = open(os.path.join(D, 'palette.bin'), 'rb').read()
PAL = [(pal[4 + i * 4], pal[5 + i * 4], pal[6 + i * 4]) for i in range(256)]
groups = json.load(open(os.path.join(ROOT, 'dump', 'sprite_groups.json')))
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


def compose(gid, bank):
    parts = groups[str(gid)]
    canvas = Image.new('RGBA', (96, 96), (0, 0, 0, 0))
    for dx, dy, spr, fl in parts:
        sdx = dx - 256 if dx > 127 else dx
        sdy = dy - 256 if dy > 127 else dy
        canvas.alpha_composite(tile(spr + 64, bank), (32 + sdx, 32 + sdy))
    bb = canvas.getbbox()
    return canvas.crop(bb) if bb else canvas


for bank, tag in ((9, 'blue'), (10, 'green'), (11, 'red'), (12, 'gray')):
    cols, cell = 8, 150
    rows = (23 + cols - 1) // cols
    sheet = Image.new('RGB', (cols * cell, rows * cell), (18, 18, 26))
    d = ImageDraw.Draw(sheet)
    for t in range(23):
        im = compose(233 + t, bank)
        name = names.get(t, '?')
        safe = ''.join(c if c.isalnum() else '_' for c in name)
        im.save(os.path.join(OUT, 'combat_unit_%02d_%s_%s.png' % (t, safe, tag)))
        x, y = (t % cols) * cell + 8, (t // cols) * cell + 30
        sheet.paste(im, (x, y), im)
        d.text((x, y - 16), '%d %s' % (t, name), fill=(255, 230, 100))
    sheet.save(os.path.join(OUT, 'all_combat_units_%s.png' % tag))
    print('wrote', tag, sheet.size)

# individual clean (transparent background) copies for the blue set
for t in range(23):
    im = compose(233 + t, 9)
    name = names.get(t, '?')
    safe = ''.join(c if c.isalnum() else '_' for c in name)
    im.save(os.path.join(OUT, 'combat_unit_%02d_%s.png' % (t, safe)))
print('done')
