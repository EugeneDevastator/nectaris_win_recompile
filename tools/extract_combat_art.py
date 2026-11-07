import struct, os
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
b = open(os.path.join(ROOT, 'game', 'Nec.exe'), 'rb').read()
bgr = open(os.path.join(ROOT, 'dump', 'palette_bgr.bin'), 'rb').read()
PAL = [(bgr[i*4+2], bgr[i*4+1], bgr[i*4]) for i in range(256)]
tb = open(os.path.join(ROOT, 'dump', 'tilebank.bin'), 'rb').read()

BASE_VA, BASE_OFF = 0x42a000, 0x27c00
def foff(va): return BASE_OFF + (va - BASE_VA)

outdir = os.path.join(ROOT, 'assets', 'art', 'combat_terrain')
os.makedirs(outdir, exist_ok=True)

# PTR_DAT_00431148
ptrs = [struct.unpack_from('<I', b, foff(0x431148) + i*4)[0] for i in range(12)]
print('tileset pointers:', [hex(p) for p in ptrs])

for i, p in enumerate(ptrs):
    if not (0x42a000 <= p < 0x496000):
        print(i, 'bad pointer', hex(p)); continue
    o = foff(p)
    data = b[o:o+512]
    if len(data) < 512:
        print(i, 'short'); continue
    img = Image.new('RGB', (16*16, 16*16), (0, 0, 0))
    px = img.load()
    nz = 0
    for ty in range(16):
        for tx in range(16):
            e = struct.unpack_from('<H', data, (ty*16+tx)*2)[0]
            tile = e & 0x7ff
            bank = (e >> 0xb) + 1
            base = tile*256
            for y in range(16):
                for x in range(16):
                    v = tb[base + y*16 + x]
                    if v == 0:
                        continue
                    nz += 1
                    px[tx*16+x, ty*16+y] = PAL[((bank << 4) | (v & 0xF)) & 0xFF]
    path = os.path.join(outdir, f'panel_{i:02d}.png')
    img.save(path)
    print(i, hex(p), 'nonzero', nz, '->', path)
