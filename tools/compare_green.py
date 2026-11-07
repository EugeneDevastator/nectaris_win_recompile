import os, json
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
b = open(os.path.join(ROOT, 'dump', 'spritebank_battle.bin'), 'rb').read()
pal = json.load(open(os.path.join(ROOT, 'assets', 'art', 'palette.json')))
PAL = [(p['r'], p['g'], p['b']) for p in pal]

def tile(idx, bank):
    im = Image.new('RGBA', (32, 32), (0, 0, 0, 0)); px = im.load(); off = idx*1024
    for y in range(32):
        for x in range(32):
            v = b[off+y*32+x]
            if v: px[x, 31-y] = PAL[((bank<<4)|(v&0xF))&0xFF]+(255,)
    return im

def compose(base, bank):
    im = Image.new('RGBA', (64, 64), (0, 0, 0, 0))
    for k, dst in enumerate([(32,0),(0,0),(32,32),(0,32)]):
        t = tile(base+k, bank); im.paste(t, dst, t)
    return im

real = Image.open(os.path.join(ROOT, 'ref', 'seq', 'after_021.png')).convert('RGB')
px = real.load()
def is_green(r, g, b):
    return g > 70 and g >= r and (g - r > 12 or g - b > 12) and not (r > 180 and g > 180)

pts = [(x,y) for y in range(190,375) for x in range(320,580) if is_green(*px[x,y])]
cl = []
for p in pts:
    for c in cl:
        if abs(p[0]-c[0]) < 24 and abs(p[1]-c[1]) < 16:
            c[0]=(c[0]*c[2]+p[0])/(c[2]+1); c[1]=(c[1]*c[2]+p[1])/(c[2]+1); c[2]+=1
            break
    else:
        cl.append([p[0],p[1],1])
cl = sorted([c for c in cl if c[2] > 40], key=lambda c:-c[2])
print('green clusters', [(round(c[0]),round(c[1]),c[2]) for c in cl[:8]])

out = Image.new('RGB', (4*200, 230), (10,10,16)); d = ImageDraw.Draw(out)
if cl:
    cx, cy = int(cl[0][0]), int(cl[0][1])
    rc = real.crop((cx-34, cy-28, cx+34, cy+28)).resize((190, 156), Image.NEAREST)
    out.paste(rc, (10, 30)); d.text((10, 8), f'real green ({cx},{cy})', fill=(255,255,0))
for j, base in enumerate((600, 648, 576)):
    im = compose(base, 10).resize((180,180), Image.NEAREST)
    x = (j+1)*200+10
    out.paste(im, (x, 30), im); d.text((x, 8), f'base {base} b10', fill=(255,255,0))
out.save(os.path.join(ROOT, 'dump', 'compare_green_648.png'))
print('wrote dump/compare_green_648.png')
