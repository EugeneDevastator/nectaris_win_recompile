"""Compare spritebank 64+type against the live battle framebuffer."""
import os
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
D = os.path.join(ROOT, 'dump', 'battle_sprites')
b = open(os.path.join(D, 'spritebank.bin'), 'rb').read()
pal = open(os.path.join(D, 'palette.bin'), 'rb').read()
PAL = [(pal[4 + i * 4], pal[5 + i * 4], pal[6 + i * 4]) for i in range(256)]


def tile(idx, bank):
    im = Image.new('RGBA', (32, 32), (0, 0, 0, 0))
    px = im.load()
    off = idx * 1024
    for y in range(32):
        for x in range(32):
            v = b[off + y * 32 + x]
            if v:
                px[x, 31 - y] = PAL[((bank << 4) | (v & 15)) & 255] + (255,)
    return im


frame = Image.open(os.path.join(D, 'frames', 'f10.png')).convert('RGB')
# downscale the playfield to its logical 320x240 resolution
small = frame.resize((320, 240), Image.NEAREST)
small.save(os.path.join(D, 'f10_logical.png'))

sheet = Image.new('RGB', (4 * 140, 2 * 140), (20, 20, 30))
d = ImageDraw.Draw(sheet)
for i, (idx, bank) in enumerate([(70, 9), (70, 10), (82, 9), (82, 10)]):
    im = tile(idx, bank).resize((128, 128), Image.NEAREST)
    sheet.paste(im, (i * 140 + 6, 18), im)
    d.text((i * 140 + 6, 2), 'sprite %d bank %d' % (idx, bank), fill=(255, 255, 0))
# candidate crops from the logical frame (blue unit top-left area)
crops = [(28, 74, 60, 106), (40, 90, 72, 122), (52, 105, 84, 137)]
for j, box in enumerate(crops):
    c = small.crop(box).resize((128, 128), Image.NEAREST)
    sheet.paste(c, (j * 140 + 6, 158))
    d.text((j * 140 + 6, 142), 'frame crop %d' % j, fill=(255, 255, 0))
sheet.save(os.path.join(D, 'unit_compare.png'))
print('wrote unit_compare.png')
