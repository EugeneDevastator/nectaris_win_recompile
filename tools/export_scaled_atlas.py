"""Build a scaled-variants atlas from the decoded full-resolution unit frames.

The original data stores only the full 64x64 frame; the combat simulation scales
it at draw time. This contact sheet shows each unit's first frame at the likely
in-game size ladder (nearest-neighbour, combat-blue palette), so the variants can
be used directly by the port without runtime stretching.

Output: assets/art/unit_frames/unit_frames_atlas_scaled_combat_blue.png
"""
import os, glob, re, json
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
SRC = os.path.join(ROOT, 'assets', 'art', 'unit_frames')
OUT = os.path.join(SRC, 'unit_frames_atlas_scaled_combat_blue.png')
units = json.load(open(os.path.join(ROOT, 'assets', 'data', 'units.json'), encoding='utf-8'))['units']
names = {u['id']: u['name'] for u in units}

SCALES = [(16, '16'), (32, '32'), (64, '64'), (128, '128')]
col_w = 150
row_h = 140
head = 24
sheet = Image.new('RGB', (len(SCALES) * col_w + 70, 23 * row_h + head), (70, 70, 70))
d = ImageDraw.Draw(sheet)

for c, (size, lab) in enumerate(SCALES):
    d.text((70 + c * col_w + 40, 6), '%dpx' % size, fill=(0, 0, 0))

for t in range(23):
    files = sorted(glob.glob(os.path.join(SRC, 'unit_%02d_frame_*_combat_blue.png' % t)),
                   key=lambda p: int(re.search(r'frame_(\d+)', p).group(1)))
    d.text((5, head + t * row_h + 50), '%2d %s' % (t, names.get(t, '?')[:7]), fill=(0, 0, 0))
    if not files:
        continue
    im = Image.open(files[0]).convert('RGBA')
    for c, (size, lab) in enumerate(SCALES):
        # preserve aspect ratio of the cropped frame
        h = size
        w = max(1, round(im.width * size / im.height))
        big = im.resize((w, h), Image.NEAREST)
        x = 70 + c * col_w + (col_w - w) // 2
        y = head + t * row_h + (row_h - h) // 2
        sheet.paste(big, (x, y), big)

sheet.save(OUT)
print('wrote', OUT, sheet.size)
