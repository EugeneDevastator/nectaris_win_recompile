"""Full spritebank contact sheet (32x32 entries) for manual inspection."""
import os, sys
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
D = os.path.join(ROOT, 'dump', 'battle_sprites')
b = open(os.path.join(D, 'spritebank.bin'), 'rb').read()
pal = open(os.path.join(D, 'palette.bin'), 'rb').read()
PAL = [(pal[4 + i * 4], pal[5 + i * 4], pal[6 + i * 4]) for i in range(256)]
S = int(sys.argv[1]) if len(sys.argv) > 1 else 0
E = int(sys.argv[2]) if len(sys.argv) > 2 else 1024
bank = int(sys.argv[3]) if len(sys.argv) > 3 else 9
n = len(b) // 1024
E = min(E, n)
cols, cell = 32, 42
rows = (E - S + cols - 1) // cols
sheet = Image.new('RGB', (cols * cell, rows * cell), (16, 16, 24))
d = ImageDraw.Draw(sheet)
for k, idx in enumerate(range(S, E)):
    im = Image.new('RGBA', (32, 32), (0, 0, 0, 0))
    px = im.load()
    off = idx * 1024
    for y in range(32):
        for x in range(32):
            v = b[off + y * 32 + x]
            if v:
                px[x, 31 - y] = PAL[((bank << 4) | (v & 15)) & 255] + (255,)
    c, r = k % cols, k // cols
    x, y = c * cell + 2, r * cell + 12
    sheet.paste(im, (x, y), im)
    d.text((x, y - 10), str(idx), fill=(200, 200, 90))
out = os.path.join(D, 'spritebank_%d_%d_b%d.png' % (S, E, bank))
sheet.save(out)
print('wrote', out, sheet.size)
