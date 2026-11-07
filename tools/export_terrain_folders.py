"""Organize terrain into per-terrain folders:
    assets/terrain/00_void/
        terrain.json       configuration (defense, move cost, name)
        battle_left.png    combat backdrop (self-pair, attacker side)
        battle_right.png   combat backdrop (self-pair, defender side)
        world_map.png      placeholder texture for the world map
"""
import os, json, shutil
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
TD = json.load(open(os.path.join(ROOT, 'assets', 'data', 'terrain.json'), encoding='utf-8'))
CT = json.load(open(os.path.join(ROOT, 'assets', 'data', 'combat_terrain.json'), encoding='utf-8'))
remap = CT['terrain_remap']
table = CT['panel_table']
PANELS = os.path.join(ROOT, 'assets', 'art', 'combat_terrain')
OUT = os.path.join(ROOT, 'assets', 'terrain')
os.makedirs(OUT, exist_ok=True)

names = TD.get('names', {})
for t in range(16):
    r = remap[t] if t < len(remap) else 6
    left = table[(r * 16 + r + 8)] if (r * 16 + r + 8) < len(table) else 6
    right = table[(r * 16 + r + 0)]
    left %= 24
    right %= 24
    name = names.get(str(t), 'terrain%d' % t)
    safe = ''.join(c if c.isalnum() else '_' for c in name).strip('_') or 't%d' % t
    folder = os.path.join(OUT, '%02d_%s' % (t, safe))
    os.makedirs(folder, exist_ok=True)
    lpath = os.path.join(PANELS, 'panel_%02d.png' % left)
    rpath = os.path.join(PANELS, 'panel_%02d.png' % right)
    shutil.copyfile(lpath, os.path.join(folder, 'battle_left.png'))
    shutil.copyfile(rpath, os.path.join(folder, 'battle_right.png'))
    # placeholder world-map texture: average colour of the backdrop
    im = Image.open(lpath).convert('RGB')
    small = im.resize((1, 1), Image.BOX)
    col = small.getpixel((0, 0))
    Image.new('RGB', (32, 32), col).save(os.path.join(folder, 'world_map.png'))
    cfg = {
        'id': t,
        'name': name,
        'defense': TD['defense'][t] if t < len(TD['defense']) else 0,
        'move_cost': TD['move_cost'][t] if t < len(TD['move_cost']) else [1, 1, 1, 1],
        'battle_left': 'battle_left.png',
        'battle_right': 'battle_right.png',
        'world_map': 'world_map.png',
        'combat_panel_left': left,
        'combat_panel_right': right,
    }
    json.dump(cfg, open(os.path.join(folder, 'terrain.json'), 'w', encoding='utf-8'),
              indent=2, ensure_ascii=False)
    print('terrain %2d %-10s panels %2d/%2d' % (t, name, left, right))
print('wrote 16 terrain folders to', OUT)
