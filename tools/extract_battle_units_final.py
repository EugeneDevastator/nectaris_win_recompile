"""Extract every battle unit from the sprite-group table (gid 233..255) using
the real part offsets and the +512 bank mapping."""
import os, json
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
b = open(os.path.join(ROOT, 'dump', 'spritebank_battle.bin'), 'rb').read()
pal = json.load(open(os.path.join(ROOT, 'assets', 'art', 'palette.json')))
PAL = [(p['r'], p['g'], p['b']) for p in pal]
groups = json.load(open(os.path.join(ROOT, 'dump', 'sprite_groups.json')))
units = json.load(open(os.path.join(ROOT, 'assets', 'data', 'units.json'), encoding='utf-8'))['units']
names = {u['id']: u['name'] for u in units}
OUT = os.path.join(ROOT, 'assets', 'art', 'combat_units')
os.makedirs(OUT, exist_ok=True)

BANK_ADD = 512
CANVAS = 96
ORIGIN = 32

def tile(idx, bank, flip=True):
    im = Image.new('RGBA', (32, 32), (0, 0, 0, 0)); px = im.load(); off = idx * 1024
    if off + 1024 > len(b):
        return im
    for y in range(32):
        for x in range(32):
            v = b[off + y * 32 + x]
            if v:
                px[x, 31 - y if flip else y] = PAL[((bank << 4) | (v & 0xF)) & 0xFF] + (255,)
    return im

def compose(gid, bank):
    parts = groups[str(gid)]
    canvas = Image.new('RGBA', (CANVAS, CANVAS), (0, 0, 0, 0))
    for dx, dy, spr, fl in parts:
        sdx = dx - 256 if dx > 127 else dx
        sdy = dy - 256 if dy > 127 else dy
        t = tile(spr + BANK_ADD, bank)
        canvas.alpha_composite(t, (ORIGIN + sdx, ORIGIN + sdy))
    bbox = canvas.getbbox()
    return canvas.crop(bbox) if bbox else canvas

def sheet(items, bank, name, cols=8, cell=120):
    rows = (len(items) + cols - 1) // cols
    out = Image.new('RGB', (cols * cell, rows * (cell + 18)), (12, 12, 20))
    d = ImageDraw.Draw(out)
    for i, (gid, label) in enumerate(items):
        im = compose(gid, bank)
        im.resize((cell - 4, int(im.height * (cell - 4) / im.width)), Image.NEAREST)
        thumb = im.resize((cell - 4, cell - 4), Image.NEAREST)
        x = (i % cols) * cell; y = (i // cols) * (cell + 18)
        out.paste(thumb, (x + 2, y + 18), thumb)
        d.text((x + 2, y), label, fill=(255, 255, 0))
    path = os.path.join(OUT, name); out.save(path); print('wrote', path, out.size)

items = [(233 + t, f'{t}:{names.get(t, "?")}') for t in range(23)]
for bank, tag in ((9, 'blue'), (10, 'green')):
    sheet(items, bank, f'all_units_{tag}.png')

for t in range(23):
    gid = 233 + t
    safe = ''.join(c if c.isalnum() else '_' for c in names.get(t, f't{t}'))
    for bank, tag in ((9, 'blue'), (10, 'green')):
        compose(gid, bank).save(os.path.join(OUT, f'battle_unit_{t:02d}_{safe}_{tag}.png'))
print('individual battle unit sprites written')
