import os, json
from PIL import Image, ImageDraw
ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
b = open(os.path.join(ROOT, 'dump', 'spritebank.bin'), 'rb').read()
pal = json.load(open(os.path.join(ROOT, 'assets', 'art', 'palette.json')))
PAL = [(p['r'], p['g'], p['b']) for p in pal]

def t8(idx, bank, flip):
    im = Image.new('RGBA', (32, 32), (0, 0, 0, 0)); px = im.load(); off = idx * 1024
    for y in range(32):
        for x in range(32):
            v = b[off + y * 32 + x]
            if v:
                px[x, 31 - y if flip else y] = PAL[((bank << 4) | (v & 0xF)) & 0xFF] + (255,)
    return im

out = Image.new('RGB', (8 * 100, 8 * 108), (10, 10, 18))
d = ImageDraw.Draw(out)
for i, idx in enumerate(range(160, 192)):
    for row, flip in enumerate((False, True)):
        im = t8(idx, 9, flip).resize((92, 92), Image.NEAREST)
        x = (i % 8) * 100
        y = (row * 4 + (i // 8)) * 108
        out.paste(im, (x + 4, y + 16), im)
        label = '%d %s' % (idx, 'flip' if flip else 'raw')
        d.text((x + 4, y), label, fill=(255, 255, 0))
out.save(os.path.join(ROOT, 'dump', 'row4_both.png'))
print('wrote dump/row4_both.png')
