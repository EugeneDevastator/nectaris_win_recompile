import os, json
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
b = open(os.path.join(ROOT, 'dump', 'spritebank_battle.bin'), 'rb').read()
pal = json.load(open(os.path.join(ROOT, 'assets', 'art', 'palette.json')))
PAL = [(p['r'], p['g'], p['b']) for p in pal]
OUT = os.path.join(ROOT, 'assets', 'art', 'combat_units')

def idxcolors(raw, bank):
    im = Image.new('RGBA', (32, 32), (0, 0, 0, 0)); px = im.load()
    for y in range(32):
        for x in range(32):
            v = raw[y * 32 + x]
            if v:
                px[x, 31 - y] = PAL[((bank << 4) | (v & 0xF)) & 0xFF] + (255,)
    return im

def compose_custom(base, body_bank):
    # body indices 1..5 use the team bank; 6..7 come from bank 7 (orange gun)
    im = Image.new('RGBA', (64, 64), (0, 0, 0, 0)); px = im.load()
    for k, (ox, oy) in enumerate([(32, 0), (0, 0), (32, 32), (0, 32)]):
        raw = b[(base + k) * 1024:(base + k) * 1024 + 1024]
        for y in range(32):
            for x in range(32):
                v = raw[y * 32 + x]
                if not v:
                    continue
                n = v & 0xF
                if n in (6, 7):
                    col = PAL[(7 << 4) | n]          # yellow/orange gun
                else:
                    col = PAL[((body_bank << 4) | n) & 0xFF]
                px[ox + x, oy + (31 - y)] = col + (255,)
    return im

# individual
for bank, tag in ((9, 'blue'), (10, 'green')):
    im = compose_custom(688, bank)
    im.save(os.path.join(OUT, f'person_orange_gun_{tag}.png'))
    print('wrote', f'person_orange_gun_{tag}.png')

# comparison with the real green soldier
real = Image.open(os.path.join(ROOT, 'ref', 'seq', 'after_021.png')).convert('RGB')
out = Image.new('RGB', (3 * 200, 220), (10, 10, 16)); d = ImageDraw.Draw(out)
rc = real.crop((549 - 34, 334 - 28, 549 + 34, 334 + 28)).resize((190, 156), Image.NEAREST)
out.paste(rc, (10, 30)); d.text((10, 8), 'real green soldier', fill=(255, 255, 0))
for i, tag in enumerate(('blue', 'green')):
    im = compose_custom(688, 9 if tag == 'blue' else 10).resize((180, 180), Image.NEAREST)
    x = (i + 1) * 200 + 10
    out.paste(im, (x, 30), im); d.text((x, 8), f'person {tag} + orange gun', fill=(255, 255, 0))
out.save(os.path.join(OUT, 'person_check.png'))
print('wrote person_check.png')
