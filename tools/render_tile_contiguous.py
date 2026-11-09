"""Render tile (type 0/1) resources as contiguous bitmaps with one palette."""
import json, os, sys
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
D = os.path.join(ROOT, 'dump', 'art_resources')
OUT = os.path.join(ROOT, 'dump', 'decoded_blue_contig')
os.makedirs(OUT, exist_ok=True)
M = json.load(open(os.path.join(D, 'manifest.json')))['resources']
pal = open(os.path.join(ROOT, 'dump', 'battle_sprites', 'palette.bin'), 'rb').read()
PAL = [(pal[4 + i * 4], pal[5 + i * 4], pal[6 + i * 4]) for i in range(256)]
BANK = 9


def render(rid, count, raw, cols, scale=3):
    rows = (count + cols - 1) // cols
    w, h = cols * 16, rows * 16
    im = Image.new('RGB', (w, h), (0, 0, 0))
    px = im.load()
    for t in range(count):
        tx = (t % cols) * 16
        ty = (t // cols) * 16
        for y in range(16):
            for x in range(16):
                v = raw[t * 256 + y * 16 + x]
                if v:
                    px[tx + x, ty + (15 - y)] = PAL[((BANK << 4) | (v & 15)) & 255]
    if scale != 1:
        im = im.resize((w * scale, h * scale), Image.NEAREST)
    return im


for r in M:
    if r['kind'] != 'tile':
        continue
    raw = open(os.path.join(D, r['expanded']), 'rb').read()
    n = min(r['units'], len(raw) // 256)
    cols = 16 if n <= 256 else 32
    im = render(r['id'], n, raw, cols)
    fn = os.path.join(OUT, 'res_%02d_contig_b09.png' % r['id'])
    im.save(fn)
    # also a raw 1:1 copy
    im.resize((im.width // 3, im.height // 3), Image.NEAREST).save(
        os.path.join(OUT, 'res_%02d_contig_1x.png' % r['id']))
    print(fn, im.size, 'tiles', n)
