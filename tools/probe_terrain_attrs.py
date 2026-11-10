"""Probe the original game's per-cell terrain attributes and move-cost rules."""
import ctypes
import os
import struct
import subprocess
import sys
import time

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
sys.path.insert(0, os.path.join(ROOT, 'tools'))
import dump_mem as dm


def main():
    subprocess.run(['taskkill', '/F', '/IM', 'Nec.exe'],
                   stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    time.sleep(0.3)
    subprocess.call([sys.executable, os.path.join(ROOT, 'tools', 'orig_play_mission.py'),
                     '1', '--no-preview'], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    time.sleep(4)
    pid = None
    for line in subprocess.check_output(
            ['tasklist', '/FI', 'IMAGENAME eq Nec.exe', '/FO', 'CSV', '/NH'],
            text=True).splitlines():
        p = [x.strip('"') for x in line.split('","')]
        if p and p[0].lower() == 'nec.exe':
            pid = int(p[1])
    h = dm.k32.OpenProcess(0x1F0FFF, False, pid)
    base = dm.module_base(h)

    def u8(va):
        return dm.read(h, base, va, 1)[0]

    def u32(va):
        return struct.unpack('<I', dm.read(h, base, va, 4))[0]

    print('state', hex(u32(0x483270)))
    w, hgt = u8(0x42a9a0), u8(0x42a9b0)
    print('size', w, hgt)
    print('terrain attributes (0x481a80 & 0x0f):')
    for y in range(hgt):
        print(' '.join('%2d' % (u8(0x481a80 + y * 32 + x) & 0x0f) for x in range(w)))
    print('move table [terrain][class 0..3]:')
    for t in range(16):
        print(t, [u8(0x42b248 + t * 4 + c) for c in range(4)])
    print('unit movement classes:', [u8(0x42b230 + i) for i in range(24)])
    print('units:')
    for s in range(32):
        typ = u8(0x4957a0 + s)
        if typ == 0xff:
            continue
        x, y = u8(0x495ab0 + s), u8(0x495af0 + s)
        print(' slot', s, 'type', typ & 0x1f, 'owner', typ >> 7, 'at', x & 0x1f, y & 0x1f,
              'terrain', u8(0x481a80 + (y & 0x1f) * 32 + (x & 0x1f)) & 0x0f)
    dm.k32.CloseHandle(h)


if __name__ == '__main__':
    sys.exit(main())
