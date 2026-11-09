"""Export combat unit sprites from the identified clean block (sb_156 / 9f01e50b).

Layout derived from inspection:
  slots 128..191  clean chassis:  8 units x 4 rotations x 2 horizontal chunks
  slots 192..255  damaged chassis: same layout
A chassis rotation is a 64x32 sprite (two 32x32 chunks side by side).
"""
import os, json, sys
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
D = os.path.join(ROOT, 'dump', 'ai_turn2')
STATE = sys.argv[1] if len(sys.argv) > 1 else 'sb_156.bin'
OUT = os.path.join(ROOT, 'assets', 'art', 'combat_units_real')
os.makedirs(OUT, exist_ok=True)
b = open(os.path.join(D, STATE), 'rb').read()
pal = open(os.path.join(D, 'palette.bin'), 'rb').read()
PAL = [(pal[4 + i * 4], pal[5 + i * 4], pal[6 + i * 4]) for i in range(256)]


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


def chassis(slot, bank):
    c = Image.new('RGBA', (64, 32), (0, 0, 0, 0))
    c.alpha_composite(tile(slot, bank), (0, 0))
    c.alpha_composite(tile(slot + 1, bank), (32, 0))
    return c


manifest = []
for bank, tag in ((9, 'blue'), (10, 'green'), (11, 'red'), (12, 'gray')):
    for damaged, base, dtag in ((False, 128, 'clean'), (True, 192, 'damaged')):
        sheet = Image.new('RGB', (4 * 210, 8 * 80), (18, 18, 26))
        dr = ImageDraw.Draw(sheet)
        for u in range(8):
            for r in range(4):
                slot = base + 2 * u + 16 * r
                im = chassis(slot, bank)
                fn = 'chassis_%02d_rot%d_%s_%s.png' % (u, r, dtag, tag)
                im.save(os.path.join(OUT, fn))
                manifest.append({'chassis': u, 'rotation': r, 'state': dtag,
                                 'bank': bank, 'file': fn, 'slot': slot})
                big = im.resize((192, 96), Image.NEAREST)
                x, y = r * 210 + 6, u * 80 + 20
                sheet.paste(big, (x, y), big)
                dr.text((x, y - 13), 'chassis %d rot %d' % (u, r), fill=(255, 230, 100))
        name = 'all_chassis_%s_%s.png' % (dtag, tag)
        sheet.save(os.path.join(OUT, name))
        print('wrote', name)

json.dump(manifest, open(os.path.join(OUT, 'combat_chassis_manifest.json'), 'w'), indent=1)
print('sprites', len(manifest))
