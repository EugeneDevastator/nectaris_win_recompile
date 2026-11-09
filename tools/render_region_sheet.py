"""Render a raw dumped memory region as a tile/sprite sheet with one palette."""
import os, sys
import numpy as np
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
BANKS = os.path.join(ROOT, 'dump', 'banks')
PAL = None


def load_pal(path):
    raw = open(path, 'rb').read()
    return [(raw[4 + i * 4], raw[5 + i * 4], raw[6 + i * 4]) for i in range(256)]


def render_region(region, out, bank=9, bpp=8, cell=1024, tw=32, th=32, cols=16,
                  start=0, count=None, scale=2, pal_path=None):
    global PAL
    PAL = load_pal(pal_path or os.path.join(BANKS, 'palette.bin'))
    data = np.frombuffer(open(os.path.join(BANKS, region), 'rb').read(), dtype=np.uint8)
    total = len(data) // cell
    count = total if count is None else min(count, total - start)
    cellpx = tw * scale + 4
    rows = (count + cols - 1) // cols
    sheet = Image.new('RGB', (cols * cellpx, rows * cellpx + 16), (18, 18, 26))
    d = ImageDraw.Draw(sheet)
    for k in range(count):
        idx = start + k
        chunk = data[idx * cell:(idx + 1) * cell]
        sx = (k % cols) * cellpx + 2
        sy = (k // cols) * cellpx + 16
        if bpp == 8:
            if len(chunk) < tw * th:
                continue
            px = chunk[:tw * th].reshape(th, tw)
        else:
            px = np.empty(tw * th, dtype=np.uint8)
            lo = chunk & 15
            hi = chunk >> 4
            px[0::2] = lo[:len(px) // 2]
            px[1::2] = hi[:len(px) // 2]
            px = px.reshape(th, tw)
        for y in range(th):
            for x in range(tw):
                v = int(px[th - 1 - y, x])
                if not v:
                    continue
                c = PAL[((bank << 4) | (v & 15)) & 255]
                for yy in range(scale):
                    for xx in range(scale):
                        sheet.putpixel((sx + x * scale + xx, sy + y * scale + yy), c)
        d.text((sx, sy - 12), str(idx), fill=(210, 210, 120))
    sheet.save(out)
    print('wrote', out, sheet.size, 'cells', count)


if __name__ == '__main__':
    a = sys.argv[1:]
    region = a[0]
    out = a[1]
    bank = int(a[2]) if len(a) > 2 else 9
    bpp = int(a[3]) if len(a) > 3 else 8
    cell = int(a[4]) if len(a) > 4 else 1024
    tw = int(a[5]) if len(a) > 5 else 32
    th = int(a[6]) if len(a) > 6 else 32
    cols = int(a[7]) if len(a) > 7 else 16
    start = int(a[8]) if len(a) > 8 else 0
    count = int(a[9]) if len(a) > 9 else None
    scale = int(a[10]) if len(a) > 10 else 2
    render_region(region, out, bank, bpp, cell, tw, th, cols, start, count, scale,
                  os.path.join(ROOT, 'dump', 'battle_sprites', 'palette.bin'))
