"""Export the 32 chunk-pair 64x32 combat sprites from the clean block state."""
import os, json, sys
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
D = os.path.join(ROOT, 'dump', 'ai_turn2')
STATE = sys.argv[1] if len(sys.argv) > 1 else 'sb_156.bin'
OUT = os.path.join(ROOT, 'assets', 'art', 'combat_pairs')
os.makedirs(OUT, exist_ok=True)
b = open(os.path.join(D, STATE), 'rb').read()
pal = open(os.path.join(D, 'palette.bin'), 'rb').read()
PAL = [(pal[4 + i * 4], pal[5 + i * 4], pal[6 + i * 4]) for i in range(256)]


def tile(idx, bank):
    im = Image.new('RGBA', (32, 32), (0, 0, 0, 0))
    px = im.load()
    off = idx * 1024
    for y in range(32):
        for x in range(32):
            v = b[off + y * 32 + x]
            if v:
                px[x, 31 - y] = PAL[((bank << 4) | (v & 15)) & 255] + (255,)
    return im


manifest = []
for bank, tag in ((9, 'blue'), (10, 'green'), (11, 'red'), (12, 'gray')):
    for base, dtag in ((128, 'clean'), (192, 'damaged')):
        for k in range(32):
            slot = base + 2 * k
            im = Image.new('RGBA', (64, 32), (0, 0, 0, 0))
            im.alpha_composite(tile(slot, bank), (0, 0))
            im.alpha_composite(tile(slot + 1, bank), (32, 0))
            fn = 'pair_%02d_%s_%s.png' % (k, dtag, tag)
            im.save(os.path.join(OUT, fn))
            manifest.append({'pair': k, 'state': dtag, 'bank': bank, 'slot': slot, 'file': fn})
json.dump(manifest, open(os.path.join(OUT, 'manifest.json'), 'w'), indent=1)
print('exported', len(manifest))
