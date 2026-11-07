"""Robust dump of the composite sprite-group table at DAT_004846f4."""
import ctypes, ctypes.wintypes as wt, os, sys, struct, subprocess, json
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
    print('not running'); sys.exit(1)
h = dm.k32.OpenProcess(0x1F0FFF, False, pid)
base = dm.module_base(h)
def u32(va): return struct.unpack('<I', dm.read(h, base, va, 4))[0]
tbl = u32(0x4846f4)
print('table', hex(tbl))
groups = {}
for gid in range(1, 256):
    addr = tbl - 4 + gid * 4
    try:
        p = u32(addr)
    except Exception:
        continue
    if not (0x400000 <= p < 0x800000):
        continue
    try:
        raw = dm.read_abs(h, p, 1 + 4 * 64)
    except Exception:
        continue
    cnt = raw[0]
    if cnt == 0 or cnt > 64:
        continue
    parts = []
    for i in range(cnt):
        dx, dy, spr, fl = raw[1+i*4], raw[2+i*4], raw[3+i*4], raw[4+i*4]
        parts.append((dx, dy, spr, fl))
    groups[gid] = parts
json.dump(groups, open(os.path.join(ROOT, 'dump', 'sprite_groups.json'), 'w'), indent=0)
print('groups', len(groups))
for gid, parts in groups.items():
    if len(parts) > 1 or max(p[2] for p in parts) >= 96:
        print('gid %d count=%d sprites=%s' % (gid, len(parts),
              [p[2] for p in parts]))
dm.k32.CloseHandle(h)
