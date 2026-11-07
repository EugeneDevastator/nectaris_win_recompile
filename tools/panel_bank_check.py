import os, struct, json
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
def panel_raw(i):
    return lz(b, foff(ptrs[i]), 512)

def render(i, bank_add):
    raw = panel_raw(i)
    img = Image.new('RGB', (256, 256), (0, 0, 0))
    px = img.load()
    banks = set()
    for ty in range(16):
        for tx in range(16):
            e = struct.unpack_from('<H', raw, (ty*16+tx)*2)[0]
            tile = e & 0x7ff
            bank = ((e >> 11) + bank_add) & 15
            banks.add(bank)
            base = tile*256
            for y in range(16):
                for x in range(16):
                    v = tb[base + y*16 + x]
                    if v:
                        idx = ((bank << 4) | (v & 0xF)) & 0xFF
                        px[tx*16+x, ty*16+(15-y)] = PAL[idx]
    return img, sorted(banks)

# bank usage per panel
for i in range(24):
    _, banks = render(i, 1)
    print('panel %02d banks %s' % (i, banks))

# water candidates
out = Image.new('RGB', (4*260, 2*280), (10, 10, 18))
d = ImageDraw.Draw(out)
for j, (panel, add) in enumerate([(16, 0), (16, 1), (16, 2), (16, 3)]):
    im, _ = render(panel, add)
    out.paste(im, (j*260+2, 20))
    d.text((j*260+2, 2), 'panel %d bank+%d' % (panel, add), fill=(255,255,0))
for j, (panel, add) in enumerate([(1, 0), (1, 1), (1, 2), (1, 3)]):
    im, _ = render(panel, add)
    out.paste(im, (j*260+2, 300))
    d.text((j*260+2, 282), 'panel %d bank+%d' % (panel, add), fill=(255,255,0))
out.save(os.path.join(ROOT, 'dump', 'panel_banks.png'))
print('wrote dump/panel_banks.png')
