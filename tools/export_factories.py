"""Export the original factory table and stored unit rosters.

Static data:
  DAT_0047f7d0 + mission*0x10   up to 8 (x,y) factory positions, 0xff terminator
  unit records                  xraw & 0xe0 != 0, group = xraw >> 5 (1-based)

The factory's terrain class gives its owner:
  0xb = player factory, 0xc = neutral factory, 0xd = enemy factory.
A factory stores up to 12 units (the 4x3 screen).  There is no unit production:
captured/stored units are free, and a stored damaged unit is repaired to 8 HP.
"""
import os, struct, json
from collections import Counter

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
data = open(os.path.join(ROOT, 'game', 'Nec.exe'), 'rb').read()
ib = 0x400000
secs = [(0x1000, 0x27400, 0x400, 0x27400), (0x2a000, 0x6c064, 0x27c00, 0x56200)]


def va_to_off(va):
    rva = va - ib
    for sva, vsize, rawptr, rawsize in secs:
        if sva <= rva < sva + max(vsize, rawsize):
            return rawptr + (rva - sva)
    raise ValueError(hex(va))


def u8(va):
    return data[va_to_off(va)]


def u32(va):
    return struct.unpack('<I', data[va_to_off(va):va_to_off(va) + 4])[0]


def lzss(src, n):
    ring = bytearray(b' ' * 256)
    out = bytearray()
    pos = 0xef
    sp = 1
    bitbyte = src[0] if src else 0
    bits = 8
    match_src = 0
    match_left = 0

    def rb(k):
        nonlocal sp, bitbyte, bits
        c = 0
        for _ in range(k):
            c = (c << 1) & 0xff
            if bitbyte & 0x80:
                c |= 1
            bitbyte = (bitbyte << 1) & 0xff
            bits -= 1
            if bits == 0:
                if sp >= len(src):
                    return None
                bitbyte = src[sp]
                sp += 1
                bits = 8
        return c

    while len(out) < n:
        if match_left == 0:
            flag = rb(1)
            if flag is None:
                break
            if flag:
                c = rb(8)
                if c is None:
                    break
                ring[pos] = c
                pos = (pos + 1) & 0xff
                out.append(c)
                continue
            match_src = rb(8)
            ln = rb(4)
            if match_src is None or ln is None:
                break
            match_left = ln + 2
        c = ring[match_src & 0xff]
        match_src = (match_src + 1) & 0xff
        match_left -= 1
        ring[pos] = c
        pos = (pos + 1) & 0xff
        out.append(c)
    return bytes(out)


NAMES = ['M-77', 'FX-1', 'AX-87', 'EF-88', 'T-79', 'PT-6', 'S-61', 'GS-81',
         'GT-86', 'HMB-2', 'TT-1', 'SG-4', 'MR-22', 'MB-5', 'MB-4', 'AAG-4',
         'MM107', 'SS-80', 'GX-77', 'GX-87', 'CBX-1', 'NC-1', 'C-61', 'DUMMY']
W = [u8(0x42a9a0 + i) for i in range(16)]
H = [u8(0x42a9b0 + i) for i in range(16)]
MISSION_NAMES = []
for i in range(16):
    o = va_to_off(0x42a9c8 + i * 8)
    MISSION_NAMES.append(data[o:o + 8].split(b'\0')[0].decode('latin1'))

FACTORY_OWNER = {0xb: 'player', 0xc: 'neutral', 0xd: 'enemy'}

out = {
    'capacity': 12,
    'automatic_production': False,
    'capture': 'infantry only (GX-77, GX-87, CBX-1)',
    'repair': {'stored_turns': 1, 'hp': 8},
    'note': 'Factories do not produce new units. Captured/stored units are free; '
            'a damaged unit moved into a factory is repaired to 8 HP and can be '
            'deployed again on the following turn.',
    'missions': [],
}

for m in range(16):
    w, h = W[m], H[m]
    p = u32(0x47f750 + m * 4)
    grid = lzss(data[va_to_off(p):va_to_off(p) + 20000], w * h)
    terrain = [[grid[y * w + x] for x in range(w)] for y in range(h)]

    # Static factory locations.
    factories = []
    ftab = va_to_off(0x47f7d0 + m * 0x10)
    for i in range(8):
        fx, fy = data[ftab + i * 2], data[ftab + i * 2 + 1]
        if fx == 0xff and fy == 0xff:
            break
        if fx >= w or fy >= h:
            continue
        t = terrain[fy][fx]
        factories.append({
            'group': i + 1,
            'x': fx,
            'y': fy,
            'terrain': t,
            'owner': FACTORY_OWNER.get(t, 'unknown'),
            'capacity': 12,
            'roster': [],
        })

    # Attach reserve unit records to their factory group.  Records whose x/y do
    # not match a factory are separate reinforcement spawns (usually on grass or
    # a road), so keep them apart instead of silently losing them.
    reinforcements = []
    by_pos = {(f['x'], f['y'], f['group']): f for f in factories}
    up = u32(0x46b988 + m * 4)
    o = va_to_off(up)
    for i in range(0x38):
        rec = data[o + i * 3:o + i * 3 + 3]
        if len(rec) < 3 or rec[0] == 0xff:
            break
        typ = rec[0] & 0x1f
        xraw = rec[1]
        if not (xraw & 0xe0):
            continue
        group = xraw >> 5
        x, y = xraw & 0x1f, rec[2] & 0x1f
        f = by_pos.get((x, y, group))
        item = {
            'type': typ,
            'name': NAMES[typ] if typ < len(NAMES) else f'T{typ}',
            'record_owner': 1 if (rec[0] & 0x80) else 0,
        }
        if f is not None:
            f['roster'].append(item)
        else:
            item.update({'group': group, 'x': x, 'y': y})
            reinforcements.append(item)

    out['missions'].append({
        'mission': m + 1,
        'name': MISSION_NAMES[m],
        'width': w,
        'height': h,
        'factories': factories,
        'reinforcements': reinforcements,
    })

path = os.path.join(ROOT, 'assets', 'data', 'factories.json')
json.dump(out, open(path, 'w', encoding='utf-8'), indent=2)

print('mission  factories  rosters')
for mi in out['missions']:
    print('%2d %-8s %d' % (mi['mission'], mi['name'], len(mi['factories'])))
    for f in mi['factories']:
        comp = Counter(u['name'] for u in f['roster'])
        print('   #%d (%2d,%2d) %-7s %2d/%-2d  %s' %
              (f['group'], f['x'], f['y'], f['owner'], len(f['roster']),
               f['capacity'], ', '.join('%s x%d' % (k, v) for k, v in comp.items())))
    if mi['reinforcements']:
        rg = {}
        for u in mi['reinforcements']:
            rg.setdefault((u['group'], u['x'], u['y']), []).append(u['name'])
        for (g, x, y), names in sorted(rg.items()):
            print('   reinforcement group %d @(%d,%d): %s' %
                  (g, x, y, ', '.join(names)))

print('\nwrote', path)
