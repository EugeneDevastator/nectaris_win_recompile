import struct
from PIL import Image

D = r'dump'
raw = open(D + r'\palette_bgr.bin', 'rb').read()
PAL = [(raw[i*4+2], raw[i*4+1], raw[i*4]) for i in range(256)]
tb = open(D + r'\tilebank.bin', 'rb').read()
tm = open(D + r'\tilemap.bin', 'rb').read()

# Exact simulation of FUN_00411b30 loop: 64 entries x 8px, 32 rows x 8px,
# each entry draws a 16x16 tile (overwriting).
W, H = 64*8 + 8, 32*8 + 8
img = Image.new('RGB', (W, H), (0, 0, 0))
px = img.load()
for row in range(32):
    for col in range(64):
        e = struct.unpack_from('<H', tm, (row*64+col)*2)[0]
        tile = e & 0x7ff
        bank = (e >> 0xb) + 1
        base = tile*256
        x0, y0 = col*8, row*8
        for y in range(16):
            for x in range(16):
                v = tb[base + y*16 + x]
                if v == 0: continue
                r, g, b = PAL[((bank << 4) | (v & 0xF)) & 0xFF]
                px[x0+x, y0+y] = (r, g, b)
img.save(D + r'\bg_overlap.png')
print(img.size)
