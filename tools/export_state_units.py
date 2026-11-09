"""Export combat units from a captured state as 64x64 composed quadrants."""
import os, sys
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
D = os.path.join(ROOT, 'dump', 'block_changes')
STATE = sys.argv[1] if len(sys.argv) > 1 else '01_db71d13f'
OUT = os.path.join(ROOT, 'assets', 'art', 'combat_units_real')
os.makedirs(OUT, exist_ok=True)
b = open(os.path.join(D, STATE, 'block.bin'), 'rb').read()
pal = open(os.path.join(ROOT, 'dump', 'battle_sprites', 'palette.bin'), 'rb').read()
PAL = [(pal[4 + i * 4], pal[5 + i * 4], pal[6 + i * 4]) for i in range(256)]


def tile(k, bank):
    im = Image.new('RGBA', (32, 32), (0, 0, 0, 0))
    px = im.load()
    for y in range(32):
        for x in range(32):
            v = b[k * 1024 + y * 32 + x]
            if v:
                px[x, 31 - y] = PAL[((bank << 4) | (v & 15)) & 255] + (255,)
    return im


def compose(group, bank):
    c = Image.new('RGBA', (64, 64), (0, 0, 0, 0))
    for j in range(4):
        k = group * 4 + j
        c.alpha_composite(tile(k, bank), ((j % 2) * 32, (j // 2) * 32))
    return c


for bank, tag in ((9, 'blue'), (10, 'green')):
    for g in range(8):
        im = compose(g, bank)
        bb = im.getbbox()
        if not bb:
            continue
        im.save(os.path.join(OUT, 'combat_g%d_%s.png' % (g, tag)))
    print('exported bank', tag)
print('state', STATE)
