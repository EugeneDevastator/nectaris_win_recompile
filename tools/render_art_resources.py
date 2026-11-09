"""Render extracted art resources with a chosen palette bank and build contact sheets."""
import os, sys, json
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
OUT = os.path.join(ROOT, 'dump', 'art_resources')
M = json.load(open(os.path.join(OUT, 'manifest.json'), encoding='utf-8'))['resources']
pal = open(os.path.join(OUT, 'palette.bin'), 'rb').read()
PAL = [(pal[4 + i * 4], pal[5 + i * 4], pal[6 + i * 4]) for i in range(256)]


def render_tiles(raw, count, bank, scale=2, cols=16):
    tw = th = 16
    rows = max(1, (count + cols - 1) // cols)
    img = Image.new('RGBA', (cols * tw * scale, rows * th * scale), (0, 0, 0, 0))
    px = img.load()
    for t in range(count):
        b = t * 256
        if b + 256 > len(raw):
            break
        sx = (t % cols) * tw * scale
        sy = (t // cols) * th * scale
        for y in range(16):
            for x in range(16):
                v = raw[b + y * 16 + x]
                if not v:
                    continue
                r, g, bl = PAL[(bank << 4) | (v & 0xF)]
                for yy in range(scale):
                    for xx in range(scale):
                        px[sx + x * scale + xx, sy + (15 - y) * scale + yy] = (r, g, bl, 255)
    return img


def render_sprites(raw, count, bank, scale=2, cols=8):
    tw = th = 32
    rows = max(1, (count + cols - 1) // cols)
    img = Image.new('RGBA', (cols * tw * scale, rows * th * scale), (0, 0, 0, 0))
    px = img.load()
    for t in range(count):
        b = t * 1024
        if b + 1024 > len(raw):
            break
        sx = (t % cols) * tw * scale
        sy = (t // cols) * th * scale
        for y in range(32):
            for x in range(32):
                v = raw[b + y * 32 + x]
                if not v:
                    continue
                r, g, bl = PAL[(bank << 4) | (v & 0xF)]
                for yy in range(scale):
                    for xx in range(scale):
                        px[sx + x * scale + xx, sy + (31 - y) * scale + yy] = (r, g, bl, 255)
    return img


def render_one(r, bank):
    raw = open(os.path.join(OUT, r['expanded']), 'rb').read()
    if r['kind'] == 'tile':
        return render_tiles(raw, r['units'], bank)
    return render_sprites(raw, r['units'], bank)


banks = [int(x) for x in sys.argv[1:]] or [1, 6, 9, 10, 13]
for bank in banks:
    thumbs = []
    for r in M:
        im = render_one(r, bank)
        im.save(os.path.join(OUT, 'res_%02d_bank%02d.png' % (r['id'], bank)))
        w = 280
        h = max(1, int(im.height * w / im.width))
        thumbs.append((r, im.resize((w, h), Image.NEAREST)))
    sheet_w = 1200
    row_h = 150
    rows = (len(thumbs) + 3) // 4
    sheet = Image.new('RGB', (sheet_w, rows * row_h + 20), (18, 18, 26))
    d = ImageDraw.Draw(sheet)
    for i, (r, im) in enumerate(thumbs):
        col, row = i % 4, i // 4
        x = col * 300 + 10
        y = row * row_h + 20
        sheet.paste(im, (x, y + 18), im)
        d.text((x, y), '%02d t%d %s n%d' % (r['id'], r['type'], r['kind'], r['units']),
               fill=(255, 230, 100))
    path = os.path.join(OUT, 'contact_bank%02d.png' % bank)
    sheet.save(path)
    print('wrote', path, sheet.size)
