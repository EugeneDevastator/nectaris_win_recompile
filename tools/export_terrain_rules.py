"""Export the original terrain defence / movement rules from Nec.exe.

Tables (verified against the live game and FUN_00401680):
  0x42b160  terrain defence, 16 bytes
  0x42b248  movement cost, 16 terrains x 4 movement classes
            cost bit 0x80 = impassable
            cost 0x7f = consumes the unit's whole movement allowance
  0x42b230  unit movement class (0 tracked, 1 wheeled, 2 bike,
            3 infantry, 4 air; 4+ ignores terrain and costs 1)
  0x42b218  unit movement points

Writes assets/data/terrain.json, keeping the existing terrain names.
"""
import json
import os

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
EXE = os.path.join(ROOT, 'game', 'Nec.exe')
OUT = os.path.join(ROOT, 'assets', 'data', 'terrain.json')
BASE_VA, BASE_OFF = 0x42a000, 0x27c00

b = open(EXE, 'rb').read()


def arr(va, n):
    return list(b[BASE_OFF + (va - BASE_VA):BASE_OFF + (va - BASE_VA) + n])


def main():
    old = {}
    if os.path.exists(OUT):
        old = json.load(open(OUT, encoding='utf-8'))

    defense = arr(0x42b160, 16)
    move = arr(0x42b248, 64)
    move_cost = [move[i * 4:i * 4 + 4] for i in range(16)]
    unit_class = arr(0x42b230, 24)
    unit_move = arr(0x42b218, 24)

    names = old.get('names') or {
        str(i): n for i, n in enumerate(
            ['void', 'grass', 'forest', 'mountain', 'water', 'plain', 'city',
             'factory', 'hq', 'road', 'bridge', 'base', 'ruin', 'special',
             'x13', 'x14'])
    }
    cfg = {
        'defense': defense,
        'move_cost': move_cost,
        'names': names,
        'movement_classes': {
            '0': 'tracked', '1': 'wheeled', '2': 'bike', '3': 'infantry',
            '4': 'air (ignores terrain, cost 1)',
        },
        'rules': {
            'impassable_mask': 0x80,
            'all_movement_cost': 0x7f,
            'note': 'cost = move_cost[terrain][unit movement class]; bit 0x80 = impassable; 0x7f = whole movement allowance and only enterable with full MP; class >= 4 ignores terrain (cost 1)',
        },
        'unit_class': {str(i): unit_class[i] for i in range(24)},
        'unit_movement': {str(i): unit_move[i] for i in range(24)},
    }
    json.dump(cfg, open(OUT, 'w', encoding='utf-8'), indent=2, ensure_ascii=False)
    print('wrote', OUT)

    # sanity checks against the extracted unit table
    units_path = os.path.join(ROOT, 'assets', 'data', 'units.json')
    if os.path.exists(units_path):
        units = json.load(open(units_path, encoding='utf-8'))['units']
        bad = []
        for u in units:
            i = u['id']
            if i < 24 and (u.get('movement_class') != unit_class[i] or
                           u.get('movement') != unit_move[i]):
                bad.append((i, u.get('movement_class'), u.get('movement'),
                            unit_class[i], unit_move[i]))
        print('unit table mismatches:', bad if bad else 'none')
    print('movement classes:', list(unit_class))
    print('movement points :', list(unit_move))


if __name__ == '__main__':
    main()
