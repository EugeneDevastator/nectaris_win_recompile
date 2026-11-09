"""Union every full sprite-bank dump into one atlas of unique combat chunks.

Classifies each 32x32 slot: map sprite (exact match to res13/res14) or combat chunk.
Exports every unique non-map sprite with its first source.
"""
import os, glob, json, hashlib
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
D = os.path.join(ROOT, 'dump', 'art_resources')
OUT = os.path.join(ROOT, 'assets', 'art', 'combat_chunks', 'bank_union')
os.makedirs(OUT, exist_ok=True)
pal = open(os.path.join(ROOT, 'dump', 'battle_sprites', 'palette.bin'), 'rb').read()
PAL = [(pal[4 + i * 4], pal[5 + i * 4], pal[6 + i * 4]) for i in range(256)]

map_hashes = set()
for rid in (13, 14):
    raw = open(os.path.join(D, 'res_%02d_sprites.bin' % rid), 'rb').read()
    for t in range(len(raw) // 1024):
        blk = raw[t * 1024:(t + 1) * 1024]
        if any(blk):
            map_hashes.add(hashlib.md5(blk).hexdigest())
sbmap = open(os.path.join(ROOT, 'dump', 'spritebank_m6.bin'), 'rb').read()
for t in range(64, 128):
    blk = sbmap[t * 1024:(t + 1) * 1024]
    if any(blk):
        map_hashes.add(hashlib.md5(blk).hexdigest())

banks = []
for pat in ('dump/spritebank_*.bin', 'dump/*/spritebank.bin', 'dump/live_art/spritebank.bin'):
    banks += glob.glob(os.path.join(ROOT, pat))
banks = sorted(set(banks))
print('full banks', len(banks))

uniq = {}
for fn in banks:
    b = open(fn, 'rb').read()
    if len(b) < 1024 * 512:
        continue
    src = os.path.relpath(fn, ROOT)
    for s in range(512):
        blk = b[s * 1024:(s + 1) * 1024]
        if not any(blk):
            continue
        h = hashlib.md5(blk).hexdigest()
        if h in map_hashes:
            continue
        if h not in uniq:
            uniq[h] = (blk, s, src)

print('unique combat chunks', len(uniq))
items = sorted(uniq.items(), key=lambda kv: kv[1][1])
cols = 16
cell = 42
rows = (len(items) + cols - 1) // cols
sheet = Image.new('RGB', (cols * cell, rows * cell + 16), (70, 70, 70))
d = ImageDraw.Draw(sheet)
manifest = []
for i, (h, (blk, slot, src)) in enumerate(items):
    im = Image.new('RGBA', (32, 32), (0, 0, 0, 0))
    px = im.load()
    for y in range(32):
        for x in range(32):
            v = blk[y * 32 + x]
            if v:
                px[x, 31 - y] = PAL[((9 << 4) | (v & 15)) & 255] + (255,)
    fn = 'chunk_%03d_%s.png' % (i, h[:8])
    im.save(os.path.join(OUT, fn))
    x = (i % cols) * cell + 4
    y = (i // cols) * cell + 16
    sheet.paste(im, (x, y), im)
    d.text((x, y - 12), '%d' % i, fill=(10, 10, 10))
    manifest.append({'index': i, 'md5': h, 'first_slot': slot, 'source': src, 'file': fn})
sheet.save(os.path.join(OUT, 'bank_union_atlas.png'))
json.dump(manifest, open(os.path.join(OUT, 'manifest.json'), 'w'), indent=1)
print('wrote', OUT, 'chunks', len(items))
