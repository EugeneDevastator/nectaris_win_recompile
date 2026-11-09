"""Simulate the unit chunk script interpreter in Python.

Script = [block_count] then per block: [cmd_count][cmd...]
Each block consumes 512 bytes from the decompressed unit resource = four
16x16 4bpp chunks, expanded 2x to 32x32 8bpp and written at
slot = 64 + (cmd & 0x1f)*4, for (cmd >> 5)+1 chunks.
"""
import os, sys, struct
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
TYP = int(sys.argv[1]) if len(sys.argv) > 1 else 18
RESID = 26 + TYP
OUT = os.path.join(ROOT, 'dump', 'script_sim')
os.makedirs(OUT, exist_ok=True)
pal = open(os.path.join(ROOT, 'dump', 'battle_sprites', 'palette.bin'), 'rb').read()
PAL = [(pal[4 + i * 4], pal[5 + i * 4], pal[6 + i * 4]) for i in range(256)]

# script table from the exe
exe = open(os.path.join(ROOT, 'game', 'Nec.exe'), 'rb').read()
ib = 0x400000
secs = [(0x1000, 0x27400, 0x400, 0x27400), (0x2a000, 0x6c064, 0x27c00, 0x56200)]


def off(va):
    r = va - ib
    for sva, vs, rp, rs in secs:
        if sva <= r < sva + max(vs, rs):
            return rp + (r - sva)
    raise ValueError(hex(va))


ptrs = [struct.unpack_from('<I', exe, off(0x42fee8) + t * 4)[0] for t in range(23)]
# script length = next unique pointer
uniq = sorted(set(ptrs))
end = uniq[uniq.index(ptrs[TYP]) + 1] if uniq.index(ptrs[TYP]) + 1 < len(uniq) else 0x42fee8
script = exe[off(ptrs[TYP]):off(end)]
res_path = os.environ.get('STREAM') or os.path.join(ROOT, 'dump', 'resources_lzss', 'res_%03d.bin' % RESID)
res = open(res_path, 'rb').read()
print('type', TYP, 'script', script.hex(' '), 'len', len(script), 'res', len(res))

# sprite bank image slots 64..191
slots = {}
pos = 1
rpos = 0
nblocks = script[0]
print('nblocks', nblocks)
for b in range(nblocks):
    if pos >= len(script):
        break
    ncmd = script[pos]; pos += 1
    data = res[rpos:rpos + 512]; rpos += 512
    if len(data) < 512:
        print('resource exhausted at block', b)
        break
    print('block', b, 'ncmd', ncmd, 'data off', hex(rpos - 512))
    for c in range(ncmd):
        if pos >= len(script):
            break
        cmd = script[pos]; pos += 1
        idx = cmd & 0x1F
        cnt = (cmd >> 5) + 1
        slot = 64 + idx * 4
        # place chunks: chunk j at slot+j, j < cnt (clamp 4)
        for j in range(min(cnt, 4)):
            chunk = data[j * 128:(j + 1) * 128]
            img = Image.new('RGBA', (32, 32), (0, 0, 0, 0))
            px = img.load()
            for y in range(16):
                for x in range(16):
                    if x < 8:
                        bbyte = chunk[y * 4 + (x >> 1)]
                        v = (bbyte >> 4) if (x & 1) == 0 else (bbyte & 0xF)
                    else:
                        xx = x - 8
                        bbyte = chunk[0x40 + y * 4 + (xx >> 1)]
                        v = (bbyte >> 4) if (xx & 1) == 0 else (bbyte & 0xF)
                    if v:
                        c = PAL[((9 << 4) | v) & 255]
                        px[x * 2, (31 - y * 2)] = c + (255,)
                        px[x * 2 + 1, (31 - y * 2)] = c + (255,)
                        px[x * 2, (31 - y * 2) - 1] = c + (255,)
                        px[x * 2 + 1, (31 - y * 2) - 1] = c + (255,)
            slots[slot + j] = img
        print('  cmd %02x idx %d cnt %d slot %d' % (cmd, idx, cnt, slot))

# render slots 64..191
cols = 16
cell = 70
sheet = Image.new('RGB', (cols * cell, 8 * cell + 16), (70, 70, 70))
d = ImageDraw.Draw(sheet)
for k in range(128):
    idx = 64 + k
    x = (k % cols) * cell + 3
    y = (k // cols) * cell + 16
    if idx in slots:
        im = slots[idx].resize((64, 64), Image.NEAREST)
        sheet.paste(im, (x, y), im)
    d.text((x, y - 12), str(idx), fill=(0, 0, 0))
sheet.save(os.path.join(OUT, 'unit_%02d_sim.png' % TYP))
print('wrote', os.path.join(OUT, 'unit_%02d_sim.png' % TYP))

# compose full 4-chunk groups into 64x64 units
groups = {}
for slot, im in slots.items():
    groups.setdefault(slot // 4, {})[slot % 4] = im
gitem = [g for g, p in sorted(groups.items()) if len(p) == 4]
if gitem:
    gs = Image.new('RGB', (6 * 160, ((len(gitem) + 5) // 6) * 160), (70, 70, 70))
    gd = ImageDraw.Draw(gs)
    for i, g in enumerate(gitem):
        parts = groups[g]
        c = Image.new('RGBA', (64, 64), (0, 0, 0, 0))
        for j, im in parts.items():
            c.alpha_composite(im, ((j % 2) * 32, (j // 2) * 32))
        bb = c.getbbox()
        c = c.crop(bb) if bb else c
        c = c.resize((c.width * 3, c.height * 3), Image.NEAREST)
        x = (i % 6) * 160 + 5
        y = (i // 6) * 160 + 20
        gs.paste(c, (x, y), c)
        gd.text((x, y - 16), 'slot %d' % (g * 4), fill=(0, 0, 0))
    gs.save(os.path.join(OUT, 'unit_%02d_groups.png' % TYP))
    print('wrote groups', os.path.join(OUT, 'unit_%02d_groups.png' % TYP))
