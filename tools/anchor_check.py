import os, json
from PIL import Image, ImageDraw
ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
b = open(os.path.join(ROOT, 'dump', 'spritebank.bin'), 'rb').read()
pal = json.load(open(os.path.join(ROOT, 'assets', 'art', 'palette.json')))
PAL = [(p['r'], p['g'], p['b']) for p in pal]

def t8(idx, bank=9):
    im = Image.new('RGBA', (32, 32), (0, 0, 0, 0)); px = im.load(); off = idx * 1024
    for y in range(32):
        for x in range(32):
            v = b[off + y * 32 + x]
            if v:
                px[x, 31 - y] = PAL[((bank << 4) | (v & 0xF)) & 0xFF] + (255,)
    return im

items = [
    ('combat t2 = 162', 162),
    ('row3 #2 = 129', 129), ('row3 #3 = 130', 130),
    ('combat t7 = 167', 167),
    ('row2 #13 = 108', 108), ('row2 #14 = 109', 109),
    ('row2 #14 = 110', 110),
]
out = Image.new('RGB', (len(items) * 140, 170), (10, 10, 18))
d = ImageDraw.Draw(out)
for i, (label, idx) in enumerate(items):
    im = t8(idx).resize((128, 128), Image.NEAREST)
    out.paste(im, (i * 140 + 6, 30), im)
    d.text((i * 140 + 4, 8), label, fill=(255, 255, 0))
out.save(os.path.join(ROOT, 'dump', 'anchor_check.png'))
print('wrote dump/anchor_check.png')
