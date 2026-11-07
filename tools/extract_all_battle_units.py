"""Extract all 64x64 battle sprites (4 chunks, order UR/UL/BR/BL) from the
combat-time sprite bank, for every unit type, in both team palettes."""
import os, json
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
b = open(os.path.join(ROOT, 'dump', 'spritebank_battle.bin'), 'rb').read()
pal = json.load(open(os.path.join(ROOT, 'assets', 'art', 'palette.json')))
PAL = [(p['r'], p['g'], p['b']) for p in pal]
OUT = os.path.join(ROOT, 'assets', 'art', 'combat_units')
os.makedirs(OUT, exist_ok=True)

# unit names by id
units = json.load(open(os.path.join(ROOT, 'assets', 'data', 'units.json'),
                       encoding='utf-8'))['units']
names = {u['id']: u['name'] for u in units}


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


def compose(base, bank):
    im = Image.new('RGBA', (64, 64), (0, 0, 0, 0))
    dst = [(32, 0), (0, 0), (32, 32), (0, 32)]
    for k in range(4):
        t = tile(base + k, bank)
        im.paste(t, dst[k], t)
    return im


def sheet(items, bank, name, cols=8, cell=128):
    rows = (len(items) + cols - 1) // cols
    out = Image.new('RGB', (cols * cell, rows * (cell + 18)), (12, 12, 20))
    d = ImageDraw.Draw(out)
    for i, (base, label) in enumerate(items):
        im = compose(base, bank).resize((cell - 4, cell - 4), Image.NEAREST)
        x = (i % cols) * cell; y = (i // cols) * (cell + 18)
        out.paste(im, (x + 2, y + 18), im)
        d.text((x + 2, y), label, fill=(255, 255, 0))
    path = os.path.join(OUT, name)
    out.save(path)
    print('wrote', path, out.size)


BASE0 = 576
type_items = [(BASE0 + t * 4, f'{t}:{names.get(t, "?")}') for t in range(24)]
extra_items = [(BASE0 + t * 4, str(BASE0 + t * 4)) for t in range(24, 42)]

for bank, tag in ((9, 'blue'), (10, 'green')):
    sheet(type_items, bank, f'unit_types_{tag}.png')
    sheet(extra_items, bank, f'extras_{tag}.png')

# individual type sprites, team palettes
for t in range(24):
    safe = ''.join(c if c.isalnum() else '_' for c in names.get(t, f't{t}'))
    for bank, tag in ((9, 'blue'), (10, 'green')):
        compose(BASE0 + t * 4, bank).save(os.path.join(OUT, f'type_{t:02d}_{safe}_{tag}.png'))
print('individual unit sprites written')

# extras (soldiers, effects, etc.)
for t in range(24, 42):
    base = BASE0 + t * 4
    for bank, tag in ((9, 'blue'), (10, 'green')):
        compose(base, bank).save(os.path.join(OUT, f'extra_{base}_{tag}.png'))
print('extra sprites written')
