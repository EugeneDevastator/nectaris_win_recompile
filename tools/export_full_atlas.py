"""Export the FULL contiguous combat/map sprite banks as one huge atlas.

The battle bank is 1 MB = 1024 tiles of 32x32 (32 x 32 grid).
Per-tile vertical flip is applied (the bank is bottom-up).
"""
import os, json
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
pal = json.load(open(os.path.join(ROOT, 'assets', 'art', 'palette.json')))
PAL = [(p['r'], p['g'], p['b']) for p in pal]
OUT = os.path.join(ROOT, 'assets', 'art', 'combat_atlas')
os.makedirs(OUT, exist_ok=True)

def atlas(data, bank, name, cols=32, cell=32, x2=True, grid=False):
    rows = (len(data) // 1024 + cols - 1) // cols
    img = Image.new('RGBA', (cols * cell, rows * cell), (0, 0, 0, 0))
    px = img.load()
    for t in range(len(data) // 1024):
        tx, ty = (t % cols) * cell, (t // cols) * cell
        off = t * 1024
        for y in range(32):
            for x in range(32):
                v = data[off + y * 32 + x]
                if v:
                    px[tx + x, ty + (31 - y)] = PAL[((bank << 4) | (v & 0xF)) & 0xFF] + (255,)
    bg = Image.new('RGB', img.size, (10, 10, 18))
    bg.paste(img, (0, 0), img)
    bg.save(os.path.join(OUT, name))
    print('wrote', name, bg.size)
    if x2:
        bg.resize((bg.width * 2, bg.height * 2), Image.NEAREST).save(
            os.path.join(OUT, name.replace('.png', '_x2.png')))
    return bg

battle = open(os.path.join(ROOT, 'dump', 'spritebank_battle.bin'), 'rb').read()
mapb = open(os.path.join(ROOT, 'dump', 'spritebank.bin'), 'rb').read()

atlas(battle, 9, 'battle_atlas_blue.png')
atlas(battle, 10, 'battle_atlas_green.png')
atlas(battle, 6, 'battle_atlas_orange.png')
atlas(mapb, 9, 'map_atlas_blue.png')
atlas(mapb, 10, 'map_atlas_green.png')

# labelled grid over the battle bank (2x, labels every tile)
w = 2048
img = Image.new('RGB', (w, w), (10, 10, 18))
for t in range(1024):
    tx, ty = (t % 32) * 64, (t // 32) * 64
    off = t * 1024
    for y in range(32):
        for x in range(32):
            v = battle[off + y * 32 + x]
            if v:
                img.putpixel((tx + x * 2, ty + (31 - y) * 2),
                             PAL[((9 << 4) | (v & 0xF)) & 0xFF])
                img.putpixel((tx + x * 2 + 1, ty + (31 - y) * 2),
                             PAL[((9 << 4) | (v & 0xF)) & 0xFF])
                img.putpixel((tx + x * 2, ty + (31 - y) * 2 + 1),
                             PAL[((9 << 4) | (v & 0xF)) & 0xFF])
                img.putpixel((tx + x * 2 + 1, ty + (31 - y) * 2 + 1),
                             PAL[((9 << 4) | (v & 0xF)) & 0xFF])
d = ImageDraw.Draw(img)
for i in range(33):
    d.line([(i * 64, 0), (i * 64, w)], fill=(40, 40, 60))
    d.line([(0, i * 64), (w, i * 64)], fill=(40, 40, 60))
for t in range(0, 1024, 32):
    d.text(((t % 32) * 64 + 2, (t // 32) * 64 + 2), str(t), fill=(255, 255, 0))
img.save(os.path.join(OUT, 'battle_atlas_grid.png'))
print('wrote battle_atlas_grid.png', img.size)
print('raw bank: dump/spritebank_battle.bin (1 MB, 1024 tiles)')
