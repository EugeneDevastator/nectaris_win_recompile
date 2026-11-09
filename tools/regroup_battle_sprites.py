"""Regroup resource sprite pages into units x rotations and verify layout."""
import json, os
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
D = os.path.join(ROOT, 'dump', 'art_resources')
M = {r['id']: r for r in json.load(open(os.path.join(D, 'manifest.json')))['resources']}
pal = open(os.path.join(D, 'palette.bin'), 'rb').read()
PAL = [(pal[4 + i * 4], pal[5 + i * 4], pal[6 + i * 4]) for i in range(256)]


def load(rid):
    r = M[rid]
    raw = open(os.path.join(D, r['expanded']), 'rb').read()
    return raw, r['units']


def sprite(raw, t):
    return [raw[t * 1024 + y * 32:(t * 1024 + y * 32) + 32] for y in range(32)]


def nonzero(raw, t):
    return sum(1 for b in raw[t * 1024:(t + 1) * 1024] if b)


def regroup(rid, units_per_page, rotations, bank, name):
    raw, n = load(rid)
    scale = 4
    cell = 32 * scale + 6
    sheet = Image.new('RGB', (units_per_page * cell, rotations * cell + 18), (18, 18, 26))
    d = ImageDraw.Draw(sheet)
    for rot in range(rotations):
        for u in range(units_per_page):
            t = rot * units_per_page + u
            if t >= n:
                continue
            sx = u * cell + 3
            sy = rot * cell + 18
            for y in range(32):
                for x in range(32):
                    v = raw[t * 1024 + y * 32 + x]
                    if not v:
                        continue
                    col = PAL[((bank << 4) | (v & 15)) & 255]
                    for yy in range(scale):
                        for xx in range(scale):
                            sheet.putpixel((sx + x * scale + xx, sy + (31 - y) * scale + yy), col)
            d.text((sx, sy - 12), '%d/%d' % (u, rot), fill=(255, 230, 100))
    out = os.path.join(D, name)
    sheet.save(out)
    print('wrote', out, sheet.size)


if __name__ == '__main__':
    for rid in (13, 14, 15, 16):
        raw, n = load(rid)
        counts = [nonzero(raw, t) for t in range(n)]
        print('res', rid, 'sprites', n, 'nonzero per sprite', counts)
    regroup(13, 16, 4, 9, 'res13_units_rotations.png')
    regroup(14, 16, 2, 9, 'res14_units_rotations.png')
    regroup(15, 16, 4, 9, 'res15_units_rotations.png')
    regroup(16, 16, 4, 9, 'res16_units_rotations.png')
