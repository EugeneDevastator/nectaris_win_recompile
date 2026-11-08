"""Rebuild the 64x64 guide frames from the raw spritebank tile dumps."""
import os, json
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
GUIDE = os.path.join(ROOT, 'dump', 'guide')
UNITS = os.path.join(ROOT, 'assets', 'units')
OFFS = ((4, 0), (36, 0), (4, 32), (36, 32))

def build(tiles, pal):
    img = Image.new('RGBA', (64, 64), (0, 0, 0, 0)); px = img.load()
    for i, (ox, oy) in enumerate(OFFS):
        tile = tiles[i]
        for y in range(32):
            for x in range(32):
                v = tile[y * 32 + x]
                if v:
                    xx = ox + x; yy = oy + (31 - y)
                    if 0 <= xx < 64 and 0 <= yy < 64:
                        px[xx, yy] = pal[(0xE0 | (v & 0xF)) & 0xFF] + (255,)
    return img

for t in range(24):
    rawp = os.path.join(GUIDE, 'bank_type_%02d.bin' % t)
    palp = os.path.join(GUIDE, 'pal_type_%02d.bin' % t)
    if not os.path.exists(rawp):
        print('missing', t); continue
    raw = open(rawp, 'rb').read()
    palraw = open(palp, 'rb').read()
    pal = [(palraw[4 + i * 4], palraw[5 + i * 4], palraw[6 + i * 4]) for i in range(256)]
    tiles = [raw[i * 1024:(i + 1) * 1024] for i in range(8)]
    folder = os.path.join(UNITS, 'unit_%02d' % t)
    if t == 23:
        Image.new('RGBA', (64, 64), (0, 0, 0, 0)).save(os.path.join(folder, 'combat_alive.png'))
        Image.new('RGBA', (64, 64), (0, 0, 0, 0)).save(os.path.join(folder, 'combat_diagonal.png'))
        print('unit 23 dummy -> empty')
        continue
    build(tiles[:4], pal).save(os.path.join(folder, 'combat_alive.png'))
    build(tiles[4:], pal).save(os.path.join(folder, 'combat_diagonal.png'))
    print('unit %02d rebuilt 64x64' % t)
