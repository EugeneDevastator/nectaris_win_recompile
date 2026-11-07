import os
from PIL import Image

D = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'dump'))

def load_palette():
    p = os.path.join(D, 'palette_bgr.bin')
    if os.path.exists(p):
        raw = open(p, 'rb').read()
        assert len(raw) >= 1024
        return [(raw[i*4+2], raw[i*4+1], raw[i*4]) for i in range(256)]
    raw = open(os.path.join(D, 'palette.bin'), 'rb').read()
    assert len(raw) >= 4 + 1024
    return [(raw[4+i*4], raw[4+i*4+1], raw[4+i*4+2]) for i in range(256)]

def render_screen():
    p = os.path.join(D, 'screen8.bin')
    if not os.path.exists(p):
        return
    data = open(p, 'rb').read()
    img = Image.new('RGB', (640, 480))
    px = img.load()
    for y in range(480):
        base = (543 - (y + 32)) * 704 + 32
        for x in range(640):
            img.putpixel((x, y), PAL[data[base+x]])
    img.save(os.path.join(D, 'screen.png'))
    print('screen ->', img.size)

def render_8bit(path, sheet, tile_w, tile_h, cols, rows, scale=1, transparent_zero=True, bank=0):
    data = open(path, 'rb').read()
    pal = PAL
    W, H = cols*tile_w, rows*tile_h
    img = Image.new('RGB', (W*scale, H*scale), (16, 16, 24))
    px = img.load()
    tile_bytes = tile_w*tile_h
    n = len(data)//tile_bytes
    for t in range(min(n, cols*rows)):
        tx, ty = (t % cols)*tile_w, (t//cols)*tile_h
        base = t*tile_bytes
        for y in range(tile_h):
            for x in range(tile_w):
                v = data[base + y*tile_w + x]
                if transparent_zero and v == 0:
                    continue
                r, g, b = pal[(bank << 4) | (v & 0xF)]
                for sy in range(scale):
                    for sx in range(scale):
                        px[(tx+x)*scale+sx, (ty+y)*scale+sy] = (r, g, b)
    name = os.path.basename(path).replace('.bin', f'_b{bank}.png')
    img.save(os.path.join(D, name))
    print(f"{path} bank={bank}: {n} tiles -> {img.size} ({name})")

def render_atlas():
    data = open(os.path.join(D, 'atlas.bin'), 'rb').read()
    cols = 48; rows = 48; tw = 8
    img = Image.new('RGB', (cols*tw*2, rows*tw*2), (16, 16, 24))
    px = img.load()
    n = len(data)//32
    for t in range(min(n, cols*rows)):
        tx, ty = (t % cols)*tw, (t//cols)*tw
        for y in range(8):
            row = data[t*32 + y*4:t*32 + y*4 + 4]
            for x in range(8):
                v = (row[x//2] >> (4 if x % 2 == 0 else 0)) & 0xF
                if v == 0:
                    continue
                r, g, b = PAL[v]
                for sy in range(2):
                    for sx in range(2):
                        px[(tx+x)*2+sx, (ty+y)*2+sy] = (r, g, b)
    img.save(os.path.join(D, 'atlas.png'))
    print("atlas ->", img.size)

PAL = load_palette()
print("palette[1..16]:", PAL[1:17])
render_8bit(os.path.join(D, 'tilebank.bin'), None, 16, 16, 64, 32, 1, bank=0)
render_8bit(os.path.join(D, 'spritebank.bin'), None, 32, 32, 32, 32, 1, bank=13)
render_8bit(os.path.join(D, 'spritebank.bin'), None, 32, 32, 16, 16, 2, bank=13)
render_atlas()
render_screen()
