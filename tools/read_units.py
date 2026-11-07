import os, sys, struct, subprocess
sys.path.insert(0, os.path.join(os.path.dirname(__file__)))
import dump_mem as dm
out = subprocess.check_output(['tasklist', '/FI', 'IMAGENAME eq Nec.exe', '/FO', 'CSV', '/NH'], text=True)
pid = None
for line in out.splitlines():
    p = [x.strip('"') for x in line.split('","')]
    if p and p[0].lower() == 'nec.exe':
        pid = int(p[1])
print('pid', pid)
h = dm.k32.OpenProcess(0x1F0FFF, False, pid)
base = dm.module_base(h)
t = dm.read(h, base, 0x4957a0, 0x38)
st = dm.read(h, base, 0x495ab0, 0x38)
fl = dm.read(h, base, 0x495af0, 0x38)
for i in range(0x38):
    if t[i] != 0xff:
        print(i, 'owner', t[i] & 0x80, 'type', t[i] & 0x1f,
              'cell', (st[i] & 0x1f, fl[i] & 0x1f))
dm.k32.CloseHandle(h)
