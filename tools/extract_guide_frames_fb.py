"""Extract the guide frames from the raw 8-bit framebuffer dumps and write the
guide stat block into the unit configs.

The WEAPON DATA screen draws each sprite into the 8-bit screen buffer at
DAT_00480f18.  Box interiors (screen coords): (28,112) and (108,112), 64x64.
Background palette index is 16; everything else is sprite.
"""
import os, json, io
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
GUIDE = os.path.join(ROOT, 'dump', 'guide')
UNITS = os.path.join(ROOT, 'assets', 'units')
W, H = 704, 544
BOXES = {'combat_alive': (28, 112), 'combat_diagonal': (108, 112)}
BG = 16

sheet = Image.new('RGBA', (24 * 72, 2 * 72), (20, 20, 28, 255))
all_guide = []

for t in range(24):
    fbp = os.path.join(GUIDE, 'fb_type_%02d.bin' % t)
    palp = os.path.join(GUIDE, 'pal_type_%02d.bin' % t)
    if not (os.path.exists(fbp) and os.path.exists(palp)):
        print('missing', t); continue
    fb = open(fbp, 'rb').read()
    pal = open(palp, 'rb').read()
    PAL = [(pal[4 + i * 4], pal[4 + i * 4 + 1], pal[4 + i * 4 + 2]) for i in range(256)]
    screen = Image.frombytes('P', (W, H), fb).transpose(Image.FLIP_TOP_BOTTOM).crop((32, 32, 672, 512))
    spx = screen.load()
    for row, (name, (bx, by)) in enumerate(BOXES.items()):
        img = Image.new('RGBA', (64, 64), (0, 0, 0, 0)); px = img.load()
        if t != 23:
            for y in range(64):
                for x in range(64):
                    v = spx[bx + x, by + y]
                    if v != BG:
                        px[x, y] = PAL[v] + (255,)
        folder = os.path.join(UNITS, 'unit_%02d' % t)
        os.makedirs(folder, exist_ok=True)
        img.save(os.path.join(folder, name + '.png'))
        sheet.paste(img, (t * 72 + 4, row * 72 + 4), img)

    meta_path = os.path.join(UNITS, 'unit_%02d' % t, 'unit.json')
    meta = json.load(io.open(meta_path, encoding='utf-8'))
    rg = meta['counter_ground'] if meta['attack_ground'] else 0
    ra = meta['counter_air'] if meta['attack_air'] else 0
    guide = {
        'english_name': 'DUMMY' if t == 23 else meta['name'],
        'movement': meta['movement'],
        'defense': meta['defense'],
        'attack_ground': meta['attack_ground'],
        'attack_air': meta['attack_air'],
        'range_ground': rg,
        'range_air': ra,
        'frame_horizontal': 'combat_alive.png',
        'frame_diagonal': 'combat_diagonal.png',
    }
    meta['guide'] = guide
    json.dump(meta, io.open(meta_path, 'w', encoding='utf-8'), indent=2, ensure_ascii=False)
    all_guide.append(dict(id=t, **guide))
    print('unit %02d %-10s MOV %d DEF %d ATK %d/%d RNG %d/%d' %
          (t, meta['name'], guide['movement'], guide['defense'],
           guide['attack_ground'], guide['attack_air'],
           guide['range_ground'], guide['range_air']))

json.dump({'units': all_guide},
          io.open(os.path.join(ROOT, 'assets', 'data', 'units_guide.json'), 'w', encoding='utf-8'),
          indent=2, ensure_ascii=False)
sheet.save(os.path.join(GUIDE, 'guide_units_sheet_fb.png'))
print('wrote assets/data/units_guide.json and per-unit guide blocks')
