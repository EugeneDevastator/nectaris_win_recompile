import os, json
from PIL import Image, ImageDraw
ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
pal = json.load(open(os.path.join(ROOT, 'assets', 'art', 'palette.json')))
PAL = [(p['r'], p['g'], p['b']) for p in pal]
D = os.path.join(ROOT, 'dump', 'resources')

def tile(raw, idx, bank):
    im = Image.new('RGBA', (32, 32), (0, 0, 0, 0)); px = im.load(); off = idx * 1024
    if off + 1024 > len(raw): return None
    for y in range(32):
        for x in range(32):
            v = raw[off + y * 32 + x]
            if v: px[x, 31 - y] = PAL[((bank << 4) | (v & 0xF)) & 0xFF] + (255,)
    return im

files = sorted(os.listdir(D))
cols = 8
out = Image.new('RGB', (cols * 150, len(files) * 88), (12, 12, 20))
d = ImageDraw.Draw(out)
for row, f in enumerate(files):
    raw = open(os.path.join(D, f), 'rb').read()
    d.text((2, row * 88 + 2), f, fill=(255, 255, 0))
    for i in range(4):
        t = tile(raw, i, 9)
        if t:
            t = t.resize((72, 72), Image.NEAREST)
            out.paste(t, (i * 76 + 4, row * 88 + 12), t)
out.save(os.path.join(ROOT, 'dump', 'resources_sheet.png'))
print('wrote resources_sheet.png', out.size)
