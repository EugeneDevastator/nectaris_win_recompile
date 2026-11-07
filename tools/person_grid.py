import os
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
b = open(os.path.join(ROOT, 'dump', 'spritebank_battle.bin'), 'rb').read()

# compose base 688 in the UR/UL/BR/BL order, keep raw nibbles
grid = [[0]*64 for _ in range(64)]
for k, (ox, oy) in enumerate([(32, 0), (0, 0), (32, 32), (0, 32)]):
    raw = b[(688 + k) * 1024:(688 + k) * 1024 + 1024]
    for y in range(32):
        for x in range(32):
            v = raw[y * 32 + x]
            if v:
                grid[oy + x * 0 + y][ox + x] = v & 0xF

# print as hex, only non-zero
for y in range(64):
    row = ''.join('%x' % grid[y][x] if grid[y][x] else '.' for x in range(64))
    if row.strip('.'):
        print('%02d %s' % (y, row))

# unique nibbles
from collections import Counter
c = Counter(v for row in grid for v in row if v)
print('nibbles', c.most_common())
