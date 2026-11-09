"""Render each unit's four rotation sprites from the sprite group table."""
import os, json, sys
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
D = os.path.join(ROOT, 'dump', 'battle_sprites')
b = open(os.path.join(D, 'spritebank.bin'), 'rb').read()
pal = open(os.path.join(D, 'palette.bin'), 'rb').read()
PAL = [(pal[4 + i * 4], pal[5 + i * 4], pal[6 + i * 4]) for i in range(256)]
groups = json.load(open(os.path.join(ROOT, 'dump', 'sprite_groups.json')))
units = json.load(open(os.path.join(ROOT, 'assets', 'data', 'units.json'), encoding='utf-8'))['units']
names = {u['id']: u['name'] for u in units}


def tile(idx, bank):
    im = Image.new('RGBA', (32, 32), (0, 0, 0, 0))
    px = im.load()
    off = idx * 1024
    if off + 1024 > len(b):
        return im
    for y in range(32):
        for x in range(32):
            v = b[off + y * 32 + x]
            if v:
                px[x, 31 - y] = PAL[((bank << 4) | (v & 15)) & 255] + (255,)
    return im


sel = [int(x) for x in sys.argv[1:]] or [6]
for t in sel:
    gid = 233 + t
    parts = groups[str(gid)]
    print(t, names.get(t, '?'), 'group', gid, parts)
    sheet = Image.new('RGB', (4 * 140, 170), (20, 20, 30))
    d = ImageDraw.Draw(sheet)
    for k, (dx, dy, spr, fl) in enumerate(parts):
        im = tile(spr, 9).resize((128, 128), Image.NEAREST)
        x = k * 140 + 4
        sheet.paste(im, (x, 22), im)
        d.text((x, 4), 'spr %d @%d,%d f%d' % (spr, dx, dy, fl), fill=(255, 255, 0))
    out = os.path.join(D, 'unit_%02d_rotations.png' % t)
    sheet.save(out)
    print('wrote', out)
