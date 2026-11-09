"""Reference implementation of the game's LZSS and compare with game output."""
import os, struct, sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
b = open(os.path.join(ROOT, 'game', 'Nec.exe'), 'rb').read()
ib = 0x400000
secs = [(0x1000, 0x27400, 0x400, 0x27400), (0x2a000, 0x6c064, 0x27c00, 0x56200)]


def off(va):
    r = va - ib
    for sva, vs, rp, rs in secs:
        if sva <= r < sva + max(vs, rs):
            return rp + (r - sva)
    raise ValueError(hex(va))


def comp_ptr(p1):
    o = (b[off(0x4324d1 + p1 * 4)] << 16) | (b[off(0x4324d2 + p1 * 4)] << 8) | b[off(0x4324d3 + p1 * 4)]
    return off(0x444348) + o


def lz(src, n):
    win = bytearray([0x20] * 256)
    wp = 0xEF
    rp = 0
    bits = 8
    cur = src[0]
    pos = 1
    remain = 0

    def rb(k):
        nonlocal bits, cur, pos
        v = 0
        for _ in range(k):
            v = (v << 1) | ((cur >> 7) & 1)
            cur = (cur << 1) & 0xFF
            bits -= 1
            if bits == 0:
                cur = src[pos] if pos < len(src) else 0
                pos += 1
                bits = 8
        return v

    out = bytearray()
    while len(out) < n:
        if remain == 0:
            if rb(1):
                c = rb(8)
                win[wp] = c
                wp = (wp + 1) & 0xFF
                out.append(c)
                continue
            rp = rb(8)
            remain = rb(4) + 2
        c = win[rp]
        rp = (rp + 1) & 0xFF
        remain -= 1
        win[wp] = c
        wp = (wp + 1) & 0xFF
        out.append(c)
    return bytes(out)


p1 = int(sys.argv[1]) if len(sys.argv) > 1 else 44
n = int(sys.argv[2]) if len(sys.argv) > 2 else 0x10000
ptr = comp_ptr(p1)
print('p1', p1, 'compressed at file off', hex(ptr), 'bytes', b[ptr:ptr + 16].hex(' '))
data = lz(b[ptr:], n)
open(os.path.join(ROOT, 'dump', 'game_streams', 'lz_p1_%03d.bin' % p1), 'wb').write(data)
g = os.path.join(ROOT, 'dump', 'game_streams', 'res_044.bin')
if os.path.exists(g):
    gd = open(g, 'rb').read()
    print('match first512:', data[:512] == gd[:512])
    print('ours ', data[:32].hex(' '))
    print('game ', gd[:32].hex(' '))
print('nonzero ours', sum(1 for x in data if x))
