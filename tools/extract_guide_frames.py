"""Extract the two WEAPON DATA sprites (alive + wreck) for all 24 unit types.

Captured by tools/extract_guide_all.py: dump/guide/type_00..23.png.
Box interiors: (32,161)-(94,223) and (112,161)-(174,223); background is
palette colour (8,57,90).
"""
import os, json, io
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
GUIDE = os.path.join(ROOT, 'dump', 'guide')
UNITS = os.path.join(ROOT, 'assets', 'units')
BG = (8, 57, 90)
BOXES = {'combat_alive': (32, 161, 95, 224), 'combat_wreck': (112, 161, 175, 224)}
os.makedirs(os.path.join(GUIDE, 'frames'), exist_ok=True)

sheet = Image.new('RGBA', (24 * 72, 2 * 72), (20, 20, 28, 255))
for t in range(24):
    src = Image.open(os.path.join(GUIDE, 'type_%02d.png' % t)).convert('RGB')
    for row, (name, box) in enumerate(BOXES.items()):
        if t == 23:
            canvas = Image.new('RGBA', (64, 64), (0, 0, 0, 0))
            folder = os.path.join(UNITS, 'unit_%02d' % t)
            os.makedirs(folder, exist_ok=True)
            canvas.save(os.path.join(folder, name + '.png'))
            continue
        crop = src.crop(box).convert('RGBA')
        px = crop.load()
        for y in range(crop.height):
            for x in range(crop.width):
                if px[x, y][:3] == BG:
                    px[x, y] = (0, 0, 0, 0)
        # 64x64 canvas, sprite keeps its position inside the guide box
        canvas = Image.new('RGBA', (64, 64), (0, 0, 0, 0))
        canvas.paste(crop, (0, 0))
        folder = os.path.join(UNITS, 'unit_%02d' % t)
        os.makedirs(folder, exist_ok=True)
        canvas.save(os.path.join(folder, name + '.png'))
        canvas.save(os.path.join(GUIDE, 'frames', 'unit_%02d_%s.png' % (t, name)))
        sheet.paste(canvas, (t * 72 + 4, row * 72 + 4), canvas)
    meta_path = os.path.join(UNITS, 'unit_%02d' % t, 'unit.json')
    if os.path.exists(meta_path):
        meta = json.load(io.open(meta_path, encoding='utf-8'))
        sprites = meta.get('sprites', [])
        for n in ('combat_alive.png', 'combat_wreck.png'):
            if n not in sprites:
                sprites.append(n)
        meta['sprites'] = sprites
        meta['guide_frames'] = {'alive': 'combat_alive.png', 'wreck': 'combat_wreck.png'}
        json.dump(meta, io.open(meta_path, 'w', encoding='utf-8'), indent=2, ensure_ascii=False)
    print('unit %02d %-12s bbox=%s' % (t, name.replace('combat_', ''), canvas.getbbox()))

sheet.save(os.path.join(GUIDE, 'guide_units_sheet.png'))
print('wrote assets/units/unit_XX/combat_alive.png + combat_wreck.png')
