import os, json
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
a = open(os.path.join(ROOT, 'dump', 'spritebank.bin'), 'rb').read()
b = open(os.path.join(ROOT, 'dump', 'spritebank_battle.bin'), 'rb').read()
print('map bank', len(a), 'battle bank', len(b))
diffs = sum(1 for x, y in zip(a, b) if x != y)
print('differing bytes', diffs)
# first differing tile
for i in range(len(a)//1024):
    if a[i*1024:(i+1)*1024] != b[i*1024:(i+1)*1024]:
        print('first differing tile', i)
        break

pal = json.load(open(os.path.join(ROOT, 'assets', 'art', 'palette.json')))
PAL = [(p['r'], p['g'], p['b']) for p in pal]

def tile(data, idx, bank, flip=True):
    img = Image.new('RGBA', (32, 32), (0, 0, 0, 0))
    px = img.load(); off = idx * 1024
    for y in range(32):
        for x in range(32):
            v = data[off + y * 32 + x]
            if v:
                px[x, 31 - y if flip else y] = PAL[((bank << 4) | (v & 0xF)) & 0xFF] + (255,)
    return img

def sheet(data, bank, name, cols=16, cell=44, lim=320):
    rows = (lim + cols - 1) // cols
    out = Image.new('RGB', (cols * cell, rows * (cell + 12)), (12, 12, 20))
    d = ImageDraw.Draw(out)
    nonzero = 0
    for idx in range(lim):
        t = tile(data, idx, bank).resize((cell - 6, cell - 6), Image.NEAREST)
        if any(t.getdata()):
            nonzero += 1
        x = (idx % cols) * cell; y = (idx // cols) * (cell + 12)
        out.paste(t, (x + 3, y + 12), t)
        d.text((x + 1, y), str(idx), fill=(255, 255, 0))
    out.save(os.path.join(ROOT, 'dump', name))
    print('wrote', name, out.size, 'nonzero', nonzero)

sheet(b, 9, 'battle_bank9_flip.png')
sheet(b, 6, 'battle_bank6_flip.png')
sheet(b, 10, 'battle_bank10_flip.png')
