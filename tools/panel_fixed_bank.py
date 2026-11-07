import os, struct
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
b = open(os.path.join(ROOT, 'game', 'Nec.exe'), 'rb').read()
BASE_VA, BASE_OFF = 0x42a000, 0x27c00
def foff(va): return BASE_OFF + (va - BASE_VA)
bgr = open(os.path.join(ROOT, 'dump', 'palette_bgr.bin'), 'rb').read()
PAL = [(bgr[i*4+2], bgr[i*4+1], bgr[i*4]) for i in range(256)]
tb = open(os.path.join(ROOT, 'dump', 'tilebank_battle.bin'), 'rb').read()

class Bits:
    def __init__(s, d, p): s.d=d; s.p=p; s.cur=d[p]; s.p+=1; s.left=8
    def get(s, n):
        v=0
        for _ in range(n):
            v=(v<<1)|((s.cur>>7)&1); s.cur=(s.cur<<1)&0xFF; s.left-=1
            if s.left==0:
                s.cur=s.d[s.p] if s.p<len(s.d) else 0; s.p+=1; s.left=8
        return v
def lz(d, st, n):
    br=Bits(d,st); ring=bytearray([0x20]*256); wr=0xEF; out=[]
    while len(out)<n:
        if br.get(1):
            c=br.get(8); out.append(c); ring[wr]=c; wr=(wr+1)&0xFF
        else:
            off=br.get(8); ln=br.get(4)+2; src=off
            for _ in range(ln):
                c=ring[src&0xFF]; src+=1; out.append(c); ring[wr]=c; wr=(wr+1)&0xFF
                if len(out)>=n: break
    return bytes(out[:n])
ptrs = [struct.unpack_from('<I', b, foff(0x431148) + i*4)[0] for i in range(24)]

def render_fixed(i, bank):
    raw = lz(b, foff(ptrs[i]), 512)
    img = Image.new('RGB', (64, 64), (0, 0, 0))
    px = img.load()
    for ty in range(16):
        for tx in range(16):
            e = struct.unpack_from('<H', raw, (ty*16+tx)*2)[0]
            tile = e & 0x7ff
            base = tile*256
            for y in range(16):
                for x in range(16):
                    v = tb[base + y*16 + x]
                    if v:
                        px[tx*4 + x//4, ty*4 + (15-y)//4] = PAL[((bank << 4) | (v & 0xF)) & 0xFF]
    return img

for panel, name in ((1, 'panel1'), (16, 'panel16')):
    out = Image.new('RGB', (8 * 140, 2 * 150), (10, 10, 18))
    d = ImageDraw.Draw(out)
    for bank in range(16):
        im = render_fixed(panel, bank).resize((132, 132), Image.NEAREST)
        x = (bank % 8) * 140; y = (bank // 8) * 150
        out.paste(im, (x + 4, y + 16))
        d.text((x + 4, y), '%s bank %d' % (name, bank), fill=(255, 255, 0))
    out.save(os.path.join(ROOT, 'dump', name + '_banks.png'))
    print('wrote', name + '_banks.png')
