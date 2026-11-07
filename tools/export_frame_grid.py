"""Export the full battle bank as a grid of 64x64 frames (each = 4 consecutive
32x32 chunks in UR/UL/BR/BL order). 1024 tiles = 256 frames.
"""
import os, json
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
b = open(os.path.join(ROOT, 'dump', 'spritebank_battle.bin'), 'rb').read()
pal = json.load(open(os.path.join(ROOT, 'assets', 'art', 'palette.json')))
PAL = [(p['r'], p['g'], p['b']) for p in pal]
OUT = os.path.join(ROOT, 'assets', 'art', 'combat_atlas')

def tile(idx, bank):
    im = Image.new('RGBA', (32, 32), (0, 0, 0, 0)); px = im.load(); off = idx * 1024
    for y in range(32):
        for x in range(32):
            v = b[off + y * 32 + x]
            if v:
                px[x, 31 - y] = PAL[((bank << 4) | (v & 0xF)) & 0xFF] + (255,)
    return im

def frame(gi, bank):
    im = Image.new('RGBA', (64, 64), (0, 0, 0, 0))
    for k, dst in enumerate([(32, 0), (0, 0), (32, 32), (0, 32)]):
        t = tile(gi * 4 + k, bank)
        im.paste(t, dst, t)
    return im

def grid(bank, name, cols=16, cell=64, label=True):
    n = 256
    rows = (n + cols - 1) // cols
    out = Image.new('RGB', (cols * cell, rows * (cell + (12 if label else 0))), (10, 10, 18))
    d = ImageDraw.Draw(out)
    for gi in range(n):
        im = frame(gi, bank)
        x = (gi % cols) * cell
        y = (gi // cols) * (cell + (12 if label else 0))
        out.paste(im, (x, y + (12 if label else 0)), im)
        if label:
            d.text((x + 1, y), str(gi * 4), fill=(255, 255, 0))
    out.save(os.path.join(OUT, name))
    print('wrote', name, out.size)
    return out

for bank, name in ((9, 'battle_frames_blue.png'), (10, 'battle_frames_green.png'),
                   (6, 'battle_frames_orange.png')):
    o = grid(bank, name)
    o.resize((o.width * 2, o.height * 2), Image.NEAREST).save(
        os.path.join(OUT, name.replace('.png', '_x2.png')))
print('raw contiguous bank: dump/spritebank_battle.bin')
