"""Fill transparent areas of the combat terrain panels so they render as a
continuous backdrop.

* top/bottom holes: the existing terrain is repeated vertically (tiled), which
  keeps the ground texture instead of a flat band;
* interior holes: diffuse fill (average of already-known neighbours) so large
  cut-outs (e.g. the canyon strip) blend smoothly instead of streaking.
"""
import os
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
D = os.path.join(ROOT, 'assets', 'art', 'combat_terrain')

for i in range(24):
    p = os.path.join(D, 'panel_%02d.png' % i)
    im = Image.open(p).convert('RGBA')
    px = im.load()

    def opaque(x, y):
        return px[x, y][3] > 0

    # 1. tile the terrain into the top/bottom holes per column
    for x in range(256):
        ys = [y for y in range(256) if opaque(x, y)]
        if not ys:
            continue
        first, last = ys[0], ys[-1]
        h = last - first + 1
        for y in range(first):
            px[x, y] = px[x, first + ((y - first) % h)]
        for y in range(last + 1, 256):
            px[x, y] = px[x, first + ((y - first) % h)]

    # 2. diffusion-fill the remaining (interior) holes
    left = {(x, y) for y in range(256) for x in range(256) if not opaque(x, y)}
    while left:
        todo = []
        for x, y in left:
            acc = [0, 0, 0, 0]
            n = 0
            for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
                nx, ny = x + dx, y + dy
                if 0 <= nx < 256 and 0 <= ny < 256 and opaque(nx, ny):
                    c = px[nx, ny]
                    acc[0] += c[0]; acc[1] += c[1]; acc[2] += c[2]; acc[3] += c[3]
                    n += 1
            if n:
                todo.append((x, y, [v // n for v in acc]))
        if not todo:
            break
        for x, y, c in todo:
            px[x, y] = tuple(c)
            left.discard((x, y))

    # 3. anything left (fully empty rows): copy the nearest filled row
    rows = [y for y in range(256) if any(opaque(x, y) for x in range(256))]
    if rows:
        for y in range(256):
            if any(opaque(x, y) for x in range(256)):
                continue
            src = min(rows, key=lambda r: abs(r - y))
            for x in range(256):
                px[x, y] = px[x, src]

    im.convert('RGB').save(p)

print('filled transparent areas in 24 panels (tiled + diffusion)')
