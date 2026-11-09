"""Compose and harvest unique combat-sim units (4-chunk groups) from all captures."""
import os, glob, json, hashlib
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
D = os.path.join(ROOT, 'dump', 'art_resources')
OUT = os.path.join(ROOT, 'assets', 'art', 'combat_chunks', 'units')
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

blocks = []
for pat in ('dump/attack2/*/block.bin', 'dump/forced_attacks/*/block.bin',
            'dump/type_captures/*/block.bin', 'dump/block_changes/*/block.bin'):
    blocks += glob.glob(os.path.join(ROOT, pat))

uniq = {}
for fn in blocks:
    b = open(fn, 'rb').read()
    if len(b) < 32 * 1024:
        continue
    src = os.path.relpath(fn, ROOT)
    for g in range(8):
        slots = [128 + g * 4 + j for j in range(4)]
        blks = [b[(g * 4 + j) * 1024:(g * 4 + j + 1) * 1024] for j in range(4)]
        if not any(any(x) for x in blks):
            continue
        hashes = [hashlib.md5(x).hexdigest() if any(x) else None for x in blks]
        if all(h is None or h in map_hashes for h in hashes):
            continue
        canvas = Image.new('RGBA', (64, 64), (0, 0, 0, 0))
        for j, blk in enumerate(blks):
            t = Image.new('RGBA', (32, 32), (0, 0, 0, 0))
            px = t.load()
            for y in range(32):
                for x in range(32):
                    v = blk[y * 32 + x]
                    if v:
                        px[x, 31 - y] = PAL[((9 << 4) | (v & 15)) & 255] + (255,)
            canvas.alpha_composite(t, ((j % 2) * 32, (j // 2) * 32))
        bb = canvas.getbbox()
        if not bb:
            continue
        canvas = canvas.crop(bb)
        key = hashlib.md5(canvas.tobytes()).hexdigest()
        if key not in uniq:
            uniq[key] = (canvas, slots[0], src)

print('unique composed units', len(uniq))
items = sorted(uniq.items(), key=lambda kv: kv[1][1])
cols = 10
cell = 90
rows = (len(items) + cols - 1) // cols
sheet = Image.new('RGB', (cols * cell, rows * cell + 16), (70, 70, 70))
d = ImageDraw.Draw(sheet)
manifest = []
for i, (key, (im, slot, src)) in enumerate(items):
    fn = 'unit_%03d_%s.png' % (i, key[:8])
    im.save(os.path.join(OUT, fn))
    big = im.resize((max(1, im.width * 2), max(1, im.height * 2)), Image.NEAREST)
    x = (i % cols) * cell + 6
    y = (i // cols) * cell + 18
    sheet.paste(big, (x, y), big)
    d.text((x, y - 12), '%d' % i, fill=(10, 10, 10))
    manifest.append({'index': i, 'slot': slot, 'source': src, 'file': fn,
                     'w': im.width, 'h': im.height})
sheet.save(os.path.join(OUT, 'units_atlas.png'))
json.dump(manifest, open(os.path.join(OUT, 'manifest.json'), 'w'), indent=1)
print('wrote', OUT, 'units', len(items))
