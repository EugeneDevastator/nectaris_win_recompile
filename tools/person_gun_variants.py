import os, json
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
b = open(os.path.join(ROOT, 'dump', 'spritebank_battle.bin'), 'rb').read()
pal = json.load(open(os.path.join(ROOT, 'assets', 'art', 'palette.json')))
PAL = [(p['r'], p['g'], p['b']) for p in pal]
OUT = os.path.join(ROOT, 'assets', 'art', 'combat_units')

def tile(idx, remap):
    im = Image.new('RGBA', (32, 32), (0, 0, 0, 0)); px = im.load(); off = idx * 1024
    for y in range(32):
        for x in range(32):
            v = b[off + y * 32 + x]
            if v:
                n = v & 0xF
                col = remap.get(n, PAL[(6 << 4) | n])
                px[x, 31 - y] = col + (255,)
    return im

def compose(base, remap):
    im = Image.new('RGBA', (64, 64), (0, 0, 0, 0))
    for k, dst in enumerate([(32, 0), (0, 0), (32, 32), (0, 32)]):
        t = tile(base + k, remap)
        im.paste(t, dst, t)
    return im

variants = {
    'gun4_orange': {4: (222, 90, 0)},
    'gun4_3_orange': {4: (255, 140, 0), 3: (255, 200, 0)},
    'gun6_orange': {6: (255, 140, 0)},
    'gun4_6_orange': {4: (255, 140, 0), 6: (222, 90, 0)},
}
out = Image.new('RGB', (len(variants) * 160, 200), (10, 10, 16))
d = ImageDraw.Draw(out)
for i, (name, remap) in enumerate(variants.items()):
    im = compose(688, remap).resize((150, 150), Image.NEAREST)
    out.paste(im, (i * 160 + 5, 30), im)
    d.text((i * 160 + 5, 8), name, fill=(255, 255, 0))
    compose(688, remap).save(os.path.join(OUT, f'person_{name}.png'))
out.save(os.path.join(ROOT, 'dump', 'person_gun_variants.png'))
print('wrote variants')
