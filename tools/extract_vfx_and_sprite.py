"""Extract the combat VFX (battle bank tiles 832..896) and pick one generic
horizontal combat sprite (base 600, the S-61 tank) for the port."""
import os, json
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
b = open(os.path.join(ROOT, 'dump', 'spritebank_battle.bin'), 'rb').read()
pal = json.load(open(os.path.join(ROOT, 'assets', 'art', 'palette.json')))
PAL = [(p['r'], p['g'], p['b']) for p in pal]
OUT = os.path.join(ROOT, 'assets', 'art', 'combat_effects')
os.makedirs(OUT, exist_ok=True)

def tile(idx, bank):
    im = Image.new('RGBA', (32, 32), (0, 0, 0, 0)); px = im.load(); off = idx * 1024
    if off + 1024 > len(b):
        return im
    for y in range(32):
        for x in range(32):
            v = b[off + y * 32 + x]
            if v:
                px[x, 31 - y] = PAL[((bank << 4) | (v & 0xF)) & 0xFF] + (255,)
    return im

def frame(base, bank):
    im = Image.new('RGBA', (64, 64), (0, 0, 0, 0))
    for k, dst in enumerate([(32, 0), (0, 0), (32, 32), (0, 32)]):
        t = tile(base + k, bank)
        im.paste(t, dst, t)
    return im

# VFX: tiles 832..896 = 16 frames
vfx = [frame(832 + i * 4, 6) for i in range(16)]
strip = Image.new('RGBA', (16 * 64, 64), (0, 0, 0, 0))
for i, im in enumerate(vfx):
    strip.paste(im, (i * 64, 0), im)
strip.save(os.path.join(OUT, 'vfx_sheet.png'))
strip.resize((16 * 128, 128), Image.NEAREST).save(os.path.join(OUT, 'vfx_sheet_x2.png'))
for i, im in enumerate(vfx):
    im.save(os.path.join(OUT, 'vfx_%02d.png' % i))
print('wrote vfx_sheet.png + vfx_00..15.png')

# generic combat sprite: base 600 (S-61), both teams, mirrored
for bank, tag in ((9, 'blue'), (10, 'green')):
    im = frame(600, bank)
    im.save(os.path.join(OUT, '..', 'combat_units', 'combat_sprite_%s.png' % tag))
    if tag == 'green':
        im.transpose(Image.FLIP_LEFT_RIGHT).save(
            os.path.join(OUT, '..', 'combat_units', 'combat_sprite_green_flip.png'))
print('wrote generic combat sprite')
