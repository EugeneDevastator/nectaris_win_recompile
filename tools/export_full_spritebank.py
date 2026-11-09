"""Export the whole live sprite bank (512 unique 32x32 sprites) as one atlas."""
import os, sys, json
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
SRC = sys.argv[1] if len(sys.argv) > 1 else os.path.join(ROOT, 'dump', 'forced_attacks', 'type_18_20cd778b', 'spritebank.bin')
BANK = int(sys.argv[2]) if len(sys.argv) > 2 else 9
TAG = sys.argv[3] if len(sys.argv) > 3 else 'combat'
OUT = os.path.join(ROOT, 'assets', 'art', 'combat_bank_full')
os.makedirs(OUT, exist_ok=True)
b = open(SRC, 'rb').read()
pal = open(os.path.join(ROOT, 'dump', 'battle_sprites', 'palette.bin'), 'rb').read()
PAL = [(pal[4 + i * 4], pal[5 + i * 4], pal[6 + i * 4]) for i in range(256)]

cols = 16
cell = 34
n = min(512, len(b) // 1024)
rows = (n + cols - 1) // cols
sheet = Image.new('RGB', (cols * cell + 1, rows * cell + 14), (18, 18, 26))
d = ImageDraw.Draw(sheet)
for t in range(n):
    im = Image.new('RGBA', (32, 32), (0, 0, 0, 0))
    px = im.load()
    for y in range(32):
        for x in range(32):
            v = b[t * 1024 + y * 32 + x]
            if v:
                px[x, 31 - y] = PAL[((BANK << 4) | (v & 15)) & 255] + (255,)
    x = (t % cols) * cell + 1
    y = (t // cols) * cell + 14
    sheet.paste(im, (x, y), im)
    d.text((x, y - 11), str(t), fill=(190, 190, 90))
out = os.path.join(OUT, 'spritebank_full_%s_b%02d.png' % (TAG, BANK))
sheet.save(out)
print('wrote', out, sheet.size)
