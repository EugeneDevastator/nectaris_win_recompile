"""Dump the game's composite battle sprite lists (DAT_004846f4)."""
import ctypes, ctypes.wintypes as wt, os, sys, struct, subprocess
sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
out = subprocess.check_output(['tasklist', '/FI', 'IMAGENAME eq Nec.exe', '/FO', 'CSV', '/NH'], text=True)
pid = None
for line in out.splitlines():
    p = [x.strip('"') for x in line.split('","')]
    if p and p[0].lower() == 'nec.exe':
        pid = int(p[1])
if not pid:
    print('Nec.exe not running')
    sys.exit(1)
h = dm.k32.OpenProcess(0x1F0FFF, False, pid)
base = dm.module_base(h)
def u32(va): return struct.unpack('<I', dm.read(h, base, va, 4))[0]
def u8(va): return dm.read(h, base, va, 1)[0]

print('base', hex(base))
for va in (0x4846f4, 0x4846f0, 0x4846f8):
    try:
        print(hex(va), '=', hex(u32(va)))
    except Exception as e:
        print(hex(va), 'err', e)

tbl = u32(0x4846f4)
print('table base', hex(tbl))
if not (0x400000 <= tbl < 0x800000):
    # maybe it is the table itself
    tbl = 0x4846f4
    print('using literal table at', hex(tbl))

for gid in range(1, 90):
    addr = tbl - 4 + gid * 4
    try:
        p = u32(addr)
    except Exception:
        break
    if not (0x400000 <= p < 0x800000):
        continue
    try:
        raw = dm.read_abs(h, p, 1 + 4 * 40)
    except Exception as e:
        print('gid', gid, 'ptr', hex(p), 'unreadable', e)
        continue
    cnt = raw[0]
    parts = []
    for i in range(min(cnt, 40)):
        dx, dy, spr, fl = raw[1+i*4], raw[2+i*4], raw[3+i*4], raw[4+i*4]
        parts.append((dx, dy, spr, fl))
    print('gid %d ptr=%#x count=%d parts=%s' % (gid, p, cnt, parts))
dm.k32.CloseHandle(h)
