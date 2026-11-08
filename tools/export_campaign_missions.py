"""Export the 16 campaign missions (chapter 0) to port map JSONs.

Terrain: PTR_DAT_0047f750[mission], LZSS-compressed grid of terrain classes
         (0..13), width/height from DAT_0042a9a0 / DAT_0042a9b0.
Units:   PTR_DAT_0046b988[mission], 3-byte records [type|owner, x, y], 0xff end.
         x & 0xe0 != 0 marks a unit inside a factory/base (garrison/reserve).
Names:   DAT_0042a9c8 + mission*8.
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

def u32(va): return struct.unpack('<I', data[va_to_off(va):va_to_off(va)+4])[0]
def u8(va): return data[va_to_off(va)]

def lzss(src, n):
    ring = bytearray(b' ' * 256)
    out = bytearray(); pos = 0xef; sp = 1
    bitbyte = src[0] if src else 0; bits = 8
    match_src = 0; match_left = 0
    def rb(k):
        nonlocal sp, bitbyte, bits
        c = 0
        for _ in range(k):
            c = (c << 1) & 0xff
            if bitbyte & 0x80: c |= 1
            bitbyte = (bitbyte << 1) & 0xff
            bits -= 1
            if bits == 0:
                if sp >= len(src): return None
                bitbyte = src[sp]; sp += 1; bits = 8
        return c
    while len(out) < n:
        if match_left == 0:
            flag = rb(1)
            if flag is None: break
            if flag:
                c = rb(8)
                if c is None: break
                ring[pos] = c; pos = (pos + 1) & 0xff; out.append(c); continue
            match_src = rb(8); ln = rb(4)
            if match_src is None or ln is None: break
            match_left = ln + 2
        c = ring[match_src & 0xff]
        match_src = (match_src + 1) & 0xff
        match_left -= 1
        ring[pos] = c; pos = (pos + 1) & 0xff; out.append(c)
    return bytes(out)

W = [u8(0x42a9a0 + i) for i in range(16)]
H = [u8(0x42a9b0 + i) for i in range(16)]
NAME = []
for i in range(16):
    o = va_to_off(0x42a9c8 + i * 8)
    NAME.append(data[o:o+8].split(b'\0')[0].decode('latin1'))

UNITS = ['M-77','FX-1','AX-87','EF-88','T-79','PT-6','S-61','GS-81','GT-86','HMB-2',
         'TT-1','SG-4','MR-22','MB-5','MB-4','AAG-4','MM107','SS-80','GX-77','GX-87',
         'CBX-1','NC-1','C-61','DUMMY','FALCON','JAVI','HUNTER','GRIZZLY','ARMADILLO','PYTHON',
         'SLUGGER','MONSTER','GIGANT','RENET','NASCHORN','ESTRE','RABBIT','LYNX','SEEKER','HAWKEYE']

OUT = os.path.join(ROOT, 'assets', 'maps')
INDEX = []
for m in range(16):
    w, h = W[m], H[m]
    p = u32(0x47f750 + m * 4)
    grid = lzss(data[va_to_off(p):va_to_off(p) + 20000], w * h)
    terrain = [[grid[y * w + x] for x in range(w)] for y in range(h)]

    up = u32(0x46b988 + m * 4)
    units = []
    garrison = []
    o = va_to_off(up)
    for i in range(0x38):
        b = data[o + i*3:o + i*3 + 3]
        if len(b) < 3 or b[0] == 0xFF:
            break
        typ = b[0] & 0x1F
        xraw = b[1]
        u = {'type': typ, 'owner': 1 if (b[0] & 0x80) else 0,
             'x': xraw & 0x1F, 'y': b[2] & 0x1F,
             'name': UNITS[typ] if typ < len(UNITS) else 'T%d' % typ}
        if xraw & 0xE0:
            u['reserve'] = True
            u['group'] = xraw >> 5
            garrison.append(u)
        else:
            units.append(u)

    mission = {
        'name': NAME[m],
        'chapter': 0,
        'map': m,
        'mission': m + 1,
        'width': w,
        'height': h,
        'background': '',
        'terrain': terrain,
        'units': units,
        'garrison': garrison,
    }
    fn = 'campaign_%02d_%s.json' % (m + 1, NAME[m].lower())
    json.dump(mission, open(os.path.join(OUT, fn), 'w', encoding='utf-8'), indent=1)

    own = Counter(u['owner'] for u in units)
    INDEX.append({
        'mission': m + 1, 'name': NAME[m], 'file': fn, 'width': w, 'height': h,
        'units': len(units), 'player': own.get(0, 0), 'enemy': own.get(1, 0),
        'garrison': len(garrison),
        'composition': dict(Counter(u['name'] for u in units)),
        'garrison_composition': dict(Counter(u['name'] for u in garrison)),
    })
    print('mission %2d %-7s %2dx%-2d field=%2d (P%d/E%d) garrison=%2d  %s' %
          (m + 1, NAME[m], w, h, len(units), own.get(0, 0), own.get(1, 0), len(garrison),
           ', '.join('%s x%d' % (k, v) for k, v in sorted(Counter(u['name'] for u in units).items()))))

json.dump({'missions': INDEX}, open(os.path.join(ROOT, 'assets', 'data', 'campaign_missions.json'),
                                   'w', encoding='utf-8'), indent=2)
print('\nwrote', len(INDEX), 'campaign maps + assets/data/campaign_missions.json')
