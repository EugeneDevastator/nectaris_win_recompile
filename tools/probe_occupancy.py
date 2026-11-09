"""Inspect mission unit positions, occupancy and terrain arrays."""
import sys, os, struct, subprocess
sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm

pid = None
for line in subprocess.check_output(['tasklist', '/FI', 'IMAGENAME eq Nec.exe', '/FO', 'CSV', '/NH'],
                                    text=True).splitlines():
    p = [x.strip('"') for x in line.split('","')]
    if p and p[0].lower() == 'nec.exe':
        pid = int(p[1])
if pid is None:
    raise SystemExit('no game')
h = dm.k32.OpenProcess(0x1F0FFF, False, pid)
base = dm.module_base(h)
occ = dm.read(h, base, 0x495820, 0x580)
ter = dm.read(h, base, 0x481a80, 0x580)
print('pid', pid, 'occ nonzero', sum(1 for b in occ if b), 'unique', sorted(set(occ))[:24])
print('terrain unique', sorted(set(ter))[:24])
to = dm.read(h, base, 0x4957a0, 0x38)
xs = dm.read(h, base, 0x495ab0, 0x38)
ys = dm.read(h, base, 0x495af0, 0x38)
for i in range(0x38):
    if to[i] or xs[i] or ys[i]:
        x = xs[i] & 31
        y = ys[i] & 31
        idx = (y * 32 + x) % len(occ)
        print('slot %2d type %2d own %d x %2d y %2d occ[%d]=%d' %
              (i, to[i] & 31, 1 if to[i] & 0x80 else 0, x, y, idx, occ[idx]))
dm.k32.CloseHandle(h)
