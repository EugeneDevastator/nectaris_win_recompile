"""Give every unit folder its readable code + human name suffix.

Examples:
    unit_00_m77_trigger
    unit_06_s61_bison
    unit_09_hmb2_giant

The type id stays first so sorting and the port's id lookup still work.
Updates each unit.json's "folder" field and assets/data/units.json. Safe to
re-run.
"""
import json
import os
import re

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
UNITS = os.path.join(ROOT, 'assets', 'units')
DATA = os.path.join(ROOT, 'assets', 'data', 'units.json')

SLUGS = {
    0: 'm77_trigger', 1: 'fx1_falcon', 2: 'ax87_eagle', 3: 'ef88_hunter',
    4: 't79_grizzly', 5: 'pt6_polar', 6: 's61_bison', 7: 'gs81_slagger',
    8: 'gt86_titan', 9: 'hmb2_giant', 10: 'tt1_lenet', 11: 'sg4_hadrian',
    12: 'mr22_octopus', 13: 'mb5_rabbit', 14: 'mb4_lynx', 15: 'aag4_seeker',
    16: 'mm107_hawkeye', 17: 'ss80_atlas', 18: 'gx77_charlie',
    19: 'gx87_kilroy', 20: 'cbx1_panther', 21: 'nc1_mule', 22: 'c61_pelican',
    23: 'yamaarashi',
}


def main():
    renamed = {}
    for name in sorted(os.listdir(UNITS)):
        m = re.match(r'unit_(\d\d)(?:_|$)', name)
        if not m:
            continue
        uid = int(m.group(1))
        src = os.path.join(UNITS, name)
        if not os.path.isdir(src):
            continue
        slug = SLUGS.get(uid)
        if not slug:
            uj = os.path.join(src, 'unit.json')
            if os.path.exists(uj):
                data = json.load(open(uj, encoding='utf-8'))
                slug = re.sub(r'[^a-z0-9]+', '_',
                              str(data.get('name', '')).lower()).strip('_')
        if not slug:
            continue
        dst_name = 'unit_%02d_%s' % (uid, slug)
        dst = os.path.join(UNITS, dst_name)
        if os.path.abspath(src) != os.path.abspath(dst) and not os.path.exists(dst):
            os.rename(src, dst)
            print('%s -> %s' % (name, dst_name))
        data = {}
        uj = os.path.join(dst, 'unit.json')
        if os.path.exists(uj):
            data = json.load(open(uj, encoding='utf-8'))
        data['folder'] = dst_name
        with open(uj, 'w', encoding='utf-8') as f:
            json.dump(data, f, ensure_ascii=False, indent=1)
        renamed[uid] = dst_name

    units = json.load(open(DATA, encoding='utf-8'))
    for u in units.get('units', []):
        if u['id'] in renamed:
            u['folder'] = renamed[u['id']]
    with open(DATA, 'w', encoding='utf-8') as f:
        json.dump(units, f, ensure_ascii=False, indent=1)
    print('updated', DATA)


if __name__ == '__main__':
    main()
