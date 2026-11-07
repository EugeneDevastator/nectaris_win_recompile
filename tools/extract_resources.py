import os, struct, json
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
b = open(os.path.join(ROOT, 'game', 'Nec.exe'), 'rb').read()
BASE_VA, BASE_OFF = 0x42a000, 0x27c00
def foff(va): return BASE_OFF + (va - BASE_VA)
def u8(va): return b[foff(va)]
DATA_BASE = 0x444348

class Bits:
    def __init__(self, data, pos):
        self.d = data; self.p = pos
        self.cur = self.d[self.p]; self.p += 1; self.left = 8
    def get(self, n):
        v = 0
        for _ in range(n):
            v = (v << 1) | ((self.cur >> 7) & 1)
            self.cur = (self.cur << 1) & 0xFF; self.left -= 1
            if self.left == 0:
                self.cur = self.d[self.p] if self.p < len(self.d) else 0
                self.p += 1; self.left = 8
        return v

def lz(data, n):
    br = Bits(data, 0); ring = bytearray([0x20] * 256); wr = 0xEF; out = []
    while len(out) < n:
        if br.get(1):
            c = br.get(8); out.append(c); ring[wr] = c; wr = (wr + 1) & 0xFF
        else:
            off = br.get(8); ln = br.get(4) + 2; src = off
            for _ in range(ln):
                c = ring[src & 0xFF]; src += 1; out.append(c); ring[wr] = c; wr = (wr + 1) & 0xFF
                if len(out) >= n: break
    return bytes(out[:n])

pal = json.load(open(os.path.join(ROOT, 'assets', 'art', 'palette.json')))
PAL = [(p['r'], p['g'], p['b']) for p in pal]
OUT = os.path.join(ROOT, 'dump', 'resources')
os.makedirs(OUT, exist_ok=True)

res = []
for rid in range(0, 80):
    o = (u8(0x4324d1 + rid*4) << 16) | (u8(0x4324d2 + rid*4) << 8) | u8(0x4324d3 + rid*4)
    t = u8(0x42bb88 + rid*4)
    p1 = u8(0x42bb89 + rid*4)
    p2 = u8(0x42bb8a + rid*4) or 256
    p3 = u8(0x42bb8b + rid*4) or 256
    size = max(0, (p2 - p3) * 16)
    if not o and not t:
        continue
    res.append((rid, t, p1, p2, p3, o, size))

print('id type p1 p2 p3 offset size')
for r in res:
    print('%2d %d %3d %3d %3d %#08x %6d' % r)

# decompress each and save
for rid, t, p1, p2, p3, o, size in res:
    src = b[foff(DATA_BASE + o):]
    try:
        raw = lz(src, max(size, 4096))
    except Exception as e:
        print('rid', rid, 'decode err', e); continue
    open(os.path.join(OUT, 'res_%02d.bin' % rid), 'wb').write(raw)
print('decompressed', len(res), 'resources')
