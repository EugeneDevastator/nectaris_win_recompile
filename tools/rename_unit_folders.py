"""Give every unit folder a readable name suffix (unit_02_ax87).

The type id stays first so sorting and the port's id lookup still work; the
suffix is the sanitised unit name from unit.json.  Updates each unit.json's
"folder" field and assets/data/units.json.  Safe to re-run.
"""
import json
import os
import re

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
UNITS = os.path.join(ROOT, 'assets', 'units')
DATA = os.path.join(ROOT, 'assets', 'data', 'units.json')


def main():
    renamed = {}
    for name in sorted(os.listdir(UNITS)):
        m = re.match(r'unit_(\d\d)$', name)
        if not m:
            continue
        src = os.path.join(UNITS, name)
        uj = os.path.join(src, 'unit.json')
        if not os.path.exists(uj):
            continue
        data = json.load(open(uj, encoding='utf-8'))
        suffix = re.sub(r'[^a-z0-9]', '', str(data.get('name', '')).lower())
        if not suffix:
            continue
        dst_name = 'unit_%s_%s' % (m.group(1), suffix)
        dst = os.path.join(UNITS, dst_name)
        if os.path.abspath(src) != os.path.abspath(dst):
            os.rename(src, dst)
        data['folder'] = dst_name
        with open(os.path.join(dst, 'unit.json'), 'w', encoding='utf-8') as f:
            json.dump(data, f, ensure_ascii=False, indent=1)
        renamed[int(m.group(1))] = dst_name
        print('%s -> %s' % (name, dst_name))

    units = json.load(open(DATA, encoding='utf-8'))
    for u in units.get('units', []):
        if u['id'] in renamed:
            u['folder'] = renamed[u['id']]
    with open(DATA, 'w', encoding='utf-8') as f:
        json.dump(units, f, ensure_ascii=False, indent=1)
    print('updated', DATA)


if __name__ == '__main__':
    main()
