"""Export the corrected (per-tile vertically flipped) sprite sheets."""
import os, json
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
OUT = os.path.join(ROOT, 'assets', 'art', 'combat_sprites')
os.makedirs(OUT, exist_ok=True)

sb = open(os.path.join(ROOT, 'dump', 'spritebank.bin'), 'rb').read()
pal = json.load(open(os.path.join(ROOT, 'assets', 'art', 'palette.json')))
PAL = [(p['r'], p['g'], p['b']) for p in pal]


def tile(idx, bank, flip=True):
    im = Image.new('RGBA', (32, 32), (0, 0, 0, 0))
    px = im.load(); off = idx * 1024
    if off + 1024 > len(sb):
        return im
    for y in range(32):
        for x in range(32):
            v = sb[off + y * 32 + x]
            if v:
                px[x, 31 - y if flip else y] = PAL[((bank << 4) | (v & 0xF)) & 0xFF] + (255,)
    return im


def sheet(indices, bank, name, cell=128, cols=8):
    rows = (len(indices) + cols - 1) // cols
    out = Image.new('RGB', (cols * cell, rows * (cell + 16)), (12, 12, 20))
    d = ImageDraw.Draw(out)
    for i, idx in enumerate(indices):
        t = tile(idx, bank).resize((cell - 4, cell - 4), Image.NEAREST)
        x = (i % cols) * cell; y = (i // cols) * (cell + 16)
        out.paste(t, (x + 2, y + 16), t)
        d.text((x + 2, y), str(idx), fill=(255, 255, 0))
    path = os.path.join(OUT, name)
    out.save(path)
    print('wrote', path, out.size)


combat = list(range(160, 192))
sheet(combat, 9, 'blue_battle_sprites_160_191.png')
sheet(combat, 10, 'green_battle_sprites_160_191.png')
sheet(list(range(192, 200)), 6, 'effects_orange_192_199.png')

# individual combat sprites, both teams, with alpha
for base, bank, tag in ((160, 9, 'blue'), (160, 10, 'green')):
    for i in range(32):
        idx = base + i
        tile(idx, bank).save(os.path.join(OUT, f'{tag}_{idx:03d}.png'))
print('individual sprites written')
