"""Force the original combat screen from the tactical map by hijacking the task callback."""
import os, sys, time, struct, ctypes
sys.path.insert(0, os.path.dirname(__file__))
sys.path.insert(0, os.path.join(os.path.dirname(__file__), 'fakeinput'))
from control import Driver
import dump_mem as dm
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
game = os.path.join(ROOT, 'game')
out = os.path.join(ROOT, 'ref', 'forced')
os.makedirs(out, exist_ok=True)

d = Driver(game)
h = None
try:
    d.fake_activate()
    h = dm.k32.OpenProcess(0x1F0FFF, False, d.proc.pid)
    base = dm.module_base(h)

    def rd(va, n):
        return dm.read(h, base, va, n)

    def u8(va):
        return rd(va, 1)[0]

    def u32(va):
        return struct.unpack('<I', rd(va, 4))[0]

    def wr(va, data):
        got = ctypes.c_size_t()
        return dm.k32.WriteProcessMemory(h, ctypes.c_void_p(base + (va - 0x400000)),
                                         data, len(data), ctypes.byref(got))

    mapW = mapH = 0
    for i in range(100):
        d.tap(0x20)
        if i % 3 == 0:
            d.click(320, 240)
        d.wake(); time.sleep(0.35)
        mapW = u8(0x495c02); mapH = u8(0x495c03)
        if mapW and mapH:
            break
    print('map', mapW, mapH)
    time.sleep(1.0)

    t = rd(0x4957a0, 0x38)
    st = rd(0x495ab0, 0x38)
    fl = rd(0x495af0, 0x38)
    terr = rd(0x481a80, 0x580)
    print('tasks: 0x482330 ->', hex(u32(0x482330)))
    # print a few task fields
    tp = u32(0x482330)
    if 0x400000 <= tp < 0x500000:
        print('task bytes:', list(rd(tp, 0x40)))

    atk = defn = -1
    for i in range(0x38):
        if t[i] == 0xff:
            continue
        if (t[i] & 0x80) == 0 and atk < 0:
            atk = i
        if (t[i] & 0x80) != 0 and defn < 0:
            defn = i
    print('atk', atk, 'def', defn)
    ax, ay = st[atk] & 0x1f, fl[atk] & 0x1f
    dx, dy = st[defn] & 0x1f, fl[defn] & 0x1f
    ta = terr[ay * 32 + ax] & 0xf
    td = terr[dy * 32 + dx] & 0xf
    print('cells', (ax, ay), (dx, dy), 'terrain', ta, td)

    wr(0x48327b, bytes([atk]))
    wr(0x483274, bytes([defn]))
    wr(0x48327c, bytes([t[atk] & 0x1f]))
    wr(0x481db3, bytes([ta]))
    wr(0x481db4, bytes([td]))
    wr(0x481dbc, bytes([0]))
    wr(0x4834b4, bytes([0]))
    wr(0x483491, bytes([0]))
    wr(0x483490, bytes([0]))
    wr(0x483492, bytes([0]))

    # Point the current task at the combat screen entry.
    if 0x400000 <= tp < 0x500000:
        wr(tp + 8, struct.pack('<I', 0x415580))
    time.sleep(0.4)

    pal = open(os.path.join(ROOT, 'dump', 'palette_bgr.bin'), 'rb').read()
    PAL = [(pal[i * 4 + 2], pal[i * 4 + 1], pal[i * 4]) for i in range(256)]
    for k in range(30):
        try:
            sp = u32(0x480f18)
            data = dm.read_abs(h, sp, 704 * 544)
            img = Image.new('RGB', (640, 480))
            px = img.load()
            for yy in range(480):
                bi = (543 - (yy + 32)) * 704 + 32
                row = data[bi:bi + 640]
                for xx in range(640):
                    px[xx, yy] = PAL[row[xx]]
            img.save(os.path.join(out, f'f{k:02d}.png'))
            if k % 5 == 0:
                print('fb frame', k, 'screen', hex(sp))
        except Exception as e:
            print('fb err', k, e)
        time.sleep(0.1)
    print('done, task now', hex(u32(0x482330)))
finally:
    if h:
        dm.k32.CloseHandle(h)
    d.close()
