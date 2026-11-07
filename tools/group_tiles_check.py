import os, json
from PIL import Image, ImageDraw
ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
b = open(os.path.join(ROOT, 'dump', 'spritebank.bin'), 'rb').read()
pal = json.load(open(os.path.join(ROOT, 'assets', 'art', 'palette.json')))
PAL = [(p['r'], p['g'], p['b']) for p in pal]

def t8(idx, bank=9):
    if idx * 1024 + 1024 > len(b):
        return Image.new('RGBA', (32, 32), (0, 0, 0, 0))
    im = Image.new('RGBA', (32, 32), (0, 0, 0, 0)); px = im.load(); off = idx * 1024
    for y in range(32):
        for x in range(32):
            v = b[off + y * 32 + x]
            if v:
                px[x, 31 - y] = PAL[((bank << 4) | (v & 0xF)) & 0xFF] + (255,)
    return im

# type 2 group sprites were 2,69,68,69,2,69; type 7 were 7,39,37,38,39,7
items = []
for label, base in (('t2 combat', 162), ('t2 s69+576', 645), ('t2 s68+576', 644),
                    ('t7 combat', 167), ('t7 s39+576', 615), ('t7 s37+576', 613),
                    ('t7 s38+576', 614)):
    items.append((label, base))
out = Image.new('RGB', (len(items) * 140, 170), (10, 10, 18))
d = ImageDraw.Draw(out)
for i, (label, idx) in enumerate(items):
    im = t8(idx).resize((128, 128), Image.NEAREST)
    out.paste(im, (i * 140 + 6, 30), im)
    d.text((i * 140 + 4, 8), label, fill=(255, 255, 0))
out.save(os.path.join(ROOT, 'dump', 'group_tiles_check.png'))
print('wrote dump/group_tiles_check.png')
