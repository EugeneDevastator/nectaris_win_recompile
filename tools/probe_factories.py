"""Inspect the live original-game unit/factory state for the loaded mission."""
import os, sys, struct, subprocess
sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm


def find_pid():
    out = subprocess.check_output(['tasklist', '/FI', 'IMAGENAME eq Nec.exe', '/FO', 'CSV', '/NH'], text=True)
    for line in out.splitlines():
        p = [x.strip('"') for x in line.split('","')]
        if p and p[0].lower() == 'nec.exe':
            return int(p[1])
    return None


pid = find_pid()
print('pid', pid)
h = dm.k32.OpenProcess(0x1F0FFF, False, pid)
base = dm.module_base(h)


def rd(va, n):
    return dm.read(h, base, va, n)


def u32(va):
    return struct.unpack('<I', rd(va, 4))[0]


def u8(va):
    return rd(va, 1)[0]


T = rd(0x4957a0, 0x38)
HR = rd(0x4957e0, 0x38)
X = rd(0x495ab0, 0x38)
Y = rd(0x495af0, 0x38)
print('map dims', list(rd(0x495c02, 2)))
print('unit slots:')
for i in range(0x38):
    if T[i] not in (0, 0xff) or HR[i] != 0:
        print(' %2d type=%2d owner=%d hp=%d rank=%d x=%2d y=%2d xraw=%02x yraw=%02x' %
              (i, T[i] & 0x1f, 1 if T[i] & 0x80 else 0, HR[i] & 7, HR[i] >> 5,
               X[i] & 0x1f, Y[i] & 0x1f, X[i], Y[i]))

# dump regions around the unit arrays and look for repeated factory coords
for name, va, n in [
    ('u_typeowner', 0x4957a0, 0x80),
    ('u_hp_rank', 0x4957e0, 0x80),
    ('u_state_1fa0', 0x481fa0, 0x80),
    ('u_state_1ff0', 0x481ff0, 0x80),
    ('u_x', 0x495ab0, 0x80),
    ('u_y', 0x495af0, 0x80),
    ('factory_a', 0x495bd0, 0x80),
    ('factory_b', 0x495c10, 0x80),
    ('mapattrs', 0x495c00, 0x100),
]:
    data = rd(va, n)
    print('%s @%#x:' % (name, va))
    for off in range(0, len(data), 16):
        chunk = data[off:off+16]
        print('  %#06x  %s' % (va + off, ' '.join('%02x' % b for b in chunk)))

dm.k32.CloseHandle(h)
