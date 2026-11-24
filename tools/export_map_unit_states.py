"""Export exact world-map palette states for every unit.

The map icon is row-4 tile 160+type.  The original map palette uses:
  bank 9  - allied/player blue (horizontal.png)
  bank 10 - computer green
  bank 12 - used-up / acted grey
This writes horizontal_green.png and horizontal_used.png next to the existing
blue horizontal.png using the same tile, so no runtime tint or shader guess is
needed.
"""
import json
import os

from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
OUT = os.path.join(ROOT, 'assets', 'units')
DATA = os.path.join(ROOT, 'assets', 'data', 'units.json')

b = open(os.path.join(ROOT, 'dump', 'spritebank.bin'), 'rb').read()
pal = json.load(open(os.path.join(ROOT, 'assets', 'art', 'palette.json'),
                     encoding='utf-8'))
PAL = [(p['r'], p['g'], p['b']) for p in pal]


def tile(idx, bank):
    im = Image.new('RGBA', (32, 32), (0, 0, 0, 0))
    px = im.load()
    off = idx * 1024
    for y in range(32):
        for x in range(32):
            v = b[off + y * 32 + x]
            if v:
                px[x, 31 - y] = PAL[((bank << 4) | (v & 0xF)) & 0xFF] + (255,)
    return im


units = json.load(open(DATA, encoding='utf-8'))
for u in units.get('units', []):
    t = u['id']
    folder = os.path.join(OUT, u.get('folder', 'unit_%02d' % t))
    if not os.path.isdir(folder):
        print('missing', folder)
        continue
    idx = 160 + (t if t < 23 else 0)
    tile(idx, 10).save(os.path.join(folder, 'horizontal_green.png'))
    tile(idx, 12).save(os.path.join(folder, 'horizontal_used.png'))
    uj = os.path.join(folder, 'unit.json')
    if os.path.exists(uj):
        meta = json.load(open(uj, encoding='utf-8'))
        meta['sprites'] = ['horizontal.png', 'horizontal_green.png',
                           'horizontal_used.png']
        meta['palette'] = 'map bank 9 blue / 10 green / 12 used'
        json.dump(meta, open(uj, 'w', encoding='utf-8'), ensure_ascii=False,
                  indent=1)
print('wrote map palette states for', len(units.get('units', [])), 'units')
