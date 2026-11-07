"""Render one combat panel with the live combat palette captured from the game
(dump/battlecap/f000_pal.bin).  Used to validate the terrain palette."""
import os, sys, struct
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
b = open(os.path.join(ROOT, 'game', 'Nec.exe'), 'rb').read()
pal_raw = open(os.path.join(ROOT, 'dump', 'palette_battle.bin'), 'rb').read()
PAL = [(pal_raw[4 + i * 4], pal_raw[5 + i * 4], pal_raw[6 + i * 4]) for i in range(256)]
tb = open(os.path.join(ROOT, 'dump', 'tilebank_battle.bin'), 'rb').read()
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
                self.cur = self.d[self.p] if self.p < len(self.d) else 0
                self.p += 1; self.left = 8
        return v

def lz(data, start, n_out):
    br = Bits(data, start); ring = bytearray([0x20] * 256); wr = 0xEF; out = []
    while len(out) < n_out:
        if br.get(1):
            c = br.get(8); out.append(c); ring[wr] = c; wr = (wr + 1) & 0xFF
        else:
            off = br.get(8); ln = br.get(4) + 2; src = off
            for _ in range(ln):
                c = ring[src & 0xFF]; src += 1; out.append(c); ring[wr] = c; wr = (wr + 1) & 0xFF
                if len(out) >= n_out: break
    return bytes(out[:n_out])

ptrs = [struct.unpack_from('<I', b, foff(0x431148) + i * 4)[0] for i in range(24)]

def render(i, key_index=0x18, bank=1):
    raw = lz(b, foff(ptrs[i]), 512)
    img = Image.new('RGBA', (256, 256), (255, 0, 255, 255))
    px = img.load()
    for ty in range(16):
        for tx in range(16):
            e = struct.unpack_from('<H', raw, (ty * 16 + tx) * 2)[0]
            tile = e & 0x7ff
            base = tile * 256
            for y in range(16):
                for x in range(16):
                    v = tb[base + y * 16 + x]
                    if v == 0:
                        continue
                    idx = ((bank << 4) | (v & 0xF)) & 0xFF
                    if key_index is not None and idx == key_index:
                        continue
                    px[tx * 16 + x, ty * 16 + (15 - y)] = PAL[idx] + (255,)
    return img

if __name__ == '__main__':
    outdir = os.path.join(ROOT, 'dump', 'panels_live')
    os.makedirs(outdir, exist_ok=True)
    for i in range(24):
        render(i).save(os.path.join(outdir, 'panel_%02d.png' % i))
    print('wrote', outdir)
