"""Build per-unit folders:
    assets/units/unit_XX_code_name/
        unit.json          configuration (stats + sprite metadata)
        world_map.png      8-direction map-icon strip (256x32)
        combat_blue.png    64x64 battle sprite, player palette
        combat_green.png   64x64 battle sprite, enemy palette
        combat_green_flip.png  mirrored for the defender

The person is treated like every other unit (no special gun palette).
"""
import os, json, re, shutil
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
battle = open(os.path.join(ROOT, 'dump', 'spritebank_battle.bin'), 'rb').read()
mapbank = open(os.path.join(ROOT, 'dump', 'spritebank.bin'), 'rb').read()
pal = json.load(open(os.path.join(ROOT, 'assets', 'art', 'palette.json')))
PAL = [(p['r'], p['g'], p['b']) for p in pal]
groups = json.load(open(os.path.join(ROOT, 'dump', 'sprite_groups.json')))
units = json.load(open(os.path.join(ROOT, 'assets', 'data', 'units.json'), encoding='utf-8'))['units']
names = {u['id']: u['name'] for u in units}
# Readable folder suffixes: code + StrategyWiki name, e.g. unit_00_m77_trigger.
SLUGS = {
    0: 'm77_trigger', 1: 'fx1_falcon', 2: 'ax87_eagle', 3: 'ef88_hunter',
    4: 't79_grizzly', 5: 'pt6_polar', 6: 's61_bison', 7: 'gs81_slagger',
    8: 'gt86_titan', 9: 'hmb2_giant', 10: 'tt1_lenet', 11: 'sg4_hadrian',
    12: 'mr22_octopus', 13: 'mb5_rabbit', 14: 'mb4_lynx', 15: 'aag4_seeker',
    16: 'mm107_hawkeye', 17: 'ss80_atlas', 18: 'gx77_charlie',
    19: 'gx87_kilroy', 20: 'cbx1_panther', 21: 'nc1_mule', 22: 'c61_pelican',
    23: 'yamaarashi',
}
OUT = os.path.join(ROOT, 'assets', 'units')
os.makedirs(OUT, exist_ok=True)

def tile(data, idx, bank):
    im = Image.new('RGBA', (32, 32), (0, 0, 0, 0)); px = im.load(); off = idx * 1024
    if off + 1024 > len(data):
        return im
    for y in range(32):
        for x in range(32):
            v = data[off + y * 32 + x]
            if v:
                px[x, 31 - y] = PAL[((bank << 4) | (v & 0xF)) & 0xFF] + (255,)
    return im

def compose(sprites, bank):
    im = Image.new('RGBA', (64, 64), (0, 0, 0, 0))
    for k, spr in enumerate(sprites[:4]):
        dst = [(32, 0), (0, 0), (32, 32), (0, 32)][k]
        t = tile(battle, spr, bank)
        im.paste(t, dst, t)
    return im

PERSON = [688, 689, 690, 691]
# map icon strips: the global map bank holds 14 icon sets of 8 rotations, at 80..191
def map_strip(t, bank):
    base = 80 + (t % 14) * 8
    strip = Image.new('RGBA', (8 * 32, 32), (0, 0, 0, 0))
    for d in range(8):
        strip.paste(tile(mapbank, base + d, bank), (d * 32, 0))
    return strip

for t in range(24):
    if t in (18, 19):
        sprites = PERSON
    elif t <= 22:
        sprites = [p[2] + 512 for p in groups[str(233 + t)]]
    else:
        sprites = [p[2] + 512 for p in groups['255']]

    suffix = SLUGS.get(t) or re.sub(r'[^a-z0-9]+', '_',
                                    str(units[t].get('name', '')).lower()).strip('_')
    dirname = ('unit_%02d_%s' % (t, suffix)) if suffix else ('unit_%02d' % t)
    folder = os.path.join(OUT, dirname)
    os.makedirs(folder, exist_ok=True)
    blue = compose(sprites, 9)
    green = compose(sprites, 10)
    blue.save(os.path.join(folder, 'combat_blue.png'))
    green.save(os.path.join(folder, 'combat_green.png'))
    green.transpose(Image.FLIP_LEFT_RIGHT).save(os.path.join(folder, 'combat_green_flip.png'))
    map_strip(t, 9).save(os.path.join(folder, 'world_map.png'))
    # Exact map-icon palette states: green enemy, grey used-up.
    midx = 160 + (t if t < 23 else 0)
    tile(mapbank, midx, 10).save(os.path.join(folder, 'horizontal_green.png'))
    tile(mapbank, midx, 12).save(os.path.join(folder, 'horizontal_used.png'))

    u = dict(units[t])
    u.update({
        'folder': dirname,
        'combat_chunks': sprites,
        'combat_sprite_group': 233 + t if t <= 22 and t not in (18, 19) else 'infantry',
        'world_map_base': 80 + (t % 14) * 8,
        'sprites': ['horizontal.png', 'horizontal_green.png',
                    'horizontal_used.png'],
    })
    json.dump(u, open(os.path.join(folder, 'unit.json'), 'w', encoding='utf-8'),
              indent=2, ensure_ascii=False)
print('wrote 24 unit folders to', OUT)
