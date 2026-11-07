"""Correct per-unit map icons: the group table (0x42b170 -> group) lists the
rotated sprite indices; their bank tiles are 576 + sprite."""
import os, json
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
b = open(os.path.join(ROOT, 'dump', 'spritebank.bin'), 'rb').read()
pal = json.load(open(os.path.join(ROOT, 'assets', 'art', 'palette.json')))
PAL = [(p['r'], p['g'], p['b']) for p in pal]
groups = json.load(open(os.path.join(ROOT, 'dump', 'sprite_groups.json')))
exe = open(os.path.join(ROOT, 'game', 'Nec.exe'), 'rb').read()
BASE_VA, BASE_OFF = 0x42a000, 0x27c00
def foff(va): return BASE_OFF + (va - BASE_VA)
table = list(exe[foff(0x42b170):foff(0x42b170) + 48])
OUT = os.path.join(ROOT, 'assets', 'units')

def tile(idx, bank=9):
    im = Image.new('RGBA', (32, 32), (0, 0, 0, 0)); px = im.load(); off = idx * 1024
    if off + 1024 > len(b):
        return im
    for y in range(32):
        for x in range(32):
            v = b[off + y * 32 + x]
            if v:
                px[x, 31 - y] = PAL[((bank << 4) | (v & 0xF)) & 0xFF] + (255,)
    return im

for t in range(24):
    folder = os.path.join(OUT, 'unit_%02d' % t)
    if not os.path.isdir(folder):
        continue
    gid = table[t * 2]
    parts = groups.get(str(gid), [])
    sprites = []
    for p in parts:
        s = p[2]
        if s not in sprites:
            sprites.append(s)
    if not sprites:
        sprites = [t]
    frames = [tile(576 + s) for s in sprites]
    strip = Image.new('RGBA', (len(frames) * 32, 32), (0, 0, 0, 0))
    for i, f in enumerate(frames):
        strip.paste(f, (i * 32, 0), f)
    strip.save(os.path.join(folder, 'world_map.png'))
    meta_path = os.path.join(folder, 'unit.json')
    meta = json.load(open(meta_path, encoding='utf-8'))
    meta['world_map_tiles'] = [576 + s for s in sprites]
    meta['world_map_sprites'] = sprites
    json.dump(meta, open(meta_path, 'w', encoding='utf-8'), indent=2, ensure_ascii=False)
print('re-exported world_map strips from group rotations (576+sprite)')
