"""Add the StrategyWiki human-readable names to units and folders.

Data source: https://strategywiki.org/wiki/Military_Madness/Units

Examples:
  0 M-77 -> "M-77 Trigger" -> unit_00_m77_trigger
  9 HMB-2 -> "HMB-2 Giant" -> unit_09_hmb2_giant
"""
import json
import os
import re

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
UNITS = os.path.join(ROOT, 'assets', 'units')
DATA = os.path.join(ROOT, 'assets', 'data', 'units.json')
VFX = os.path.join(ROOT, 'assets', 'data', 'combat_vfx.json')

NAMES = {
    0: ('M-77 Trigger', 'm77_trigger'),
    1: ('FX-1 Falcon', 'fx1_falcon'),
    2: ('AX-87 Eagle', 'ax87_eagle'),
    3: ('EF-88 Hunter', 'ef88_hunter'),
    4: ('T-79 Grizzly', 't79_grizzly'),
    5: ('PT-6 Polar', 'pt6_polar'),
    6: ('S-61 Bison', 's61_bison'),
    7: ('GS-81 Slagger', 'gs81_slagger'),
    8: ('GT-86 Titan', 'gt86_titan'),
    9: ('HMB-2 Giant', 'hmb2_giant'),
    10: ('TT-1 Lenet', 'tt1_lenet'),
    11: ('SG-4 Hadrian', 'sg4_hadrian'),
    12: ('MR-22 Octopus', 'mr22_octopus'),
    13: ('MB-5 Rabbit', 'mb5_rabbit'),
    14: ('MB-4 Lynx', 'mb4_lynx'),
    15: ('AAG-4 Seeker', 'aag4_seeker'),
    16: ('MM-107 Hawkeye', 'mm107_hawkeye'),
    17: ('SS-80 Atlas', 'ss80_atlas'),
    18: ('GX-77 Charlie', 'gx77_charlie'),
    19: ('GX-87 Kilroy', 'gx87_kilroy'),
    20: ('CBX-1 Panther', 'cbx1_panther'),
    21: ('NC-1 Mule', 'nc1_mule'),
    22: ('C-61 Pelican', 'c61_pelican'),
}

units = json.load(open(DATA, encoding='utf-8'))
for u in units.get('units', []):
    uid = u['id']
    if uid not in NAMES:
        continue
    full, slug = NAMES[uid]
    target = 'unit_%02d_%s' % (uid, slug)
    old_dirs = [d for d in os.listdir(UNITS)
                if re.match(r'unit_%02d(?:_|$)' % uid, d)]
    target_path = os.path.join(UNITS, target)
    if not os.path.isdir(target_path):
        if not old_dirs:
            print('missing folder for', uid)
            continue
        src = os.path.join(UNITS, sorted(old_dirs, key=len)[-1])
        os.rename(src, target_path)
        print('%s -> %s' % (os.path.basename(src), target))
    for d in old_dirs:
        p = os.path.join(UNITS, d)
        if d != target and os.path.isdir(p):
            # Leave an unexpected extra directory alone; the resolver prefers
            # the longest name, but report it.
            print('extra folder (ignored):', d)
    u['name'] = full
    u['folder'] = target
    uj = os.path.join(target_path, 'unit.json')
    if os.path.exists(uj):
        j = json.load(open(uj, encoding='utf-8'))
        j['name'] = full
        j['folder'] = target
        with open(uj, 'w', encoding='utf-8') as f:
            json.dump(j, f, ensure_ascii=False, indent=1)

with open(DATA, 'w', encoding='utf-8') as f:
    json.dump(units, f, ensure_ascii=False, indent=1)

if os.path.exists(VFX):
    vj = json.load(open(VFX, encoding='utf-8'))
    for k, v in vj.get('units', {}).items():
        uid = int(k)
        if uid in NAMES:
            v['name'] = NAMES[uid][0]
    with open(VFX, 'w', encoding='utf-8') as f:
        json.dump(vj, f, ensure_ascii=False, indent=1)

print('updated unit names and folders')
