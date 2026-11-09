"""Build the final combat sprite atlas from the runtime-captured per-type sprites."""
import os, json
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
D = os.path.join(ROOT, 'assets', 'art', 'combat_units_real')
units = json.load(open(os.path.join(ROOT, 'assets', 'data', 'units.json'), encoding='utf-8'))['units']
names = {u['id']: u['name'] for u in units}

cols = 2
cell = 150
rows = 23
sheet = Image.new('RGB', (cols * cell + 190, rows * 110 + 20), (20, 20, 30))
d = ImageDraw.Draw(sheet)
manifest = []
missing = []
for t in range(23):
    y = 20 + t * 110
    d.text((4, y + 38), '%2d %s' % (t, names.get(t, '?')), fill=(255, 230, 100))
    entry = {'unit': t, 'name': names.get(t, '?')}
    for j, tag in enumerate(('blue', 'green')):
        fn = os.path.join(D, 'combat_type_%02d_%s.png' % (t, tag))
        x = 190 + j * cell
        if os.path.exists(fn):
            im = Image.open(fn).convert('RGBA')
            bb = im.getbbox()
            if bb:
                im = im.crop(bb)
            im = im.resize((im.width * 2, im.height * 2), Image.NEAREST)
            sheet.paste(im, (x, y), im)
            d.text((x, y + 78), '%s %dx%d' % (tag, im.width // 2, im.height // 2), fill=(160, 160, 100))
            entry[tag] = os.path.basename(fn)
        else:
            d.text((x, y + 38), 'MISSING', fill=(255, 80, 80))
    if 'blue' not in entry:
        missing.append(t)
    manifest.append(entry)
sheet.save(os.path.join(D, 'combat_units_atlas.png'))
json.dump({'units': manifest, 'missing': missing}, open(os.path.join(D, 'combat_units_manifest.json'), 'w'), indent=1)
print('wrote', os.path.join(D, 'combat_units_atlas.png'), sheet.size, 'missing', missing)
