"""Export a terrain's combat backdrop in every palette bank (0..15) so the
correct one can be picked visually.
"""
import os, struct, json
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
b = open(os.path.join(ROOT, 'game', 'Nec.exe'), 'rb').read()
BASE_VA, BASE_OFF = 0x42a000, 0x27c00
def foff(va): return BASE_OFF + (va - BASE_VA)
pal_battle = os.path.join(ROOT, 'dump', 'palette_battle.bin')
if os.path.exists(pal_battle):
    raw = open(pal_battle, 'rb').read()
    PAL = [(raw[4 + i * 4], raw[5 + i * 4], raw[6 + i * 4]) for i in range(256)]
else:
    bgr = open(os.path.join(ROOT, 'dump', 'palette_bgr.bin'), 'rb').read()
    PAL = [(bgr[i*4+2], bgr[i*4+1], bgr[i*4]) for i in range(256)]
tb = open(os.path.join(ROOT, 'dump', 'tilebank_battle.bin'), 'rb').read()
CT = json.load(open(os.path.join(ROOT, 'assets', 'data', 'combat_terrain.json'), encoding='utf-8'))
remap = CT['terrain_remap']
table = CT['panel_table']

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

def render(panel, bank):
    raw = lz(b, foff(ptrs[panel]), 512)
    img = Image.new('RGB', (256, 256), (0, 0, 0))
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
                        px[tx*16+x, ty*16+(15-y)] = PAL[((bank << 4) | (v & 0xF)) & 0xFF]
    return img

def do_terrain(t):
    r = remap[t]
    left = table[r*16 + r + 8] % 24
    right = table[r*16 + r] % 24
    folder = None
    for name in os.listdir(os.path.join(ROOT, 'assets', 'terrain')):
        if name.startswith('%02d_' % t):
            folder = os.path.join(ROOT, 'assets', 'terrain', name)
    if not folder:
        print('no folder for', t); return
    pal_dir = os.path.join(folder, 'palettes')
    os.makedirs(pal_dir, exist_ok=True)
    sheet = Image.new('RGB', (8 * 132, 2 * 148), (10, 10, 18))
    d = ImageDraw.Draw(sheet)
    for bank in range(16):
        im = render(left, bank)
        im.save(os.path.join(pal_dir, 'bank_%02d.png' % bank))
        thumb = im.resize((128, 128), Image.NEAREST)
        x = (bank % 8) * 132; y = (bank // 8) * 148
        sheet.paste(thumb, (x + 2, y + 18))
        d.text((x + 2, y), 'bank %d' % bank, fill=(255, 255, 0))
    sheet.save(os.path.join(pal_dir, 'all.png'))
    print('terrain %d (%s) panel %d/%d -> %s' % (t, os.path.basename(folder), left, right, pal_dir))

for t in range(16):
    do_terrain(t)

