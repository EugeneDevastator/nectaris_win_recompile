"""Decode the game's LZSS resource directory and dump every resource.

Resource directory: VA 0x4324d0, 4 bytes/entry:
  byte0 = uncompressed size hint (or count), bytes1..3 = 24-bit offset
  data base: VA 0x444348
Decompressor: FUN_00418bb0/FUN_00418bf0 (256-byte LZSS ring, 1-bit literal flag).
"""
import os, struct, sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
EXE = os.path.join(ROOT, 'game', 'Nec.exe')
OUT = os.path.join(ROOT, 'dump', 'resources_lzss')
os.makedirs(OUT, exist_ok=True)
data = open(EXE, 'rb').read()

# ---- parse PE sections ----
pe = struct.unpack_from('<I', data, 0x3c)[0]
assert data[pe:pe + 4] == b'PE\0\0'
num_sections = struct.unpack_from('<H', data, pe + 6)[0]
opt_size = struct.unpack_from('<H', data, pe + 20)[0]
sect = pe + 24 + opt_size
image_base = struct.unpack_from('<I', data, pe + 24 + 28)[0]
sections = []
for i in range(num_sections):
    o = sect + i * 40
    name = data[o:o + 8].rstrip(b'\0').decode('latin1')
    vsize, va, rawsize, rawptr = struct.unpack_from('<IIII', data, o + 8)
    sections.append((name, va, vsize, rawptr, rawsize))
print('image_base', hex(image_base))
for s in sections:
    print('  %-8s va=%#x vsize=%#x raw=%#x size=%#x' % s)

def va_to_off(va):
    rva = va - image_base
    for name, sva, vsize, rawptr, rawsize in sections:
        if sva <= rva < sva + max(vsize, rawsize):
            return rawptr + (rva - sva)
    raise ValueError('va not mapped: %#x' % va)

DIR_VA = 0x4324d0
BASE_VA = 0x444348

def lzss(src, max_out=0x20000):
    ring = bytearray(b' ' * 256)
    out = []
    pos = 0xef  # output ring position
    sp = 1
    bitbyte = src[0]
    bits = 8
    match_src = 0
    match_left = 0

    def read_bits(n):
        nonlocal sp, bitbyte, bits
        c = 0
        for _ in range(n):
            c = (c << 1) & 0xff
            if bitbyte & 0x80:
                c |= 1
            bitbyte = (bitbyte << 1) & 0xff
            bits -= 1
            if bits == 0:
                if sp >= len(src):
                    return None
                bitbyte = src[sp]; sp += 1
                bits = 8
        return c

    while len(out) < max_out:
        if match_left == 0:
            flag = read_bits(1)
            if flag is None:
                break
            if flag:
                c = read_bits(8)
                if c is None:
                    break
                ring[pos & 0xff] = c
                pos = (pos + 1) & 0xff
                out.append(c)
                continue
            match_src = read_bits(8)
            n = read_bits(4)
            if match_src is None or n is None:
                break
            match_left = n + 2
        c = ring[match_src & 0xff]
        match_src = (match_src + 1) & 0xff
        match_left -= 1
        ring[pos & 0xff] = c
        pos = (pos + 1) & 0xff
        out.append(c)
    return bytes(out)

# ---- dump directory ----
entries = []
for i in range(0x400):
    off = va_to_off(DIR_VA + i * 4)
    b0, b1, b2, b3 = data[off:off + 4]
    ptr_va = BASE_VA + ((b1 << 16) | (b2 << 8) | b3)
    entries.append((i, b0, ptr_va))

# only keep plausible entries
plausible = []
for i, size_hint, ptr_va in entries:
    try:
        off = va_to_off(ptr_va)
    except ValueError:
        continue
    if 0 <= off < len(data):
        plausible.append((i, size_hint, ptr_va, off))
print('plausible entries:', len(plausible))

summary = []
for i, size_hint, ptr_va, off in plausible:
    src = data[off:off + min(0x40000, len(data) - off)]
    try:
        dec = lzss(src, max_out=size_hint * 0x400 if size_hint else 0x10000)
    except Exception as e:
        dec = b''
    if not dec:
        continue
    p = os.path.join(OUT, 'res_%03d.bin' % i)
    open(p, 'wb').write(dec)
    summary.append((i, size_hint, ptr_va, len(dec), dec[:16].hex()))

print('decoded', len(summary))
for i, hint, va, n, head in summary[:80]:
    print('  %3d hint=%3d va=%#08x len=%6d %s' % (i, hint, va, n, head))
