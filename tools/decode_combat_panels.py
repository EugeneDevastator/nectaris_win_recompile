import struct, os
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
b = open(os.path.join(ROOT, 'game', 'Nec.exe'), 'rb').read()
bgr = open(os.path.join(ROOT, 'dump', 'palette_bgr.bin'), 'rb').read()
PAL = [(bgr[i*4+2], bgr[i*4+1], bgr[i*4]) for i in range(256)]
tb_path = os.path.join(ROOT, 'dump', 'tilebank_battle.bin')
if not os.path.exists(tb_path):
    tb_path = os.path.join(ROOT, 'dump', 'tilebank.bin')
tb = open(tb_path, 'rb').read()

BASE_VA, BASE_OFF = 0x42a000, 0x27c00
def foff(va): return BASE_OFF + (va - BASE_VA)

class Bits:
    def __init__(self, data, pos):
        self.d = data; self.p = pos
        self.cur = self.d[self.p]; self.p += 1
        self.left = 8
    def get(self, n):
        v = 0
        for _ in range(n):
            v = (v << 1) | ((self.cur >> 7) & 1)
            self.cur = (self.cur << 1) & 0xFF
            self.left -= 1
            if self.left == 0:
                if self.p < len(self.d): self.cur = self.d[self.p]
                else: self.cur = 0
                self.p += 1
                self.left = 8
        return v

def lz_decompress(data, start, n_out):
    br = Bits(data, start)
    ring = bytearray([0x20]*256)
    wr = 0xEF
    out = []
    while len(out) < n_out:
        if br.get(1):
            c = br.get(8)
            out.append(c); ring[wr] = c; wr = (wr+1) & 0xFF
        else:
            off = br.get(8)
            ln = br.get(4) + 2
            src = off
            for _ in range(ln):
                c = ring[src & 0xFF]; src += 1
                out.append(c); ring[wr] = c; wr = (wr+1) & 0xFF
                if len(out) >= n_out: break
    return bytes(out[:n_out])

# The game uses palette index 0x18 (bank 1, entry 8) as the magenta colour key
# for transparent pixels in the combat terrain panels, and blank (0) pixels too.
TRANSPARENT_INDEX = 0x18

outdir = os.path.join(ROOT, 'assets', 'art', 'combat_terrain')
os.makedirs(outdir, exist_ok=True)
ptrs = [struct.unpack_from('<I', b, foff(0x431148) + i*4)[0] for i in range(24)]
for i, p in enumerate(ptrs):
    raw = lz_decompress(b, foff(p), 512)
    img = Image.new('RGBA', (256, 256), (0, 0, 0, 0))
    px = img.load()
    for ty in range(16):
        for tx in range(16):
            e = struct.unpack_from('<H', raw, (ty*16+tx)*2)[0]
            tile = e & 0x7ff; bank = (e >> 0xb) + 1
            base = tile*256
            for y in range(16):
                for x in range(16):
                    v = tb[base + y*16 + x]
                    if v == 0: continue
                    idx = ((bank << 4) | (v & 0xF)) & 0xFF
                    if idx == TRANSPARENT_INDEX: continue
                    # memory tiles are bottom-up: flip inside each 16x16 tile
                    px[tx*16+x, ty*16+(15-y)] = PAL[idx] + (255,)
    img.save(os.path.join(outdir, f'panel_{i:02d}.png'))
print('decoded', len(ptrs), 'panels (RGBA, colour-keyed)')
