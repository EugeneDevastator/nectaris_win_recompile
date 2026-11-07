import os, sys, time, struct, ctypes
sys.path.insert(0, os.path.dirname(__file__))
sys.path.insert(0, os.path.join(os.path.dirname(__file__), 'fakeinput'))
from control import Driver
import dump_mem as dm

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
game = os.path.join(ROOT, 'game')
d = Driver(game)
try:
    d.fake_activate()
    for i in range(22):
        d.tap(0x20)
        d.wake()
        time.sleep(0.35)
    time.sleep(2.0)
    h = dm.k32.OpenProcess(dm.PROCESS_VM_READ | dm.PROCESS_QUERY_INFORMATION, False, d.proc.pid)
    base = dm.module_base(h)
    print('base', hex(base))
    try:
        table = dm.read(h, base, 0x4846f4, 0x500)
        print('sprite-list table around 0xe9:')
        for idx in range(0xe0, 0xef):
            off = (idx - 1) * 4
            ptr = struct.unpack_from('<I', table, off)[0] if off + 4 <= len(table) else 0
            print(f'  idx={idx:#x} ptr={ptr:#x}')
            if 0x400000 <= ptr < 0x500000:
                try:
                    raw = dm.read_abs(h, ptr, 40)
                    cnt = raw[0]
                    recs = [tuple(raw[1+i*4:5+i*4]) for i in range(min(cnt, 9))]
                    print(f'      count={cnt} recs(dx,dy,spr,fl)={recs}')
                except OSError as e:
                    print('      unreadable', e)
        for va, name in ((0x4834d0, 'atk formation'), (0x4835e0, 'def formation'),
                         (0x483290, 'atk copy'), (0x483390, 'def copy')):
            data = dm.read(h, base, va, 0x100)
            words = list(struct.unpack('<128H', data))
            nz = [hex(w) for w in words if w]
            print(f'{name} @ {va:#x}: nonzero={len(nz)} first20={nz[:20]}')
        for va, name in ((0x4834b4, 'anim flags'), (0x483490, 'frame') , (0x483292, 'frame2')):
            data = dm.read(h, base, va, 8)
            print(name, list(data))
    finally:
        dm.k32.CloseHandle(h)
finally:
    d.close()
