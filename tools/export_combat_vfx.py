"""Export the combat-simulation VFX with the live combat fire palette.

Source art: resource 23 (expanded sprite bank res_23_sprites.bin, 64 x 32x32
4bpp sprites = 32 frames of 64x32, stored as left/right halves).

During firing the game loads a dedicated fire ramp into palette bank 13
(captured as dump/attack2/palette_explosion_02.bin):

    index 1 white, 2 yellow, 3 gold, 4 orange, 5-7 greys

so the sprites must be rendered with bank 13, not with a team bank.  Index 0 is
transparent; the whole ramp 1..7 is used.

Outputs:
  vfx_half_00..63.png     individual 32x32 halves
  vfx_pair_00..31.png     assembled 64x32 animation frames
  vfx_00..15.png          legacy 64x64 2x2 sheets (port's drawProjectile/drawExplosion)
  vfx_atlas.png           16x2 grid at 2x (same layout as vfx_sheet_x2), 2048x128
  vfx_atlas_1x.png        same layout at 1x, 1024x64
  vfx_sheet.png           32 pair frames in a row
  vfx_sheet_x2.png
  vfx_contact.png         8x4 contact sheet of pairs
  effect_192..199.png     single named effects used by the port
"""
import os, json
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
RAW = os.path.join(ROOT, 'dump', 'art_resources', 'res_23_sprites.bin')
PALFILE = os.path.join(ROOT, 'dump', 'attack2', 'palette_explosion_02.bin')
OUTS = [os.path.join(ROOT, 'assets', 'art', 'combat_effects'),
        os.path.join(ROOT, 'release', 'assets', 'art', 'combat_effects'),
        os.path.join(ROOT, 'port', 'build', 'assets', 'art', 'combat_effects')]
BANK = 13
N = 64

raw = open(RAW, 'rb').read()
pal = open(PALFILE, 'rb').read()
PAL = [(pal[4 + i * 4], pal[5 + i * 4], pal[6 + i * 4]) for i in range(256)]


def sprite(t):
    im = Image.new('RGBA', (32, 32), (0, 0, 0, 0))
    px = im.load()
    b = t * 1024
    for y in range(32):
        for x in range(32):
            v = raw[b + y * 32 + x]
            if v == 0:
                continue
            px[x, 31 - y] = PAL[((BANK << 4) | (v & 0xF)) & 0xFF] + (255,)
    return im


def pair(k):
    im = Image.new('RGBA', (64, 32), (0, 0, 0, 0))
    im.alpha_composite(sprite(2 * k), (0, 0))
    im.alpha_composite(sprite(2 * k + 1), (32, 0))
    return im


sprites = [sprite(t) for t in range(N)]
pairs = [pair(k) for k in range(N // 2)]

for d in OUTS:
    os.makedirs(d, exist_ok=True)
for i, im in enumerate(sprites):
    for d in OUTS:
        im.save(os.path.join(d, 'vfx_half_%02d.png' % i))
for k, im in enumerate(pairs):
    for d in OUTS:
        im.save(os.path.join(d, 'vfx_pair_%02d.png' % k))

# 64x64 explosion cells: four consecutive 32x32 halves in row-major order
# (TL, TR, BL, BR; verified against a live combat frame).
for start in range(0, N, 4):
    quad = Image.new('RGBA', (64, 64), (0, 0, 0, 0))
    for j, dst in enumerate([(0, 0), (32, 0), (0, 32), (32, 32)]):
        quad.alpha_composite(sprites[start + j], dst)
    for d in OUTS:
        quad.save(os.path.join(d, 'vfx_quad_%02d.png' % start))

# legacy 64x64 2x2 sheets, four sprites each, same layout the port already uses
for i in range(16):
    sheet2 = Image.new('RGBA', (64, 64), (0, 0, 0, 0))
    for k, dst in enumerate([(32, 0), (0, 0), (32, 32), (0, 32)]):
        t = i * 4 + k
        if t < N:
            sheet2.alpha_composite(sprites[t], dst)
    for d in OUTS:
        sheet2.save(os.path.join(d, 'vfx_%02d.png' % i))

def make_grid(scale):
    """16 columns x 2 rows of 64x32 pair frames, on a 32px cell grid."""
    cols = 16
    cw, ch = 64 * scale, 32 * scale
    rows = (len(pairs) + cols - 1) // cols
    im = Image.new('RGBA', (cols * cw, rows * ch), (0, 0, 0, 0))
    frames = []
    for k, p in enumerate(pairs):
        big = p if scale == 1 else p.resize((cw, ch), Image.NEAREST)
        x = (k % cols) * cw
        y = (k // cols) * ch
        im.alpha_composite(big, (x, y))
        frames.append({'index': k, 'x': x, 'y': y, 'w': cw, 'h': ch})
    return im, frames


sheet1, frames1 = make_grid(1)
sheet2, frames2 = make_grid(2)
for d in OUTS:
    sheet1.save(os.path.join(d, 'vfx_sheet.png'))
    sheet2.save(os.path.join(d, 'vfx_sheet_x2.png'))
    sheet2.save(os.path.join(d, 'vfx_atlas.png'))
    sheet1.save(os.path.join(d, 'vfx_atlas_1x.png'))
json.dump({'image': 'vfx_atlas.png',
           'layout': '16 columns x 2 rows at 2x (same layout as vfx_sheet_x2)',
           'cell': [128, 64], 'palette': 'combat fire (bank 13)',
           'transparent_index': 0, 'frames': frames2},
          open(os.path.join(OUTS[0], 'vfx_atlas.json'), 'w'), indent=1)
json.dump({'image': 'vfx_atlas_1x.png', 'layout': '16 columns x 2 rows at 1x',
           'cell': [64, 32], 'frames': frames1},
          open(os.path.join(OUTS[0], 'vfx_atlas_1x.json'), 'w'), indent=1)

cols = 8
cw, ch = 140, 90
rows = (len(pairs) + cols - 1) // cols
sheet = Image.new('RGB', (cols * cw, rows * ch + 18), (10, 10, 20))
d = ImageDraw.Draw(sheet)
for k, im in enumerate(pairs):
    x = (k % cols) * cw + 4
    y = (k // cols) * ch + 16
    big = im.resize((128, 64), Image.NEAREST)
    sheet.paste(big, (x, y), big)
    d.text((x, y - 12), 'pair %02d' % k, fill=(220, 220, 220))
sheet.save(os.path.join(OUTS[0], 'vfx_contact.png'))

# named single effects used by the port (32x32 halves)
named = {192: 33, 193: 16, 194: 25, 195: 0, 196: 34, 197: 27, 198: 45, 199: 55}
for n, t in named.items():
    for d in OUTS:
        sprites[t].save(os.path.join(d, 'effect_%d.png' % n))

json.dump({'source': 'Nec.exe resource 23 (64 x 32x32 halves = 32 x 64x32 frames)',
           'palette': 'dump/attack2/palette_explosion_02.bin (live combat fire ramp)',
           'bank': BANK, 'transparent_index': 0,
           'frames': {'halves': N, 'pairs': len(pairs)},
           'named_effects': named},
          open(os.path.join(OUTS[0], 'manifest.json'), 'w'), indent=1)
print('wrote', N, 'halves,', len(pairs), 'pair frames to', OUTS[0])
