"""Export row 4 (tiles 160..191) = horizontal side-view combat sprites, one per
unit, into the per-unit folders (32x32, both teams)."""
import os, json
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
b = open(os.path.join(ROOT, 'dump', 'spritebank.bin'), 'rb').read()
pal = json.load(open(os.path.join(ROOT, 'assets', 'art', 'palette.json')))
PAL = [(p['r'], p['g'], p['b']) for p in pal]
OUT = os.path.join(ROOT, 'assets', 'units')

def tile(idx, bank):
    im = Image.new('RGBA', (32, 32), (0, 0, 0, 0)); px = im.load(); off = idx * 1024
    for y in range(32):
        for x in range(32):
            v = b[off + y * 32 + x]
            if v:
                px[x, 31 - y] = PAL[((bank << 4) | (v & 0xF)) & 0xFF] + (255,)
    return im

for t in range(24):
    idx = 160 + t if t < 23 else 160
    folder = os.path.join(OUT, 'unit_%02d' % t)
    os.makedirs(folder, exist_ok=True)
    blue = tile(idx, 9)
    green = tile(idx, 10)
    blue.save(os.path.join(folder, 'combat_blue.png'))
    green.save(os.path.join(folder, 'combat_green.png'))
    green.transpose(Image.FLIP_LEFT_RIGHT).save(os.path.join(folder, 'combat_green_flip.png'))
    # world-map icons: rows 2/3 = tiles 96 + 2*type (vertical) and 97 + 2*type (diagonal)
    base = 96 + t * 2
    vert = tile(base, 9)
    diag = tile(base + 1, 9)
    strip = Image.new('RGBA', (64, 32), (0, 0, 0, 0))
    strip.paste(vert, (0, 0), vert)
    strip.paste(diag, (32, 0), diag)
    strip.save(os.path.join(folder, 'world_map.png'))
print('exported row-4 combat (160+type) + rows-2/3 worldmap (96+2*type)')
