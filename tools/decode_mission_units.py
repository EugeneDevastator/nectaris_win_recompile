"""Decode per-mission unit placement grids from PTR_DAT_0047f750."""
import struct, os

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

def u32(va):
    o = va_to_off(va)
    return struct.unpack('<I', data[o:o + 4])[0]

def u8(va):
    return data[va_to_off(va)]

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
for i in range(32):
    o = va_to_off(0x42a9c8 + i * 8)
    NAME.append(data[o:o + 8].split(b'\0')[0].decode('latin1'))

PTR = 0x47f750
for mission in range(32):
    c52 = mission % 16
    w, h = W[c52], H[c52]
    p = u32(PTR + mission * 4)
    try:
        o = va_to_off(p)
    except ValueError:
        print('%2d %-8s %2dx%-2d ptr=%#x (bad)' % (mission, NAME[mission], w, h, p))
        continue
    # read a generous compressed window
    src = data[o:o + 20000]
    grid = lzss(src, w * h)
    from collections import Counter
    c = Counter(grid)
    print('%2d %-8s ch=%d map=%d %2dx%-2d ptr=%#x len=%d  %s' %
          (mission, NAME[mission], mission // 16, mission % 16, w, h, p, len(grid),
           dict(sorted(c.items())[:20])))
    if mission == 9:
        print('   grid (hex, row-major):')
        for y in range(h):
            print('   ' + ' '.join('%02x' % grid[y * w + x] for x in range(w)))
