"""Dump the live 4bpp art atlas and expanded tile/sprite banks."""
import os, sys, struct, subprocess, time
sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
OUT = os.path.join(ROOT, 'dump', 'live_art')
os.makedirs(OUT, exist_ok=True)


def find_pid():
    for line in subprocess.check_output(['tasklist', '/FI', 'IMAGENAME eq Nec.exe', '/FO', 'CSV', '/NH'], text=True).splitlines():
        p = [x.strip('"') for x in line.split('","')]
        if p and p[0].lower() == 'nec.exe':
            return int(p[1])
    return None


pid = find_pid()
if pid is None:
    raise SystemExit('Nec.exe is not running; use orig_play_mission.py first')
h = dm.k32.OpenProcess(0x1F0FFF, False, pid)
base = dm.module_base(h)
print('pid', pid, 'base', hex(base))


def u32(va):
    return struct.unpack('<I', dm.read(h, base, va, 4))[0]


def dump(name, va, n):
    data = dm.read(h, base, va, n)
    open(os.path.join(OUT, name), 'wb').write(data)
    return data


atlas = dump('atlas_full.bin', 0x484730, 0x20000)
tb = dump('tilebank.bin', u32(0x481500), 0x80000) if u32(0x481500) else b''
sb = dump('spritebank.bin', u32(0x480f14), 0x100000) if u32(0x480f14) else b''
dump('palette.bin', 0x480f20, 0x404)
print('atlas 0x20000: nonzero bytes', sum(1 for b in atlas if b),
      'last nonzero at', max((i for i, b in enumerate(atlas) if b), default=-1))
print('tilebank', len(tb), 'spritebank', len(sb))
dm.k32.CloseHandle(h)
