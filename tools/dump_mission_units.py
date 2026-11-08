"""Dump campaign squad composition for every mission from PTR_DAT_0046b988."""
import struct, os
from collections import Counter

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
data = open(os.path.join(ROOT, 'game', 'Nec.exe'), 'rb').read()
ib = 0x400000
secs = [(0x1000, 0x27400, 0x400, 0x27400), (0x2a000, 0x6c064, 0x27c00, 0x56200)]

def off(va):
    rva = va - ib
    for sva, vsize, rawptr, rawsize in secs:
        if sva <= rva < sva + max(vsize, rawsize):
            return rawptr + (rva - sva)
    raise ValueError(hex(va))

def u32(va): return struct.unpack('<I', data[off(va):off(va)+4])[0]
def u8(va): return data[off(va)]

NAME = []
for i in range(32):
    o = off(0x42a9c8 + i * 8)
    NAME.append(data[o:o+8].split(b'\0')[0].decode('latin1'))

NAMES = ['M-77','FX-1','AX-87','EF-88','T-79','PT-6','S-61','GS-81','GT-86','HMB-2',
         'TT-1','SG-4','MR-22','MB-5','MB-4','AAG-4','MM107','SS-80','GX-77','GX-87',
         'CBX-1','NC-1','C-61','DUMMY']

PTR = 0x46b988
missions = []
for mission in range(32):
    p = u32(PTR + mission * 4)
    if not (0x400000 <= p < 0x470000):
        missions.append(None); continue
    o = off(p)
    units = []
    for i in range(56):
        b = data[o + i*3:o + i*3 + 3]
        if len(b) < 3 or b[0] == 0xFF:
            break
        units.append({'type': b[0] & 0x1F, 'owner': 1 if (b[0] & 0x80) else 0,
                      'x': b[1] & 0x1F, 'y': b[2] & 0x1F, 'rawx': b[1], 'rawy': b[2]})
    missions.append(units)

print('%-3s %-8s %4s %4s %4s  composition' % ('#', 'name', 'all', 'P1', 'P2'))
for m, units in enumerate(missions):
    if units is None:
        print('%-3d %-8s  (no data)' % (m, NAME[m])); continue
    own = Counter(u['owner'] for u in units)
    comp = Counter()
    for u in units:
        nm = NAMES[u['type']] if u['type'] < len(NAMES) else ('T%d' % u['type'])
        comp[(u['owner'], nm)] += 1
    s = ', '.join('%sx%d' % (nm, n) for (ow, nm), n in sorted(comp.items()))
    print('%-3d %-8s %4d %4d %4d  %s' % (m, NAME[m], len(units), own.get(0,0), own.get(1,0), s))

print('\n=== mission 10 (index 9, DARWIN) unit positions ===')
for u in missions[9] or []:
    nm = NAMES[u['type']] if u['type'] < len(NAMES) else ('T%d' % u['type'])
    print('  %-8s owner=%d x=%2d y=%2d  (raw %02x %02x)' %
          (nm, u['owner'], u['x'], u['y'], u['rawx'], u['rawy']))
