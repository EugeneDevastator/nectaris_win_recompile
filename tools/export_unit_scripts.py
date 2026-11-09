"""Export the static per-unit combat chunk scripts from Nec.exe.

Table: VA 0x42fee8, 32 pointers (one per unit type).
Scripts live in the contiguous blob starting at 0x42fce8; the next pointer ends
each script. Several unit types share one script.
"""
import os, struct, json

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
EXE = os.path.join(ROOT, 'game', 'Nec.exe')
OUT = os.path.join(ROOT, 'assets', 'data', 'combat_unit_scripts')
os.makedirs(OUT, exist_ok=True)
b = open(EXE, 'rb').read()
ib = 0x400000
secs = [(0x1000, 0x27400, 0x400, 0x27400), (0x2a000, 0x6c064, 0x27c00, 0x56200)]


def off(va):
    r = va - ib
    for sva, vs, rp, rs in secs:
        if sva <= r < sva + max(vs, rs):
            return rp + (r - sva)
    raise ValueError(hex(va))


units = json.load(open(os.path.join(ROOT, 'assets', 'data', 'units.json'), encoding='utf-8'))['units']
names = {u['id']: u['name'] for u in units}

ptrs = [struct.unpack_from('<I', b, off(0x42fee8) + t * 4)[0] for t in range(23)]
uniq = sorted(set(ptrs))
# script end = next unique pointer (or end of data)
bounds = {}
for i, p in enumerate(uniq):
    end = uniq[i + 1] if i + 1 < len(uniq) else 0x42fee8
    bounds[p] = end

manifest = {'table_va': '0x42fee8', 'scripts': [], 'units': []}
for p in uniq:
    data = b[off(p):off(bounds[p])]
    fn = 'script_%08x.bin' % p
    open(os.path.join(OUT, fn), 'wb').write(data)
    manifest['scripts'].append({'va': hex(p), 'bytes': len(data), 'file': fn,
                                'hex': data.hex()})
for t in range(23):
    manifest['units'].append({'unit': t, 'name': names.get(t, '?'),
                              'script_va': hex(ptrs[t]),
                              'script': 'script_%08x.bin' % ptrs[t]})

json.dump(manifest, open(os.path.join(OUT, 'manifest.json'), 'w'), indent=1)
print('unique scripts', len(uniq))
for t in range(23):
    p = ptrs[t]
    print('unit %2d %-8s script %s (%d bytes)' % (t, names.get(t, '?'), hex(p), bounds[p] - p))
print('wrote', OUT)
