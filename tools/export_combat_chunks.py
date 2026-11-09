"""Export every slot of a combat block as individual PNGs, marking map-sprite groups."""
import os, sys, json
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
STATE = sys.argv[1] if len(sys.argv) > 1 else os.path.join(ROOT, 'dump', 'attack2', 'capture_877269c4')
OUT = os.path.join(ROOT, 'assets', 'art', 'combat_chunks', os.path.basename(STATE.rstrip('/\\')))
os.makedirs(OUT, exist_ok=True)
b = open(os.path.join(STATE, 'block.bin'), 'rb').read()
pal = open(os.path.join(ROOT, 'dump', 'battle_sprites', 'palette.bin'), 'rb').read()
PAL = [(pal[4 + i * 4], pal[5 + i * 4], pal[6 + i * 4]) for i in range(256)]
# groups the user identified as inserted map sprites for this capture
MAP_GROUPS = [(136, 139), (148, 151)]


def sprite(k):
    im = Image.new('RGBA', (32, 32), (0, 0, 0, 0))
    px = im.load()
    for y in range(32):
        for x in range(32):
            v = b[k * 1024 + y * 32 + x]
            if v:
                px[x, 31 - y] = PAL[((9 << 4) | (v & 15)) & 255] + (255,)
    return im


def is_map(slot):
    return any(lo <= slot <= hi for lo, hi in MAP_GROUPS)


cells = []
for k in range(32):
    slot = 128 + k
    im = sprite(k)
    kind = 'map' if is_map(slot) else 'sim'
    im.save(os.path.join(OUT, 'slot_%03d_%s.png' % (slot, kind)))
    cells.append((slot, kind, im))

# labeled atlas
cols = 8
cell = 80
sheet = Image.new('RGB', (cols * cell, 4 * cell + 20), (70, 70, 70))
d = ImageDraw.Draw(sheet)
for i, (slot, kind, im) in enumerate(cells):
    x = (i % cols) * cell + 8
    y = (i // cols) * cell + 20
    big = im.resize((64, 64), Image.NEAREST)
    sheet.paste(big, (x, y), big)
    d.text((x, y - 13), '%d %s' % (slot, kind), fill=(0, 0, 0) if kind == 'map' else (140, 0, 0))
sheet.save(os.path.join(OUT, 'atlas.png'))
json.dump([{'slot': s, 'kind': k, 'file': 'slot_%03d_%s.png' % (s, k)} for s, k, _ in cells],
          open(os.path.join(OUT, 'manifest.json'), 'w'), indent=1)
print('wrote', OUT, len(cells), 'slots')
