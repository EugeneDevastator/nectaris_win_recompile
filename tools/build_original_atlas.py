"""Export the original combat sprite banks as one big unstitched atlas.

Sources are the static resource pages (not the runtime per-battle cache):
  res13: 64 sprites = 16 units x 4 rotations
  res14: 24 sprites =  6 units x 4 rotations
  res21: 43 sprites (possible extra units)
Arranged 1:1 in a 32x32 grid with indices, no quadrant stitching.
"""
import json, os, sys
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
D = os.path.join(ROOT, 'dump', 'art_resources')
OUT = os.path.join(ROOT, 'assets', 'art', 'combat_bank_original')
os.makedirs(OUT, exist_ok=True)
M = {r['id']: r for r in json.load(open(os.path.join(D, 'manifest.json')))['resources']}
pal = open(os.path.join(ROOT, 'dump', 'battle_sprites', 'palette.bin'), 'rb').read()
PAL = [(pal[4 + i * 4], pal[5 + i * 4], pal[6 + i * 4]) for i in range(256)]
units = json.load(open(os.path.join(ROOT, 'assets', 'data', 'units.json'), encoding='utf-8'))['units']
names = {u['id']: u['name'] for u in units}


def load(rid):
    return open(os.path.join(D, M[rid]['expanded']), 'rb').read()


def sprite(raw, t, bank=9):
    im = Image.new('RGBA', (32, 32), (0, 0, 0, 0))
    px = im.load()
    for y in range(32):
        for x in range(32):
            v = raw[t * 1024 + y * 32 + x]
            if v:
                px[x, 31 - y] = PAL[((bank << 4) | (v & 15)) & 255] + (255,)
    return im


def atlas_for(rid, out_name, bank=9, cols=16, label=True):
    raw = load(rid)
    n = min(M[rid]['units'], len(raw) // 1024)
    nz = [t for t in range(n) if any(raw[t * 1024:(t + 1) * 1024])]
    cols = cols
    cell = 32
    pad = 1
    rows = (n + cols - 1) // cols
    label_h = 12 if label else 0
    sheet = Image.new('RGB', (cols * (cell + pad) + 1, rows * (cell + pad) + label_h + 1), (18, 18, 26))
    d = ImageDraw.Draw(sheet)
    for t in range(n):
        im = sprite(raw, t, bank)
        x = (t % cols) * (cell + pad) + 1
        y = (t // cols) * (cell + pad) + label_h + 1
        sheet.paste(im, (x, y), im)
        if label:
            d.text((x, y - 11), str(t), fill=(200, 200, 100))
    path = os.path.join(OUT, out_name)
    sheet.save(path)
    print('wrote', path, sheet.size, 'sprites', n, 'nonzero', len(nz))
    return {'resource': rid, 'sprites': n, 'nonzero': nz, 'file': out_name}


# combined atlas: res13 then res14 then res21 stacked, 16 columns
blocks = []
for rid, name in ((13, 'bank_res13'), (14, 'bank_res14'), (21, 'bank_res21')):
    blocks.append(atlas_for(rid, name + '.png', bank=9, cols=16))

# merged: draw each bank in its own labelled band
raw_parts = []
for rid, band in ((13, 'res13 (units 0..15 x4 rot)'), (14, 'res14 (units 16..21 x4 rot)'), (21, 'res21 (extra)')):
    raw = load(rid)
    n = min(M[rid]['units'], len(raw) // 1024)
    raw_parts.append((band, n, raw))

cols = 16
cell = 32
pad = 1
label_h = 14
total_rows = sum((n + cols - 1) // cols for _, n, _ in raw_parts) + len(raw_parts)
sheet = Image.new('RGB', (cols * (cell + pad) + 1, total_rows * (cell + pad) + label_h * len(raw_parts) + 8), (18, 18, 26))
d = ImageDraw.Draw(sheet)
y = 4
r = 0
manifest = []
for band, n, raw in raw_parts:
    d.text((2, y), band, fill=(255, 230, 100))
    y += label_h
    base_row = r
    for t in range(n):
        im = sprite(raw, t, 9)
        x = (t % cols) * (cell + pad) + 1
        yy = y + (t // cols) * (cell + pad)
        sheet.paste(im, (x, yy), im)
        d.text((x, yy - 11), str(t), fill=(170, 170, 90))
    rows = (n + cols - 1) // cols
    y += rows * (cell + pad) + 2
    r += rows
    manifest.append({'band': band, 'sprites': n})
sheet.save(os.path.join(OUT, 'combat_bank_all.png'))
json.dump(manifest, open(os.path.join(OUT, 'manifest.json'), 'w'), indent=1)
print('wrote combat_bank_all.png', sheet.size)
