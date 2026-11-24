"""Export the combat tables/constants from the original executable into
assets/data/combat.json.

Sources (see docs/RE_FINDINGS.md section 5):
  0x42b158  HP/rank damage multiplier (8 bytes)
  0x42b160  terrain defence (16 bytes)
  0x42b248  terrain move cost (16 x 4 bytes)   -> assets/data/terrain.json
  0x42b288  unit ability flags (24 bytes)      -> assets/data/units.json
  code      variance bands and XP/HP constants
"""
import json, io, os

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
tables = json.load(io.open(os.path.join(ROOT, 'dump', 'tables.json'), encoding='utf-8'))

combat = {
    'hp_damage_multiplier': tables['hp_damage_multiplier'],
    'terrain_defense': tables['terrain_defense'],
    'variance': [
        {'min': 20, 'max': 24, 'mult': 15},
        {'min': 60, 'max': 64, 'mult': 20},
        {'min': 80, 'max': 82, 'mult': 2},
        {'min': 98, 'max': 99, 'mult': 40},
    ],
    'variance_base': 150,
    'variance_divisor': 10,
    'support_divisor': 2,
    'surround_divisor': 2,
    'hp_bonus': 50,
    'destroy_threshold': 8,
    'xp': {'hit': 4, 'kill': 8, 'no_damage': 8, 'max': 31, 'rank_divisor': 4},
    'ability_flags': {
        'air': 0x40,
        'transport': 0x20,
        'indirect': 0x08,
        'move_after_attack': 0x04,
    },
    'source': {
        'formula': 'FUN_0041b810 + FUN_00420cd0',
        'variance': 'FUN_00421170 / FUN_0040cb50',
        'xp': 'FUN_00415850 / FUN_0040a460',
        'tables': '0x42b158, 0x42b160, 0x42b288',
    },
}

out = os.path.join(ROOT, 'assets', 'data', 'combat.json')
json.dump(combat, open(out, 'w', encoding='utf-8'), indent=2)
print('wrote', out)
print(' hp multipliers:', combat['hp_damage_multiplier'])
print(' terrain defence:', combat['terrain_defense'])
