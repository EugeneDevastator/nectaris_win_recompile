import os, sys, time, struct, ctypes
sys.path.insert(0, os.path.dirname(__file__))
sys.path.insert(0, os.path.join(os.path.dirname(__file__), 'fakeinput'))
from control import Driver
import dump_mem as dm
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
game = os.path.join(ROOT, 'game')
out = os.path.join(ROOT, 'ref')
os.makedirs(out, exist_ok=True)

bgr = open(os.path.join(ROOT, 'dump', 'palette_bgr.bin'), 'rb').read()
PAL = [(bgr[i*4+2], bgr[i*4+1], bgr[i*4]) for i in range(256)]

d = Driver(game)
h = None
try:
    d.fake_activate()
    h = dm.k32.OpenProcess(0x1F0FFF, False, d.proc.pid)
    base = dm.module_base(h)
    print('base', hex(base))

    def rd(va, n): return dm.read(h, base, va, n)
    def u8(va): return rd(va, 1)[0]
    def u32(va): return struct.unpack('<I', rd(va, 4))[0]
    def wr(va, data):
        got = ctypes.c_size_t()
        return dm.k32.WriteProcessMemory(h, ctypes.c_void_p(base + (va - 0x400000)),
                                         data, len(data), ctypes.byref(got))

    # drive to the tactical map: briefing needs a confirm, map dims become nonzero
    mapW = mapH = 0
    for i in range(80):
        d.tap(0x20)
        if i % 3 == 0:
            d.click(320, 240)
        d.wake()
        time.sleep(0.35)
        mapW = u8(0x495c02)
        mapH = u8(0x495c03)
        if mapW and mapH:
            print('map reached at', i, mapW, mapH)
            break
    time.sleep(0.8)
    print('dims', mapW, mapH)

    t = rd(0x4957a0, 0x38)
    x = rd(0x495ab0, 0x38)
    y = rd(0x495af0, 0x38)
    atk = defn = -1
    for i in range(0x38):
        if t[i] == 0xff: continue
        if (t[i] & 0x80) == 0 and atk < 0: atk = i
        if (t[i] & 0x80) != 0 and defn < 0: defn = i
    print('atk', atk, 'def', defn)
    if atk < 0:
        print('no player unit'); raise SystemExit

    terr = rd(0x481a80, 0x580)
    if defn < 0:
        defn = 9
        ax = x[atk] & 0x1f
        ay = y[atk] & 0x1f
        ex, ey = ax + 1, ay
        if ex >= mapW: ex, ey = ax - 1, ay
        if ex < 0: ex, ey = ax, ay + 1
        wr(0x4957a0 + defn, bytes([0x80 | 4]))
        wr(0x4957e0 + defn, bytes([7]))
        wr(0x495ab0 + defn, bytes([ex & 0x1f]))
        wr(0x495af0 + defn, bytes([ey & 0x1f]))
        wr(0x495820 + ey * 32 + ex, bytes([0x80 | defn]))
        print(f'injected enemy slot {defn} at {ex},{ey}')

    ta = terr[(y[atk] & 0x1f) * 32 + (x[atk] & 0x1f)] & 0xF
    td = terr[(y[defn] & 0x1f) * 32 + (x[defn] & 0x1f)] & 0xF
    print('terrain', ta, td, 'units', atk, defn)

    wr(0x48327b, bytes([atk]))
    wr(0x483274, bytes([defn]))
    wr(0x48327c, bytes([t[atk] & 0x1f]))
    wr(0x481db3, bytes([ta]))
    wr(0x481db4, bytes([td]))
    wr(0x481dbc, bytes([0]))
    task = u32(0x482330)
    print('task', hex(task))
    if task:
        wr(task + 8, struct.pack('<I', 0x417240))
    wr(0x481d18, bytes([0]))

    screen_ptr = u32(0x480f18)
    print('screen', hex(screen_ptr))
    for k in range(28):
        wr(0x481d18, bytes([0]))
        time.sleep(0.12)
        data = dm.read_abs(h, screen_ptr, 704*544)
        img = Image.new('RGB', (640, 480))
        px = img.load()
        for yy in range(480):
            bi = (543 - (yy + 32)) * 704 + 32
            row = data[bi:bi+640]
            for xx in range(640):
                px[xx, yy] = PAL[row[xx]]
        img.save(os.path.join(out, f'live_combat_{k:02d}.png'))
    print('captured 28 frames')
finally:
    if h: dm.k32.CloseHandle(h)
    d.close()
