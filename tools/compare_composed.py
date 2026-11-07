import os, json
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
b = open(os.path.join(ROOT, 'dump', 'spritebank_battle.bin'), 'rb').read()
pal = json.load(open(os.path.join(ROOT, 'assets', 'art', 'palette.json')))
PAL = [(p['r'], p['g'], p['b']) for p in pal]

def tile(idx, bank, flip=True):
    im = Image.new('RGBA', (32, 32), (0, 0, 0, 0)); px = im.load(); off = idx*1024
    for y in range(32):
        for x in range(32):
            v = b[off+y*32+x]
            if v: px[x, 31-y if flip else y] = PAL[((bank<<4)|(v&0xF))&0xFF]+(255,)
    return im

def compose(base, bank):
    im = Image.new('RGBA', (64, 64), (0, 0, 0, 0))
    dst = [(32,0),(0,0),(32,32),(0,32)]
    for k in range(4):
        t = tile(base+k, bank)
        im.paste(t, dst[k], t)
    return im

real = Image.open(os.path.join(ROOT, 'ref', 'seq', 'after_021.png')).convert('RGB')
out = Image.new('RGB', (5*200, 230), (10,10,16))
d = ImageDraw.Draw(out)
crops = [('real blue', 86, 341, 9), ('real green', 430, 210, 10)]
for i, (name, cx, cy, bank) in enumerate(crops):
    c = real.crop((cx-30, cy-24, cx+30, cy+24)).resize((180, 144), Image.NEAREST)
    out.paste(c, (i*200+10, 30)); d.text((i*200+10, 8), name, fill=(255,255,0))
for j, base in enumerate((600, 648)):
    for k, bank in enumerate((9, 10)):
        im = compose(base, bank).resize((180, 180), Image.NEAREST)
        x = (2 + j*2 + k) * 200 + 10
        out.paste(im, (x, 30), im)
        d.text((x, 8), f'base {base} b{bank}', fill=(255,255,0))
out.save(os.path.join(ROOT, 'dump', 'compare_600_648.png'))
print('wrote dump/compare_600_648.png')
