"""Render a resource sprite page large, with index labels and solid scaling."""
import json, os, sys
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
D = os.path.join(ROOT, 'dump', 'art_resources')
M = {r['id']: r for r in json.load(open(os.path.join(D, 'manifest.json')))['resources']}
pal = open(os.path.join(D, 'palette.bin'), 'rb').read()
PAL = [(pal[4 + i * 4], pal[5 + i * 4], pal[6 + i * 4]) for i in range(256)]


def render(rid, bank, scale=5, cols=8):
    r = M[rid]
    raw = open(os.path.join(D, r['expanded']), 'rb').read()
    n = min(r['units'], len(raw) // 1024)
    cell = 32 * scale + 8
    rows = (n + cols - 1) // cols
    sheet = Image.new('RGB', (cols * cell, rows * cell + 18), (18, 18, 26))
    d = ImageDraw.Draw(sheet)
    for t in range(n):
        sx = (t % cols) * cell + 4
        sy = (t // cols) * cell + 18
        for y in range(32):
            for x in range(32):
                v = raw[t * 1024 + y * 32 + x]
                if not v:
                    continue
                col = PAL[((bank << 4) | (v & 15)) & 255]
                for yy in range(scale):
                    for xx in range(scale):
                        sheet.putpixel((sx + x * scale + xx, sy + (31 - y) * scale + yy), col)
        d.text((sx, sy - 13), '%d' % t, fill=(255, 230, 100))
    out = os.path.join(D, 'big_res%02d_b%02d.png' % (rid, bank))
    sheet.save(out)
    print('wrote', out, sheet.size, 'sprites', n)


if __name__ == '__main__':
    rids = [int(x) for x in sys.argv[1:]] or [13]
    for rid in rids:
        for bank in (6, 9, 10, 11, 12):
            render(rid, bank, scale=4, cols=8)
