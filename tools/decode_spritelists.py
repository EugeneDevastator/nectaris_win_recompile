"""Decode the game's sprite-list banks: descriptor -> pieces -> composite image."""
import os, struct, sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
EXE = os.path.join(ROOT, 'game', 'Nec.exe')
data = open(EXE, 'rb').read()

pe = struct.unpack_from('<I', data, 0x3c)[0]
num_sections = struct.unpack_from('<H', data, pe + 6)[0]
opt_size = struct.unpack_from('<H', data, pe + 20)[0]
sect = pe + 24 + opt_size
image_base = struct.unpack_from('<I', data, pe + 24 + 28)[0]
sections = []
for i in range(num_sections):
    o = sect + i * 40
    vsize, va, rawsize, rawptr = struct.unpack_from('<IIII', data, o + 8)
    sections.append((va, vsize, rawptr, rawsize))

def va_to_off(va):
    rva = va - image_base
    for sva, vsize, rawptr, rawsize in sections:
        if sva <= rva < sva + max(vsize, rawsize):
            return rawptr + (rva - sva)
    raise ValueError('va not mapped: %#x' % va)

def read(va, n):
    o = va_to_off(va)
    return data[o:o + n]

def u32(va):
    return struct.unpack('<I', read(va, 4))[0]

TABLE = 0x47b898
for bank in range(12):
    ptr = u32(TABLE + bank * 4)
    if not (0x400000 <= ptr < 0x400000 + len(data)):
        print('bank', bank, 'ptr', hex(ptr), '(none)')
        continue
    print('bank', bank, 'table VA', hex(ptr))
    for sid in (1, 2, 3, 4, 12, 20, 28, 38, 48, 58, 68, 78, 86, 96):
        dptr = u32(ptr + (sid - 1) * 4)
        if not (0x400000 <= dptr < 0x400000 + len(data)):
            continue
        desc = read(dptr, 1 + 4 * 12)
        cnt = desc[0]
        pieces = []
        for i in range(min(cnt, 12)):
            dx = struct.unpack('b', desc[1 + i * 4:2 + i * 4])[0]
            dy = struct.unpack('b', desc[2 + i * 4:3 + i * 4])[0]
            piece = desc[3 + i * 4]
            flags = desc[4 + i * 4]
            pieces.append((dx, dy, piece, flags))
        print('  sid', sid, 'cnt', cnt, pieces)
