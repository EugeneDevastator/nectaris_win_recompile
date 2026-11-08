"""Compare the decoded battle state against the original game's own registers.

Reads dump/combat_state.json (tools/validate_combat_formulas.py) and:
  1. recomputes FUN_0041b810's intermediates (e4c/e50/e48/e4e/e55/e5a/e58/e5c)
     and the raw damage;
  2. checks that the game's post-variance e52/e60 are reachable with a valid
     FUN_00421170 multiplier;
  3. writes dump/_validation_mission.json so the C++ port can be run with
     --resolvetest on the exact same state.
"""
import json, os, io

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
state = json.load(open(os.path.join(ROOT, 'dump', 'combat_state.json'), encoding='utf-8'))
units_data = json.load(io.open(os.path.join(ROOT, 'assets', 'data', 'units.json'),
                               encoding='utf-8'))['units']
terrain_data = json.load(io.open(os.path.join(ROOT, 'assets', 'data', 'terrain.json'),
                                 encoding='utf-8'))

W, H = state['map_w'], state['map_h']
terrain = [[state['terrain'][y * 32 + x] & 0xF for x in range(W)] for y in range(H)]

slots = []
for i in range(56):
    t = state['types'][i]
    hr = state['hp_rank'][i]
    xb = state['xs'][i]
    yb = state['ys'][i]
    dead = (t == 0xFF) or (hr & 7) == 0
    slots.append({
        'slot': i,
        'type': 0 if dead else (t & 0x1F),
        'owner': 0 if dead else (1 if (t & 0x80) else 0),
        'hp': 0 if dead else (hr & 7),
        'xp': 0 if dead else ((hr >> 3) & 0x1F),
        'x': xb & 0x1F,
        'y': yb & 0x1F,
        'acted': dead or ((xb & 0xE0) != 0) or ((yb & 0x40) != 0),
    })

A = slots[state['atk_slot']]
D = slots[state['def_slot']]
TA = units_data[A['type']]
TD = units_data[D['type']]
mode = state['mode']

AIR = lambda u: (u['ability_flags'] & 0x40) != 0
atk_vs = lambda at, de: at['attack_air'] if AIR(de) else at['attack_ground']

hA, hD = A['hp'] + 1, D['hp'] + 1
canA = TA['counter_air'] if AIR(TD) else TA['counter_ground']
canD = TD['counter_air'] if AIR(TA) else TD['counter_ground']

baseA = baseD = 0
if mode == 0:
    if canA < 2: baseA = atk_vs(TA, TD)
    if canD == 1: baseD = atk_vs(TD, TA)
else:
    if canA >= 2: baseA = atk_vs(TA, TD)

def terdef(u, tu):
    return 0 if AIR(tu) else terrain_data['defense'][terrain[u['y']][u['x']]]

terA = terdef(A, TA)
terD = terdef(D, TD)

# supports
def neighbor_slots(u):
    DX = [0, 1, 1, 0, -1, -1]
    DYE = [-1, -1, 0, 1, 0, -1]
    DYO = [-1, 0, 1, 1, 1, 0]
    out = []
    for d in range(6):
        nx = u['x'] + DX[d]
        ny = u['y'] + (DYO if (u['x'] & 1) else DYE)[d]
        if 0 <= nx < W and 0 <= ny < H:
            out.append((nx, ny))
    return out

def at(nx, ny):
    for s in slots:
        if s['hp'] > 0 and s['x'] == nx and s['y'] == ny:
            return s
    return None

supA = supD = 0
for nx, ny in neighbor_slots(D):
    s = at(nx, ny)
    if s and s['slot'] != A['slot'] and s['owner'] == A['owner'] and not s['acted']:
        supA += atk_vs(units_data[s['type']], TD) * (s['hp'] + 1)
supA //= hA * 2
for nx, ny in neighbor_slots(A):
    s = at(nx, ny)
    if s and s['slot'] != D['slot'] and s['owner'] == D['owner'] and not s['acted']:
        supD += units_data[s['type']]['defense'] * (s['hp'] + 1)
supD //= hA * 2

# surround: all six defender neighbours adjacent to an idle attacker unit
surrounded = True
for nx, ny in neighbor_slots(D):
    cells = neighbor_slots({'x': nx, 'y': ny})
    covered = any(s['hp'] > 0 and s['owner'] == A['owner'] and not s['acted'] and
                  (s['x'], s['y']) in cells
                  for s in slots)
    if not covered:
        surrounded = False
        break

atkStr = baseA + supA
defStr = TD['defense'] + terD + supD
if surrounded: defStr //= 2
atkStr = min(100, atkStr)
defStr = min(100, defStr)

rankA = min(7, A['xp'] // 4)
rankD = min(7, D['xp'] // 4)
mult = [100, 105, 110, 120, 130, 140, 160, 200]
rawD = hA * (mult[rankA] * (((100 - defStr) * atkStr) // 100) // 100)
ctrBase = baseD
if surrounded: ctrBase //= 2
ctrDef = min(100, TA['defense'] + terA)
ctrBase = min(100, ctrBase)
rawA = hD * (mult[rankD] * (((100 - ctrDef) * ctrBase) // 100) // 100)

print('game : e4c=%d e50=%d e48=%d e4e=%d e55=%d e5a=%d e58=%d e5c=%d e4a=%d'
      % (state['e4c'], state['e50'], state['e48'], state['e4e'], state['e55'],
         state['e5a'], state['e58'], state['e5c'], state['e4a']))
print('calc : baseA=%d baseD=%d defA=%d defD=%d terA=%d terD=%d supA=%d supD=%d surround=%s'
      % (baseA, baseD, TA['defense'], TD['defense'], terA, terD, supA, supD, surrounded))
print('game : e52=%d e60=%d db6=%d db2=%d' % (state['e52'], state['e60'],
                                               state['db6'], state['db2']))
print('calc : rawDmgToDef=%d rawDmgToAtk=%d atkStr=%d defStr=%d ctrDef=%d'
      % (rawD, rawA, atkStr, defStr, ctrDef))

# Which variance multiplier reproduces the game's post-variance damage?
VARIANCES = sorted(set([15, 20, 2, 40] + [(150 - r) // 10 for r in range(100)]))
okD = [m for m in VARIANCES if m * rawD // 10 == state['e52']]
okA = [m for m in VARIANCES if m * rawA // 10 == state['e60']]
print('variance multipliers matching game damage: to defender %s, to attacker %s'
      % (okD, okA))

# Write a mission containing the exact state (slot indices preserved).
mission = {
    'name': 'combat_validation',
    'width': W, 'height': H, 'background': '',
    'terrain': terrain,
    'units': [{'type': s['type'], 'owner': s['owner'], 'x': s['x'], 'y': s['y'],
               'hp': s['hp'], 'xp': s['xp']} for s in slots],
}
json.dump(mission, open(os.path.join(ROOT, 'dump', '_validation_mission.json'), 'w'),
          indent=1)
print('wrote dump/_validation_mission.json  atk_slot=%d def_slot=%d mode=%d'
      % (A['slot'], D['slot'], mode))
