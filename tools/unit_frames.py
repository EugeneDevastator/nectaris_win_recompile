"""Decode all 23 unit art streams offline and export every frame.

Uses the verified LZSS and the decoded script format:
  directory index = 26 + unit_type
  script = [block_count] then [cmd_count][cmds]
  each block = 512 bytes = four 16x16 4bpp chunks (two 8x16 halves each)
  cmd: slot = 64 + (cmd&0x1f)*4, count = (cmd>>5)+1
"""
import os, sys, struct, json
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
OUT = os.path.join(ROOT, 'assets', 'art', 'unit_frames')
os.makedirs(OUT, exist_ok=True)
exe = open(os.path.join(ROOT, 'game', 'Nec.exe'), 'rb').read()
ib = 0x400000
secs = [(0x1000, 0x27400, 0x400, 0x27400), (0x2a000, 0x6c064, 0x27c00, 0x56200)]
pal = open(os.path.join(ROOT, 'dump', 'battle_sprites', 'palette.bin'), 'rb').read()
PAL = [(pal[4 + i * 4], pal[5 + i * 4], pal[6 + i * 4]) for i in range(256)]
units = json.load(open(os.path.join(ROOT, 'assets', 'data', 'units.json'), encoding='utf-8'))['units']
names = {u['id']: u['name'] for u in units}


def off(va):
    r = va - ib
    for sva, vs, rp, rs in secs:
        if sva <= r < sva + max(vs, rs):
            return rp + (r - sva)
    raise ValueError(hex(va))


def comp_ptr(directory_index):
    o = (exe[off(0x4324d1 + directory_index * 4)] << 16) | \
        (exe[off(0x4324d2 + directory_index * 4)] << 8) | \
        exe[off(0x4324d3 + directory_index * 4)]
    return off(0x444348) + o


def lz(src, n):
    win = bytearray([0x20] * 256)
    wp, rp, bits, pos, remain = 0xEF, 0, 8, 1, 0
    cur = src[0]

    def rb(k):
        nonlocal bits, cur, pos
        v = 0
        for _ in range(k):
            v = (v << 1) | ((cur >> 7) & 1)
            cur = (cur << 1) & 0xFF
            bits -= 1
            if bits == 0:
                cur = src[pos] if pos < len(src) else 0
                pos += 1
                bits = 8
        return v

    out = bytearray()
    while len(out) < n:
        if remain == 0:
            if rb(1):
                c = rb(8)
                win[wp] = c; wp = (wp + 1) & 0xFF
                out.append(c)
                continue
            rp = rb(8)
            remain = rb(4) + 2
        c = win[rp]; rp = (rp + 1) & 0xFF; remain -= 1
        win[wp] = c; wp = (wp + 1) & 0xFF
        out.append(c)
    return bytes(out)


def unpack(chunk, bank=9):
    im = Image.new('RGBA', (32, 32), (0, 0, 0, 0))
    px = im.load()
    for y in range(16):
        for x in range(16):
            if x < 8:
                b = chunk[y * 4 + (x >> 1)]
                v = (b >> 4) if (x & 1) == 0 else (b & 0xF)
            else:
                xx = x - 8
                b = chunk[0x40 + y * 4 + (xx >> 1)]
                v = (b >> 4) if (xx & 1) == 0 else (b & 0xF)
            if v:
                c = PAL[((bank << 4) | v) & 255] + (255,)
                px[x * 2, 31 - y * 2] = c
                px[x * 2 + 1, 31 - y * 2] = c
                px[x * 2, 30 - y * 2] = c
                px[x * 2 + 1, 30 - y * 2] = c
    return im


ptrs = [struct.unpack_from('<I', exe, off(0x42fee8) + t * 4)[0] for t in range(23)]
uniq = sorted(set(ptrs))
all_frames = []
for t in range(23):
    p = ptrs[t]
    i = uniq.index(p)
    end = uniq[i + 1] if i + 1 < len(uniq) else 0x42fee8
    script = exe[off(p):off(end)]
    try:
        res = lz(exe[comp_ptr(26 + t):], 0x40000)
    except Exception as exc:
        print('type', t, 'lz fail', exc)
        continue
    slots = {}
    pos, rpos = 1, 0
    nblocks = script[0] if script else 0
    made = 0
    for b in range(nblocks):
        if pos >= len(script):
            break
        ncmd = script[pos]; pos += 1
        data = res[rpos:rpos + 512]; rpos += 512
        if len(data) < 512:
            break
        block_frame = None
        for c in range(ncmd):
            if pos >= len(script):
                break
            cmd = script[pos]; pos += 1
            cnt = (cmd >> 5) + 1
            if cnt != 4:
                continue  # scaled variant, not the full unit
            cimg = Image.new('RGBA', (64, 64), (0, 0, 0, 0))
            for j in range(4):
                cimg.alpha_composite(unpack(data[j * 128:(j + 1) * 128]), ((j % 2) * 32, (j // 2) * 32))
            # units are stored cut in half horizontally; rotate each half 180
            top = cimg.crop((0, 0, cimg.width, cimg.height // 2)).rotate(180)
            bot = cimg.crop((0, cimg.height // 2, cimg.width, cimg.height)).rotate(180)
            fixed = Image.new('RGBA', cimg.size, (0, 0, 0, 0))
            fixed.alpha_composite(top, (0, 0))
            fixed.alpha_composite(bot, (0, cimg.height // 2))
            block_frame = fixed
        if block_frame is None:
            continue
        bb = block_frame.getbbox()
        if not bb:
            continue
        cimg = block_frame.crop(bb)
        fn = 'unit_%02d_frame_%02d.png' % (t, made)
        cimg.save(os.path.join(OUT, fn))
        all_frames.append((t, fn, cimg.copy()))
        made += 1
    print('type %2d %-8s blocks %d frames %d' % (t, names.get(t, '?'), nblocks, made), flush=True)

# atlas of all frames
if all_frames:
    cols = 10
    cell = 90
    rows = (len(all_frames) + cols - 1) // cols
    sheet = Image.new('RGB', (cols * cell, rows * cell + 16), (70, 70, 70))
    d = ImageDraw.Draw(sheet)
    for i, (t, fn, im) in enumerate(all_frames):
        big = im.resize((max(1, im.width * 2), max(1, im.height * 2)), Image.NEAREST)
        x = (i % cols) * cell + 5
        y = (i // cols) * cell + 18
        sheet.paste(big, (x, y), big)
        d.text((x, y - 13), '%d %s' % (t, names.get(t, '?')[:6]), fill=(10, 10, 10))
    sheet.save(os.path.join(OUT, 'unit_frames_atlas.png'))
    print('total frames', len(all_frames), 'atlas', os.path.join(OUT, 'unit_frames_atlas.png'))
