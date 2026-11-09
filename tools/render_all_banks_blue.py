"""Render every decoded art resource with the single combat-blue palette bank (9)."""
import json, os
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
D = os.path.join(ROOT, 'dump', 'art_resources')
OUT = os.path.join(ROOT, 'dump', 'decoded_blue')
os.makedirs(OUT, exist_ok=True)
M = json.load(open(os.path.join(D, 'manifest.json')))['resources']
pal = open(os.path.join(D, 'palette.bin'), 'rb').read()
PAL = [(pal[4 + i * 4], pal[5 + i * 4], pal[6 + i * 4]) for i in range(256)]
BANK = 9


def col(v):
    return PAL[((BANK << 4) | (v & 15)) & 255]


def render_sprite_page(rid, count, raw, scale=4, cols=8):
    cell = 32 * scale + 8
    rows = (count + cols - 1) // cols
    sheet = Image.new('RGB', (cols * cell, rows * cell + 18), (18, 18, 26))
    d = ImageDraw.Draw(sheet)
    for t in range(count):
        sx = (t % cols) * cell + 4
        sy = (t // cols) * cell + 18
        base = t * 1024
        for y in range(32):
            for x in range(32):
                v = raw[base + y * 32 + x]
                if not v:
                    continue
                c = col(v)
                for yy in range(scale):
                    for xx in range(scale):
                        sheet.putpixel((sx + x * scale + xx, sy + (31 - y) * scale + yy), c)
        d.text((sx, sy - 13), '%d' % t, fill=(255, 230, 100))
    return sheet


def render_tile_page(rid, count, raw, scale=3, cols=16):
    cell = 16 * scale + 4
    rows = (count + cols - 1) // cols
    sheet = Image.new('RGB', (cols * cell, rows * cell + 18), (18, 18, 26))
    d = ImageDraw.Draw(sheet)
    for t in range(count):
        sx = (t % cols) * cell + 2
        sy = (t // cols) * cell + 18
        base = t * 256
        for y in range(16):
            for x in range(16):
                v = raw[base + y * 16 + x]
                if not v:
                    continue
                c = col(v)
                for yy in range(scale):
                    for xx in range(scale):
                        sheet.putpixel((sx + x * scale + xx, sy + (15 - y) * scale + yy), c)
        if scale >= 3:
            d.text((sx, sy - 12), '%d' % t, fill=(150, 150, 90))
    return sheet


pages = []
for r in M:
    raw = open(os.path.join(D, r['expanded']), 'rb').read()
    if r['kind'] == 'tile':
        n = min(r['units'], len(raw) // 256)
        sheet = render_tile_page(r['id'], n, raw)
    else:
        n = min(r['units'], len(raw) // 1024)
        sheet = render_sprite_page(r['id'], n, raw)
    fn = 'res_%02d_%s_b09.png' % (r['id'], r['kind'])
    sheet.save(os.path.join(OUT, fn))
    pages.append((r, fn, sheet))
    print(fn, sheet.size, flush=True)

# index sheet of thumbnails
cols = 5
tw, th = 300, 220
rows = (len(pages) + cols - 1) // cols
idx = Image.new('RGB', (cols * (tw + 10), rows * (th + 24)), (16, 16, 24))
d = ImageDraw.Draw(idx)
for i, (r, fn, sh) in enumerate(pages):
    x = (i % cols) * (tw + 10) + 5
    y = (i // cols) * (th + 24) + 18
    thumb = sh.copy()
    thumb.thumbnail((tw, th), Image.NEAREST)
    idx.paste(thumb, (x, y))
    d.text((x, y - 13), '%02d t%d %s %s' % (r['id'], r['type'], r['kind'], fn.split('_')[-1]), fill=(255, 230, 100))
idx.save(os.path.join(OUT, 'index.png'))
print('wrote', OUT, len(pages), 'pages')
