import os, json
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
# use the corrected (flipped) unit bank sheets
bank = Image.open(os.path.join(ROOT, 'assets', 'art', 'units_bank09.png')).convert('RGBA')
units = json.load(open(os.path.join(ROOT, 'assets', 'data', 'units.json'), encoding='utf-8'))['units']
names = {u['id']: u['name'] for u in units}

def cell(idx):
    return bank.crop(((idx % 32) * 32, (idx // 32) * 32,
                      (idx % 32) * 32 + 32, (idx // 32) * 32 + 32))

out = Image.new('RGB', (8 * 130, 4 * 146), (10, 10, 18))
d = ImageDraw.Draw(out)
for i, idx in enumerate(range(160, 192)):
    im = cell(idx).resize((120, 120), Image.NEAREST)
    x = (i % 8) * 130; y = (i // 8) * 146
    out.paste(im, (x + 5, y + 20), im)
    label = '%d' % (idx - 160)
    if idx - 160 < 24:
        label += ':' + names.get(idx - 160, '?')
    d.text((x + 5, y + 2), label, fill=(255, 255, 0))
out.save(os.path.join(ROOT, 'dump', 'row4_combat.png'))
print('wrote dump/row4_combat.png', out.size)
