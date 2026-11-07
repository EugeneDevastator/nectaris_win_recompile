"""Re-extract the sprite bank with per-tile vertical flip (memory is bottom-up)
and render contact sheets to locate the battle 'person' sprite."""
import os, json
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
b = open(os.path.join(ROOT, 'dump', 'spritebank.bin'), 'rb').read()
pal = json.load(open(os.path.join(ROOT, 'assets', 'art', 'palette.json')))
PAL = [(p['r'], p['g'], p['b']) for p in pal]
OUT = os.path.join(ROOT, 'dump')
os.makedirs(OUT, exist_ok=True)

def tile(idx, bank, flip=True):
    img = Image.new('RGBA', (32, 32), (0, 0, 0, 0))
    px = img.load()
    off = idx * 1024
    for y in range(32):
        for x in range(32):
            v = b[off + y * 32 + x]
            if v:
                px[x, 31 - y if flip else y] = PAL[((bank << 4) | (v & 0xF)) & 0xFF] + (255,)
    return img

def sheet(a, bb, bank, flip, name, cols=16, cell=44):
    n = bb - a
    rows = (n + cols - 1) // cols
    out = Image.new('RGB', (cols * cell, rows * (cell + 12)), (12, 12, 20))
    d = ImageDraw.Draw(out)
    for i, idx in enumerate(range(a, bb)):
        t = tile(idx, bank, flip).resize((cell - 6, cell - 6), Image.NEAREST)
        x = (i % cols) * cell
        y = (i // cols) * (cell + 12)
        out.paste(t, (x + 3, y + 12), t)
        d.text((x + 1, y), str(idx), fill=(255, 255, 0))
    out.save(os.path.join(OUT, name))
    print('wrote', name, out.size)

# compare orientation on a likely combat sprite
for idx in (160, 166, 168, 174):
    a = tile(idx, 9, False).resize((128, 128), Image.NEAREST)
    c = tile(idx, 9, True).resize((128, 128), Image.NEAREST)
    out = Image.new('RGB', (270, 150), (10, 10, 16))
    d = ImageDraw.Draw(out)
    out.paste(a, (5, 20), a); d.text((5, 2), 'raw', fill=(255, 255, 0))
    out.paste(c, (140, 20), c); d.text((140, 2), 'vflip', fill=(255, 255, 0))
    out.save(os.path.join(OUT, f'orient_{idx}.png'))

sheet(0, 320, 9, True, 'bank9_flip_0_319.png')
sheet(0, 320, 10, True, 'bank10_flip_0_319.png')
sheet(0, 320, 6, True, 'bank6_flip_0_319.png')
sheet(0, 320, 11, True, 'bank11_flip_0_319.png')
print('done')
