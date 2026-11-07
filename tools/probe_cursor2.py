import os, sys, time, struct, ctypes
sys.path.insert(0, os.path.dirname(__file__))
sys.path.insert(0, os.path.join(os.path.dirname(__file__), 'fakeinput'))
from control import Driver
import dump_mem as dm

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
game = os.path.join(ROOT, 'game')
d = Driver(game)
h = None
try:
    d.fake_activate()
    h = dm.k32.OpenProcess(0x1F0FFF, False, d.proc.pid)
    base = dm.module_base(h)
    def rd(va, n): return dm.read(h, base, va, n)
    def u8(va): return rd(va, 1)[0]
    def u32(va): return struct.unpack('<I', rd(va, 4))[0]
    def wi(va): return struct.unpack('<i', rd(va, 4))[0]

    for i in range(80):
        d.tap(0x20)
        if i % 3 == 0: d.click(320, 240)
        d.wake(); time.sleep(0.35)
        if u8(0x495c02) and u8(0x495c03): break
    print('map', u8(0x495c02), u8(0x495c03))
    time.sleep(0.8)
    print('bounds: 0x480884=%d 0x480ef0=%d 0x481428=%d 0x4814e8=%d' %
          (u8(0x480884), u8(0x480ef0), u8(0x481428), u8(0x4814e8)))
    print('flags: 0x4814f8=%d 0x42fb68=%d 0x480ed0=%d 0x480888=%d' %
          (u8(0x4814f8), u8(0x42fb68), u8(0x480ed0), u8(0x480888)))
    for (cx, cy, label) in [(98,325,'unit'), (300,240,'center'), (150,200,'a'),
                            (250,300,'b'), (400,200,'c'), (100,100,'d')]:
        d.move(cx, cy)
        time.sleep(0.35)
        print(f'{label} client=({cx},{cy}) cursor=({u8(0x480ef8)},{u8(0x480efc)}) '
              f'flag={u8(0x4814fc)}')
    # click known unit and read again
    d.click(98, 325)
    time.sleep(0.4)
    print('after click unit: slot', u8(0x48327b), 'cursor',
          (u8(0x480ef8), u8(0x480efc)), 'flag', u8(0x4814fc))
finally:
    if h: dm.k32.CloseHandle(h)
    d.close()
