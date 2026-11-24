"""Clean per-unit assets to exactly three blue sprites:
  horizontal.png  (row 4, tile 160+type)
  vertical.png    (rotated 90)
  diagonal.png    (rotated 45)
Everything else in the folder is removed; unit.json stays.
"""
import os, json
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
b = open(os.path.join(ROOT, 'dump', 'spritebank.bin'), 'rb').read()
pal = json.load(open(os.path.join(ROOT, 'assets', 'art', 'palette.json')))
PAL = [(p['r'], p['g'], p['b']) for p in pal]
OUT = os.path.join(ROOT, 'assets', 'units')

def tile(idx, bank=9):
    im = Image.new('RGBA', (32, 32), (0, 0, 0, 0)); px = im.load(); off = idx * 1024
    for y in range(32):
        for x in range(32):
            v = b[off + y * 32 + x]
            if v:
                px[x, 31 - y] = PAL[((bank << 4) | (v & 0xF)) & 0xFF] + (255,)
    return im

def centered_rot(im, angle):
    r = im.rotate(angle, resample=Image.NEAREST, expand=True)
    out = Image.new('RGBA', (32, 32), (0, 0, 0, 0))
    out.paste(r, ((32 - r.width) // 2, (32 - r.height) // 2), r)
    return out

for t in range(24):
    folder = os.path.join(OUT, 'unit_%02d' % t)
    os.makedirs(folder, exist_ok=True)
    src = tile(160 + (t if t < 23 else 0))
    src.save(os.path.join(folder, 'horizontal.png'))
    centered_rot(src, 90).save(os.path.join(folder, 'vertical.png'))
    # remove everything except the map sprites and unit.json
    keep = {'horizontal.png', 'vertical.png', 'horizontal_green.png',
            'horizontal_used.png', 'unit.json'}
    for f in os.listdir(folder):
        if f not in keep:
            os.remove(os.path.join(folder, f))
    meta_path = os.path.join(folder, 'unit.json')
    meta = json.load(open(meta_path, encoding='utf-8'))
    meta['sprites'] = ['horizontal.png', 'horizontal_green.png',
                       'horizontal_used.png', 'vertical.png']
    meta['palette'] = 'blue default; tint for enemy'
    for k in ('world_map', 'world_map_tiles', 'world_map_sprites', 'combat_blue',
              'combat_green', 'combat_green_flip'):
        meta.pop(k, None)
    json.dump(meta, open(meta_path, 'w', encoding='utf-8'), indent=2, ensure_ascii=False)
print('cleaned 24 unit folders to horizontal/vertical/diagonal + unit.json')
