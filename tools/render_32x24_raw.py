"""Re-interpret raw 4bpp resource atlas slices as 32x24 sprites (DOS-era size)."""
import os, json
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
D = os.path.join(ROOT, 'dump', 'art_resources')
OUT = os.path.join(ROOT, 'dump', 'sprites_32x24')
os.makedirs(OUT, exist_ok=True)
M = json.load(open(os.path.join(D, 'manifest.json')))['resources']
pal = open(os.path.join(ROOT, 'dump', 'ai_turn2', 'palette.bin'), 'rb').read()
PAL = [(pal[4 + i * 4], pal[5 + i * 4], pal[6 + i * 4]) for i in range(256)]
BANK = 9

# 32x24 4bpp = 32/2 * 24 = 384 bytes; 8bpp = 32*24 = 768
CASES = [(384, '4bpp'), (768, '8bpp')]

summary = []
for r in M:
    fn = os.path.join(D, r['raw'])
    data = open(fn, 'rb').read()
    for stride, mode in CASES:
        if len(data) >= stride and len(data) % stride == 0:
            n = len(data) // stride
            if n < 2:
                continue
            summary.append((r['id'], r['type'], mode, n, fn, stride))

for rid, typ, mode, n, fn, stride in summary:
    data = open(fn, 'rb').read()
    cols = 8
    rows = (n + cols - 1) // cols
    cw, ch = 32 * 3, 24 * 3
    sheet = Image.new('RGB', (cols * (cw + 4), rows * (ch + 4) + 16), (18, 18, 26))
    d = ImageDraw.Draw(sheet)
    for t in range(n):
        base = t * stride
        sx = (t % cols) * (cw + 4) + 2
        sy = (t // cols) * (ch + 4) + 16
        for y in range(24):
            for x in range(32):
                if mode == '4bpp':
                    b = data[base + y * 16 + (x >> 1)]
                    v = (b >> 4) if (x & 1) == 0 else (b & 0xF)
                else:
                    v = data[base + y * 32 + x]
                if not v:
                    continue
                c = PAL[((BANK << 4) | (v & 15)) & 255]
                for yy in range(3):
                    for xx in range(3):
                        sheet.putpixel((sx + x * 3 + xx, sy + (23 - y) * 3 + yy), c)
        d.text((sx, sy - 12), '%d' % t, fill=(220, 220, 110))
    out = os.path.join(OUT, 'res_%02d_%s.png' % (rid, mode))
    sheet.save(out)
    print('wrote', out, sheet.size, 'sprites', n)

print('total candidates', len(summary))
