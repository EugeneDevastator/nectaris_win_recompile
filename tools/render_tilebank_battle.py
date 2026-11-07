import os, json
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
t = open(os.path.join(ROOT, 'dump', 'tilebank_battle.bin'), 'rb').read()
pal = json.load(open(os.path.join(ROOT, 'assets', 'art', 'palette.json')))
PAL = [(p['r'], p['g'], p['b']) for p in pal]

def tile16(idx, bank):
    im = Image.new('RGBA', (16, 16), (0, 0, 0, 0))
    px = im.load(); off = idx * 256
    for y in range(16):
        for x in range(16):
            v = t[off + y * 16 + x]
            if v:
                px[x, y] = PAL[((bank << 4) | (v & 0xF)) & 0xFF] + (255,)
    return im

for bank, name in ((9, 'tilebank_battle_b9.png'), (6, 'tilebank_battle_b6.png')):
    cols = 64; cell = 18
    out = Image.new('RGB', (cols * cell, 32 * cell), (12, 12, 20))
    for idx in range(2048):
        im = tile16(idx, bank)
        x = (idx % cols) * cell; y = (idx // cols) * cell
        out.paste(im, (x + 1, y + 1), im)
    out.save(os.path.join(ROOT, 'dump', name))
    print('wrote', name, out.size)
