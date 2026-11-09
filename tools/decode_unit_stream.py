"""Decode a unit resource stream as 32x32 4bpp chunks (512 bytes each)."""
import os, sys
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
RID = int(sys.argv[1]) if len(sys.argv) > 1 else 44
BANK = int(sys.argv[2]) if len(sys.argv) > 2 else 9
SRC = os.path.join(ROOT, 'dump', 'resources_lzss', 'res_%03d.bin' % RID)
if not os.path.exists(SRC):
    SRC = os.path.join(ROOT, 'dump', 'resources_lzss', 'res_%02d.bin' % RID)
OUT = os.path.join(ROOT, 'assets', 'art', 'unit_streams', 'res_%03d' % RID)
os.makedirs(OUT, exist_ok=True)
d = open(SRC, 'rb').read()
pal = open(os.path.join(ROOT, 'dump', 'battle_sprites', 'palette.bin'), 'rb').read()
PAL = [(pal[4 + i * 4], pal[5 + i * 4], pal[6 + i * 4]) for i in range(256)]
CH = 512
n = len(d) // CH


def chunk_img(k):
    blk = d[k * CH:(k + 1) * CH]
    im = Image.new('RGBA', (32, 32), (0, 0, 0, 0))
    px = im.load()
    for y in range(32):
        for x in range(32):
            b = blk[y * 16 + (x >> 1)]
            v = (b >> 4) if (x & 1) == 0 else (b & 0xF)
            if v:
                px[x, 31 - y] = PAL[((BANK << 4) | v) & 255] + (255,)
    return im


cols = 16
cell = 42
rows = (n + cols - 1) // cols
sheet = Image.new('RGB', (cols * cell, rows * cell + 16), (70, 70, 70))
dr = ImageDraw.Draw(sheet)
for k in range(n):
    im = chunk_img(k)
    im.save(os.path.join(OUT, 'chunk_%03d.png' % k))
    x = (k % cols) * cell + 4
    y = (k // cols) * cell + 16
    sheet.paste(im, (x, y), im)
    dr.text((x, y - 12), str(k), fill=(10, 10, 10))
sheet.save(os.path.join(OUT, 'atlas.png'))
print('res', RID, 'chunks', n, 'atlas', sheet.size)
