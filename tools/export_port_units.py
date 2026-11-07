"""Export a clean numbered set of 64x64 battle sprites for the port.

types 0..22 -> sprite-group 233+t (vehicles)
types 18/19 -> infantry person sprite
type 23     -> group 255
"""
import os, json
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
b = open(os.path.join(ROOT, 'dump', 'spritebank_battle.bin'), 'rb').read()
pal = json.load(open(os.path.join(ROOT, 'assets', 'art', 'palette.json')))
PAL = [(p['r'], p['g'], p['b']) for p in pal]
groups = json.load(open(os.path.join(ROOT, 'dump', 'sprite_groups.json')))
OUT = os.path.join(ROOT, 'assets', 'art', 'combat_units')
os.makedirs(OUT, exist_ok=True)

def tile(idx, bank, remap=None):
    im = Image.new('RGBA', (32, 32), (0, 0, 0, 0)); px = im.load(); off = idx * 1024
    if off + 1024 > len(b):
        return im
    for y in range(32):
        for x in range(32):
            v = b[off + y * 32 + x]
            if not v:
                continue
            n = v & 0xF
            col = remap.get(n, PAL[((bank << 4) | n) & 0xFF]) if remap else PAL[((bank << 4) | n) & 0xFF]
            px[x, 31 - y] = col + (255,)
    return im

def compose(sprites, bank, remap=None):
    im = Image.new('RGBA', (64, 64), (0, 0, 0, 0))
    for k, spr in enumerate(sprites[:4]):
        dst = [(32, 0), (0, 0), (32, 32), (0, 32)][k]   # UR UL BR BL
        t = tile(spr, bank, remap)
        im.paste(t, dst, t)
    return im

def group_sprites(gid):
    return [p[2] + 512 for p in groups[str(gid)]]

PERSON = [688, 689, 690, 691]
GUN_FIX = {4: (222, 90, 0)}

for t in range(24):
    if t in (18, 19):
        blue = compose(PERSON, 6, GUN_FIX)
        green = blue.copy()
    else:
        gid = 233 + t if t <= 22 else 255
        blue = compose(group_sprites(gid), 9)
        green = compose(group_sprites(gid), 10)
    blue.save(os.path.join(OUT, f'battle_unit_{t:02d}_blue.png'))
    green.save(os.path.join(OUT, f'battle_unit_{t:02d}_green.png'))
    green.transpose(Image.FLIP_LEFT_RIGHT).save(
        os.path.join(OUT, f'battle_unit_{t:02d}_green_flip.png'))
print('exported 24 types x 3 variants')
