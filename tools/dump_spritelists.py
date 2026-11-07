import os, sys, time, struct, ctypes
sys.path.insert(0, os.path.dirname(__file__))
sys.path.insert(0, os.path.join(os.path.dirname(__file__), 'fakeinput'))
import dump_mem as dm

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
game = os.path.join(ROOT, 'game')
p = __import__('subprocess').Popen([os.path.join(game, 'Nec.exe')], cwd=game)
time.sleep(6)
h = dm.k32.OpenProcess(dm.PROCESS_VM_READ | dm.PROCESS_QUERY_INFORMATION, False, p.pid)
try:
    base = dm.module_base(h)
    print('base', hex(base))
    table = dm.read(h, base, 0x4846f4, 0x400)
    b170 = dm.read(h, base, 0x42b170, 0x40)
    print('DAT_0042b170 =', list(b170))
    print('sprite-list pointers:')
    for i in range(0, 128):
        ptr = struct.unpack_from('<I', table, i*4)[0]
        if ptr == 0:
            continue
        try:
            hdr = dm.read_abs(h, ptr, 64)
        except OSError:
            print(f'  [{i}] ptr={ptr:#x} unreadable')
            continue
        cnt = hdr[0]
        recs = []
        for r in range(min(cnt, 12)):
            dx, dy, spr, fl = hdr[1+r*4], hdr[2+r*4], hdr[3+r*4], hdr[4+r*4]
            recs.append((dx, dy, spr, fl))
        print(f'  [{i}] ptr={ptr:#x} count={cnt} first={recs[:4]}')
finally:
    dm.k32.CloseHandle(h)
    p.terminate()
