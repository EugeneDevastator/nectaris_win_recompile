"""Export combat VFX in the game's native organization.

Resource 23 contains 64 x 32x32 cells (4bpp, fire palette bank 13).

Live combat queues show two distinct groups:

  cells  0..31 : single-cell effects - projectiles, muzzle flashes, hit/miss
                 flashes and small bursts.  These are drawn 1:1 as 32x32.
  cells 32..63 : eight 64x64 frames, each composed from four consecutive
                 cells in row-major order (TL, TR, BL, BR).  These are the
                 double-size explosion/debris animation.

The old `vfx_00..15` exports composed cells in the order
[(32,0), (0,0), (32,32), (0,32)] - a 50% roll along U - which is why they did
not match `vfx_quad_32`.  The legacy, pair, sheet and contact exports are now
removed.

Outputs (assets/, release/ and port/build mirrors):
  small/vfx_small_00..31.png   32x32 single-cell effects
  big/vfx_big_32..52.png       64x64 explosion frames: 32,36,40,48,52
  packed/vfx_packed_44.png     64x64 sheet of four 32x32 sprites (miss;
                               order BR,TR,TL,BL = 47,45,44,46)
  packed/vfx_packed_56.png     particle sheet (4 sprites packed 2x2)
  packed/vfx_packed_60.png     shard sheet (4 sprites packed 2x2)
  vfx_atlas.png                640x512 reference atlas: big row, packed row,
                               small 8x4 grid below (all at 2x)
  vfx_atlas.json               frame rectangles
  manifest.json                roles + death/miss sequence
  effect_192..199.png          named 32x32 effects used by the port
"""
import json
import os
import shutil

from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
RAW = os.path.join(ROOT, 'dump', 'art_resources', 'res_23_sprites.bin')
PALFILE = os.path.join(ROOT, 'dump', 'attack2', 'palette_explosion_02.bin')
OUTS = [os.path.join(ROOT, 'assets', 'art', 'combat_effects'),
        os.path.join(ROOT, 'release', 'assets', 'art', 'combat_effects'),
        os.path.join(ROOT, 'port', 'build', 'assets', 'art', 'combat_effects')]
BANK = 13
N = 64
SMALL_COUNT = 32
# True double-size frames used by the killed-unit explosion.
BIG_STARTS = [32, 36, 40, 48, 52]
# 64x64 groups that are actually four separate 32x32 sprites packed 2x2.
# 44 is the miss burst in the live queues; 56/60 are other particle sheets.
PACKED_STARTS = [44, 56, 60]

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


sprites = [sprite(t) for t in range(N)]


def quad(start):
    im = Image.new('RGBA', (64, 64), (0, 0, 0, 0))
    for j, dst in enumerate([(0, 0), (32, 0), (0, 32), (32, 32)]):
        im.alpha_composite(sprites[start + j], dst)
    return im


for d in OUTS:
    small_dir = os.path.join(d, 'small')
    big_dir = os.path.join(d, 'big')
    packed_dir = os.path.join(d, 'packed')
    os.makedirs(small_dir, exist_ok=True)
    os.makedirs(big_dir, exist_ok=True)
    os.makedirs(packed_dir, exist_ok=True)
    for sub in (small_dir, big_dir, packed_dir):
        for f in os.listdir(sub):
            os.remove(os.path.join(sub, f))
    # Drop the old mixed/rolled exports.
    keep = {'manifest.json', 'vfx_atlas.png', 'vfx_atlas.json', 'splat.png',
            'palette_fire.bin'}
    for f in os.listdir(d):
        if os.path.isdir(os.path.join(d, f)):
            continue
        if f.startswith('effect_') or f in keep:
            continue
        if f.startswith(('vfx_', 'strip')):
            os.remove(os.path.join(d, f))

for i in range(SMALL_COUNT):
    for d in OUTS:
        sprites[i].save(os.path.join(d, 'small', 'vfx_small_%02d.png' % i))
for start in BIG_STARTS:
    for d in OUTS:
        quad(start).save(os.path.join(d, 'big', 'vfx_big_%02d.png' % start))
for start in PACKED_STARTS:
    for d in OUTS:
        quad(start).save(os.path.join(d, 'packed', 'vfx_packed_%02d.png' % start))

# Reference atlas at 2x: big row, packed row, then the small 8x4 grid.
ATLAS_W, ATLAS_H = 640, 512
atlas = Image.new('RGBA', (ATLAS_W, ATLAS_H), (0, 0, 0, 0))
big_rects, packed_rects, small_rects = [], [], []
for n, start in enumerate(BIG_STARTS):
    x = n * 128
    y = 0
    im = quad(start).resize((128, 128), Image.NEAREST)
    atlas.alpha_composite(im, (x, y))
    big_rects.append({'index': start, 'x': x, 'y': y, 'w': 128, 'h': 128})
for n, start in enumerate(PACKED_STARTS):
    x = n * 128
    y = 128
    im = quad(start).resize((128, 128), Image.NEAREST)
    atlas.alpha_composite(im, (x, y))
    packed_rects.append({'index': start, 'x': x, 'y': y, 'w': 128, 'h': 128})
for i in range(SMALL_COUNT):
    x = (i % 8) * 64
    y = 256 + (i // 8) * 64
    im = sprites[i].resize((64, 64), Image.NEAREST)
    atlas.alpha_composite(im, (x, y))
    small_rects.append({'index': i, 'x': x, 'y': y, 'w': 64, 'h': 64})

for d in OUTS:
    atlas.save(os.path.join(d, 'vfx_atlas.png'))
json.dump({
    'image': 'vfx_atlas.png',
    'layout': '640x512 at 2x; big row, packed row, small 8x4 grid below',
    'big': big_rects,
    'packed': packed_rects,
    'small': small_rects,
}, open(os.path.join(OUTS[0], 'vfx_atlas.json'), 'w'), indent=1)

# Named effects used by the port (32x32 source cells).
named = {192: 33, 193: 16, 194: 25, 195: 0, 196: 34, 197: 27, 198: 45, 199: 55}
for n, t in named.items():
    for d in OUTS:
        sprites[t].save(os.path.join(d, 'effect_%d.png' % n))

json.dump({
    'source': 'Nec.exe resource 23 (64 x 32x32 cells, live combat fire palette)',
    'palette': 'dump/attack2/palette_explosion_02.bin (bank 13)',
    'bank': BANK,
    'transparent_index': 0,
    'organization': {
        'small': {
            'range': [0, 31],
            'cell': [32, 32],
            'note': 'single-cell projectiles, muzzle flashes, hit/miss and small bursts',
        },
        'big': {
            'starts': BIG_STARTS,
            'cell': [64, 64],
            'note': 'true double-size frames; TL,TR,BL,BR from four consecutive cells',
        },
        'packed': {
            'starts': PACKED_STARTS,
            'cell': [64, 64],
            'note': 'four separate 32x32 sprites packed in a 2x2 square',
        },
    },
    'roles': {
        'death_explosion': [32, 36, 40],
        'miss_sheet': 44,
        'miss_order': [47, 45, 44, 46],
        'other_big': [48, 52],
        'packed_sheets': PACKED_STARTS,
    },
    'frames': {'small': SMALL_COUNT, 'big': len(BIG_STARTS),
               'packed': len(PACKED_STARTS)},
    'named_effects': named,
}, open(os.path.join(OUTS[0], 'manifest.json'), 'w'), indent=1)

print('wrote %d small cells, %d big, %d packed, atlas %dx%d' %
      (SMALL_COUNT, len(BIG_STARTS), len(PACKED_STARTS), ATLAS_W, ATLAS_H))
