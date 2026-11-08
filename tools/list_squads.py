"""List unit counts / squads from every NMD map and deployment file."""
import os, glob, struct
from collections import Counter

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))

def parse(path):
    b = open(path, 'rb').read()
    if len(b) < 19 or not b.startswith(b'NEC MAP'):
        return None
    w, h = b[16], b[17]
    off = 19 + w * h
    units = []
    while off + 2 < len(b):
        if b[off] == 0xFF:
            break
        t, x, y = b[off], b[off + 1], b[off + 2]
        units.append({'type': t & 0x1F, 'owner': 1 if (t & 0x80) else 0,
                      'x': x & 0x1F, 'y': y & 0x1F})
        off += 3
    return {'w': w, 'h': h, 'units': units, 'bytes': len(b)}

rows = []
for p in sorted(glob.glob(os.path.join(ROOT, 'game', 'MAPDATA', '*', '*.NMD'))):
    folder = os.path.basename(os.path.dirname(p))
    name = os.path.splitext(os.path.basename(p))[0]
    m = parse(p)
    if not m:
        continue
    own = Counter(u['owner'] for u in m['units'])
    types = Counter(u['type'] for u in m['units'])
    rows.append((folder, name, m['w'], m['h'], m['bytes'], len(m['units']),
                 own.get(0, 0), own.get(1, 0), types))

print('%-28s %-7s %3s %3s %5s %4s %4s %4s  types' %
      ('folder', 'map', 'w', 'h', 'bytes', 'all', 'p0', 'p1'))
for folder, name, w, h, nb, n, p0, p1, types in rows:
    print('%-28s %-7s %3d %3d %5d %4d %4d %4d  %s' %
          (folder[:28], name, w, h, nb, n, p0, p1,
           dict(sorted(types.items()))))
