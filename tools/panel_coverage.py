import os
from PIL import Image
ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
D = os.path.join(ROOT, 'assets', 'art', 'combat_terrain')
for i in range(24):
    im = Image.open(os.path.join(D, 'panel_%02d.png' % i)).convert('RGBA')
    px = im.load()
    rows = []
    for y in range(256):
        n = sum(1 for x in range(256) if px[x, y][3] > 0)
        rows.append(n)
    full = sum(1 for n in rows if n > 200)
    empty = sum(1 for n in rows if n < 20)
    print('panel %02d opaque_rows=%3d empty_rows=%3d' % (i, full, empty))
