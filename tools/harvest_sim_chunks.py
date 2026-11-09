"""Harvest unique combat-sim sprite chunks across all captured combat blocks.

A slot is a map sprite if its 1024 bytes exactly match any res13/res14 sprite
(and the live map sprites). Everything else with data is a sim chunk.
"""
import os, glob, json, hashlib
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
D = os.path.join(ROOT, 'dump', 'art_resources')
OUT = os.path.join(ROOT, 'assets', 'art', 'combat_chunks', 'library')
os.makedirs(OUT, exist_ok=True)
pal = open(os.path.join(ROOT, 'dump', 'battle_sprites', 'palette.bin'), 'rb').read()
PAL = [(pal[4 + i * 4], pal[5 + i * 4], pal[6 + i * 4]) for i in range(256)]

# known map sprite set
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

# collect blocks
blocks = []
for pat in ('dump/attack2/*/block.bin', 'dump/forced_attacks/*/block.bin',
            'dump/type_captures/*/block.bin', 'dump/block_changes/*/block.bin'):
    blocks += glob.glob(os.path.join(ROOT, pat))
print('blocks', len(blocks))

uniq = {}
for fn in blocks:
    b = open(fn, 'rb').read()
    if len(b) < 32 * 1024:
        continue
    src = os.path.relpath(fn, ROOT)
    for k in range(32):
        blk = b[k * 1024:(k + 1) * 1024]
        if not any(blk):
            continue
        h = hashlib.md5(blk).hexdigest()
        if h in map_hashes:
            continue
        if h not in uniq:
            uniq[h] = (blk, 128 + k, src)

print('unique sim chunks', len(uniq))
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
    x = (i % cols) * cell + 4
    y = (i // cols) * cell + 16
    sheet.paste(im, (x, y), im)
    d.text((x, y - 12), '%d' % i, fill=(10, 10, 10))
    fn = 'chunk_%03d_%s.png' % (i, h[:8])
    im.save(os.path.join(OUT, fn))
    manifest.append({'index': i, 'md5': h, 'first_slot': slot, 'source': src, 'file': fn})
sheet.save(os.path.join(OUT, 'library_atlas.png'))
json.dump(manifest, open(os.path.join(OUT, 'manifest.json'), 'w'), indent=1)
print('wrote', OUT, 'chunks', len(items))
