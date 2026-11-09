"""Scan dumped regions for 8bpp sprite-bank-like blocks (all bytes < 16)."""
import json, os
import numpy as np
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
B = os.path.join(ROOT, 'dump', 'banks')
idx = json.load(open(os.path.join(B, 'index.json')))
pal = open(os.path.join(B, 'palette.bin'), 'rb').read()
PAL = [(pal[4 + i * 4], pal[5 + i * 4], pal[6 + i * 4]) for i in range(256)]

BLOCK = 0x10000
cands = []
for e in idx:
    data = np.frombuffer(open(os.path.join(B, e['raw']), 'rb').read(), dtype=np.uint8)
    for off in range(0, len(data) - BLOCK + 1, BLOCK):
        blk = data[off:off + BLOCK]
        lo = float((blk < 16).mean())
        nz = float((blk != 0).mean())
        if lo > 0.98 and 0.01 < nz < 0.85:
            cands.append((e['address'], e['raw'], off, lo, nz))

print('candidate blocks', len(cands))
for c in cands[:80]:
    print(c[0], 'off', hex(c[2]), 'lo %.3f nz %.3f' % (c[3], c[4]))

# render each candidate as 32x32 cells, 16 cols, bank 9
os.makedirs(os.path.join(ROOT, 'dump', 'sprite_blocks'), exist_ok=True)
thumbs = []
for i, (addr, rawfn, off, lo, nz) in enumerate(cands):
    data = open(os.path.join(B, rawfn), 'rb').read()[off:off + BLOCK]
    cells = BLOCK // 1024
    scale = 1
    cellpx = 34
    sheet = Image.new('RGB', (16 * cellpx, (cells // 16) * cellpx), (18, 18, 26))
    for t in range(cells):
        sx = (t % 16) * cellpx + 1
        sy = (t // 16) * cellpx + 1
        for y in range(32):
            for x in range(32):
                v = data[t * 1024 + y * 32 + x]
                if v:
                    sheet.putpixel((sx + x, sy + (31 - y)), PAL[((9 << 4) | (v & 15)) & 255])
    fn = os.path.join(ROOT, 'dump', 'sprite_blocks', '%02d_%s_%06x.png' % (i, addr, off))
    sheet.save(fn)
    thumbs.append((fn, addr, off, lo, nz))

cols = 6
rows = (len(thumbs) + cols - 1) // cols
idxsheet = Image.new('RGB', (cols * 560, rows * 330), (16, 16, 24))
d = ImageDraw.Draw(idxsheet)
for i, (fn, addr, off, lo, nz) in enumerate(thumbs):
    im = Image.open(fn).resize((544, 320), Image.NEAREST)
    x, y = (i % cols) * 560 + 8, (i // cols) * 330 + 20
    idxsheet.paste(im, (x, y))
    d.text((x, y - 14), '%s+%x lo%.2f' % (addr, off, lo), fill=(255, 230, 100))
idxsheet.save(os.path.join(ROOT, 'dump', 'sprite_blocks', 'index.png'))
print('wrote sprite_blocks/index.png', idxsheet.size)
