import os, json
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
b = open(os.path.join(ROOT, 'dump', 'spritebank_battle.bin'), 'rb').read()
pal = json.load(open(os.path.join(ROOT, 'assets', 'art', 'palette.json')))
PAL = [(p['r'], p['g'], p['b']) for p in pal]

def tile(idx, bank):
    im = Image.new('RGBA', (32, 32), (0, 0, 0, 0)); px = im.load(); off = idx * 1024
    for y in range(32):
        for x in range(32):
            v = b[off + y * 32 + x]
            if v:
                px[x, 31 - y] = PAL[((bank << 4) | (v & 0xF)) & 0xFF] + (255,)
    return im

def compose(base, bank):
    im = Image.new('RGBA', (64, 64), (0, 0, 0, 0))
    for k, dst in enumerate([(32, 0), (0, 0), (32, 32), (0, 32)]):
        t = tile(base + k, bank)
        im.paste(t, dst, t)
    return im

# also print the palette entries for each bank (index 0..7)
for bank in range(16):
    print('bank', bank, [PAL[bank * 16 + i] for i in range(8)])

cell = 150
out = Image.new('RGB', (8 * cell, 2 * (cell + 20)), (12, 12, 20))
d = ImageDraw.Draw(out)
for bank in range(16):
    im = compose(688, bank).resize((cell - 6, cell - 6), Image.NEAREST)
    x = (bank % 8) * cell; y = (bank // 8) * (cell + 20)
    out.paste(im, (x + 3, y + 20), im)
    d.text((x + 3, y + 2), f'bank {bank}', fill=(255, 255, 0))
out.save(os.path.join(ROOT, 'dump', 'person_all_banks.png'))
print('wrote dump/person_all_banks.png')
