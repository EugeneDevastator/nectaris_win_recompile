"""Final combat-unit extraction: labelled sheets for all vehicle types (gid
233..255) + the infantry/person sprite with the orange gun remap."""
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

def tile(idx, bank, flip=True, remap=None):
    im = Image.new('RGBA', (32, 32), (0, 0, 0, 0)); px = im.load(); off = idx * 1024
    if off + 1024 > len(b):
        return im
    for y in range(32):
        for x in range(32):
            v = b[off + y * 32 + x]
            if not v:
                continue
            n = v & 0xF
            if remap and n in remap:
                col = remap[n]
            else:
                col = PAL[((bank << 4) | n) & 0xFF]
            px[x, 31 - y if flip else y] = col + (255,)
    return im

def compose_group(gid, bank, remap=None):
    sprites = [p[2] + 512 for p in groups[str(gid)]]
    im = Image.new('RGBA', (64, 64), (0, 0, 0, 0))
    for k, spr in enumerate(sprites[:4]):
        dst = [(32, 0), (0, 0), (32, 32), (0, 32)][k]   # UR UL BR BL
        t = tile(spr, bank, True, remap)
        im.paste(t, dst, t)
    return im

def sheet(items, bank, name, remap=None):
    cols = 8; cell = 120
    rows = (len(items) + cols - 1) // cols
    out = Image.new('RGB', (cols * cell, rows * (cell + 18)), (12, 12, 20))
    d = ImageDraw.Draw(out)
    for i, (gid, label) in enumerate(items):
        im = compose_group(gid, bank, remap).resize((cell - 4, cell - 4), Image.NEAREST)
        x = (i % cols) * cell; y = (i // cols) * (cell + 18)
        out.paste(im, (x + 2, y + 18), im)
        d.text((x + 2, y), label, fill=(255, 255, 0))
    out.save(os.path.join(OUT, name))
    print('wrote', name)

items = [(233 + t, '%d:%s' % (t, names.get(t, '?'))) for t in range(23)]
sheet(items, 9, 'all_units_blue.png')
sheet(items, 10, 'all_units_green.png')

# individual vehicle sprites
for t in range(23):
    safe = ''.join(c if c.isalnum() else '_' for c in names.get(t, 't%d' % t))
    compose_group(233 + t, 9).save(os.path.join(OUT, 'unit_%02d_%s_blue.png' % (t, safe)))
    compose_group(233 + t, 10).save(os.path.join(OUT, 'unit_%02d_%s_green.png' % (t, safe)))

# person / infantry with orange gun
fix = {4: (222, 90, 0)}
for bank, tag in ((9, 'blue'), (10, 'green')):
    p = compose_group(233, bank, fix)  # placeholder, replaced below
for tag, bank in (('blue', 9), ('green', 10)):
    # person chunks 176..179 (bank tiles 688..691)
    im = Image.new('RGBA', (64, 64), (0, 0, 0, 0))
    for k, spr in enumerate(range(688, 692)):
        dst = [(32, 0), (0, 0), (32, 32), (0, 32)][k]
        t = tile(spr, 6, True, fix)
        im.paste(t, dst, t)
    im.save(os.path.join(OUT, 'person_orange_gun_%s.png' % tag))
    im.resize((256, 256), Image.NEAREST).save(os.path.join(OUT, 'person_orange_gun_%s_big.png' % tag))
print('done')
