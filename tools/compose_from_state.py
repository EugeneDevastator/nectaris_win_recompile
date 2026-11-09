"""Compose the four quadrant slots of a captured combat state into 64x64 units."""
import os, sys, json
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
D = os.path.join(ROOT, 'dump', 'block_changes')
STATE = sys.argv[1] if len(sys.argv) > 1 else '01_db71d13f'
b = open(os.path.join(D, STATE, 'block.bin'), 'rb').read()
pal = open(os.path.join(ROOT, 'dump', 'battle_sprites', 'palette.bin'), 'rb').read()
PAL = [(pal[4 + i * 4], pal[5 + i * 4], pal[6 + i * 4]) for i in range(256)]
groups = json.load(open(os.path.join(ROOT, 'dump', 'sprite_groups.json')))


def tile(k, bank):
    im = Image.new('RGBA', (32, 32), (0, 0, 0, 0))
    px = im.load()
    if k * 1024 + 1024 > len(b):
        return im
    for y in range(32):
        for x in range(32):
            v = b[k * 1024 + y * 32 + x]
            if v:
                px[x, 31 - y] = PAL[((bank << 4) | (v & 15)) & 255] + (255,)
    return im


for gid in (233,):
    parts = groups[str(gid)]
    for bank in (9, 10, 1, 2, 3, 4):
        for mode in ('group_offsets', 'grid'):
            canvas = Image.new('RGBA', (96, 96), (0, 0, 0, 0))
            for j, (dx, dy, spr, fl) in enumerate(parts):
                k = spr - 64 + 0
                if mode == 'group_offsets':
                    sdx = dx - 256 if dx > 127 else dx
                    sdy = dy - 256 if dy > 127 else dy
                    # map group's (-16,-16)..(0,0) onto 2x2 layout anchored at 16
                    px = 16 + (0 if sdx < 0 else 32)
                    py = 16 + (0 if sdy < 0 else 32)
                else:
                    px = (j % 2) * 32
                    py = (j // 2) * 32
                canvas.alpha_composite(tile(k, bank), (px, py))
            bb = canvas.getbbox()
            c = canvas.crop(bb) if bb else canvas
            if c.width == 0:
                continue
            c.resize((c.width * 5, c.height * 5), Image.NEAREST).save(
                os.path.join(D, 'attacker_%s_b%d.png' % (mode, bank)))
            print(mode, 'bank', bank, c.size)
