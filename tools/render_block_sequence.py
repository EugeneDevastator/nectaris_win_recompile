"""Render the captured block-change states as sprite grids."""
import os
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
D = os.path.join(ROOT, 'dump', 'block_changes')
dirs = sorted(d for d in os.listdir(D) if os.path.isdir(os.path.join(D, d)))
pal = open(os.path.join(D, dirs[0], 'palette.bin'), 'rb').read()
PAL = [(pal[4 + i * 4], pal[5 + i * 4], pal[6 + i * 4]) for i in range(256)]

cols = 8
cell = 34
sw, sh = cols * cell, 4 * cell
sheet = Image.new('RGB', (160 + sw, len(dirs) * (sh + 16)), (18, 18, 26))
d = ImageDraw.Draw(sheet)
for r, dd in enumerate(dirs):
    b = open(os.path.join(D, dd, 'block.bin'), 'rb').read()
    y0 = r * (sh + 16)
    d.text((2, y0 + 2), dd, fill=(255, 230, 100))
    for k in range(32):
        x = 160 + (k % cols) * cell + 1
        y = y0 + (k // cols) * cell + 1
        for yy in range(32):
            for xx in range(32):
                v = b[k * 1024 + yy * 32 + xx]
                if v:
                    sheet.putpixel((x + xx, y + yy),
                                   PAL[((9 << 4) | (v & 15)) & 255])
sheet.save(os.path.join(D, 'sequence.png'))
print('wrote', os.path.join(D, 'sequence.png'), sheet.size, len(dirs))
