import os, json, itertools
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
b = open(os.path.join(ROOT, 'dump', 'spritebank_battle.bin'), 'rb').read()
pal = json.load(open(os.path.join(ROOT, 'assets', 'art', 'palette.json')))
PAL = [(p['r'], p['g'], p['b']) for p in pal]
BASE = 688
BANK = 7

def tile(idx, flip):
    im = Image.new('RGBA', (32, 32), (0, 0, 0, 0)); px = im.load(); off = idx * 1024
    for y in range(32):
        for x in range(32):
            v = b[off + y * 32 + x]
            if v:
                px[x, 31 - y if flip else y] = PAL[((BANK << 4) | (v & 0xF)) & 0xFF] + (255,)
    return im

quads = [(0, 0), (32, 0), (0, 32), (32, 32)]  # TL TR BL BR
out = Image.new('RGB', (8 * 100, 12 * 112), (10, 10, 16))
d = ImageDraw.Draw(out)
i = 0
for flip in (True, False):
    chunks = [tile(BASE + k, flip) for k in range(4)]
    for perm in itertools.permutations(range(4)):
        im = Image.new('RGBA', (64, 64), (0, 0, 0, 0))
        for q, k in enumerate(perm):
            im.alpha_composite(chunks[k], quads[q])
        im = im.resize((92, 92), Image.NEAREST)
        x = (i % 8) * 100; y = (i // 8) * 112
        out.paste(im, (x + 4, y + 18), im)
        d.text((x + 2, y), '%s%s' % (''.join(map(str, perm)), '' if flip else 'N'), fill=(255, 255, 0))
        i += 1
out.save(os.path.join(ROOT, 'dump', 'person_permutations.png'))
print('wrote dump/person_permutations.png')
