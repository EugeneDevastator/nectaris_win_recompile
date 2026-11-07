"""Select a unit, inject an adjacent enemy, attack, and capture the battle."""
import os, sys, time, struct, ctypes
sys.path.insert(0, os.path.dirname(__file__))
sys.path.insert(0, os.path.join(os.path.dirname(__file__), 'fakeinput'))
from control import Driver
import dump_mem as dm
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
game = os.path.join(ROOT, 'game')
out = os.path.join(ROOT, 'ref', 'battle')
os.makedirs(out, exist_ok=True)

DX = [0, 1, 1, 0, -1, -1]
DY_E = [-1, -1, 0, 1, 0, -1]
DY_O = [-1, 0, 1, 1, 1, 0]
OX, OY = 3, 26

def clusters(img, pred, minn=18):
    px = img.load(); pts = []
    for y in range(img.height):
        for x in range(img.width):
            if pred(*px[x, y]):
                pts.append((x, y))
    cl = []
    for p in pts:
        for c in cl:
            if abs(p[0]-c[0]) < 20 and abs(p[1]-c[1]) < 20:
                c[0] = (c[0]*c[2]+p[0])/(c[2]+1)
                c[1] = (c[1]*c[2]+p[1])/(c[2]+1)
                c[2] += 1
                break
        else:
            cl.append([p[0], p[1], 1])
    return [c for c in cl if c[2] >= minn]

def blue(r, g, b): return b > 170 and g > 110 and r < 130 and b > r + 60
def green(r, g, b): return g > 150 and r < 150 and b < 150 and g-r > 50 and g-b > 50

d = Driver(game)
h = None
try:
    d.fake_activate()
    h = dm.k32.OpenProcess(0x1F0FFF, False, d.proc.pid)
    base = dm.module_base(h)
    def rd(va, n): return dm.read(h, base, va, n)
    def u8(va): return rd(va, 1)[0]
    def wr(va, data):
        got = ctypes.c_size_t()
        return dm.k32.WriteProcessMemory(h, ctypes.c_void_p(base + (va - 0x400000)),
                                         data, len(data), ctypes.byref(got))

    mapW = mapH = 0
    for i in range(90):
        d.tap(0x20)
        if i % 3 == 0: d.click(320, 240)
        d.wake(); time.sleep(0.35)
        mapW = u8(0x495c02); mapH = u8(0x495c03)
        if mapW and mapH: break
    print('map', mapW, mapH)
    time.sleep(0.8)

    t = rd(0x4957a0, 0x38); x = rd(0x495ab0, 0x38); y = rd(0x495af0, 0x38)

    img = d.snap()
    img.save(os.path.join(out, 'map.png'))
    bl = clusters(img, blue)
    bl.sort(key=lambda c: -c[2])
    print('player clusters', [(round(c[0]), round(c[1]), c[2]) for c in bl[:8]])

    atk = -1
    sel_px = None
    for c in bl[:8]:
        cx, cy = int(c[0]-OX), int(c[1]-OY)
        d.click(cx, cy)
        time.sleep(0.45)
        slot = u8(0x48327b)
        if slot < 0x38 and (t[slot] & 0x80) == 0:
            atk = slot
            sel_px = (cx, cy)
            print('selected slot', atk, 'type', t[slot] & 0x1f, 'pixel', sel_px)
            break
    if atk < 0:
        raise SystemExit('no selectable unit')
    # after selection the command panel is up
    d.snap(os.path.join(out, 'selected.png'))

    ax, ay = x[atk] & 0x1f, y[atk] & 0x1f
    free = None
    for dirn in range(6):
        nx = ax + DX[dirn]
        ny = ay + (DY_O[dirn] if (ax & 1) else DY_E[dirn])
        if 0 <= nx < mapW and 0 <= ny < mapH and rd(0x495820 + ny*32 + nx, 1)[0] == 0xff:
            free = (nx, ny)
            break
    print('attacker cell', ax, ay, 'free neighbor', free)
    if not free:
        raise SystemExit('no free neighbor')
    ex, ey = free
    defn = 9
    wr(0x4957a0 + defn, bytes([0x80 | 4]))
    wr(0x4957e0 + defn, bytes([7]))
    wr(0x495ab0 + defn, bytes([ex & 0x1f]))
    wr(0x495af0 + defn, bytes([ey & 0x1f]))
    wr(0x495820 + ey*32 + ex, bytes([0x80 | defn]))
    print('injected enemy at', ex, ey)
    time.sleep(0.4)

    img2 = d.snap()
    img2.save(os.path.join(out, 'enemy_injected.png'))
    gr = clusters(img2, green)
    print('green clusters', [(round(c[0]), round(c[1]), c[2]) for c in gr[:10]])
    if gr and sel_px:
        tgt = min(gr, key=lambda c: (c[0]-OX-sel_px[0])**2 + (c[1]-OY-sel_px[1])**2)
        tgt_px = (int(tgt[0]-OX), int(tgt[1]-OY))
        print('chosen target pixel', tgt_px, 'dist2', (tgt[0]-OX-sel_px[0])**2 + (tgt[1]-OY-sel_px[1])**2)
    else:
        tgt_px = None

    d.click(540, 124)          # 攻撃 in the command panel
    time.sleep(0.5)
    d.snap(os.path.join(out, 'attack_mode.png'))
    print('after attack cmd flags:', u8(0x481a68), u8(0x483276))

    if tgt_px:
        d.click(*tgt_px)
        print('clicked target', tgt_px)
    time.sleep(1.0)
    d.snap(os.path.join(out, 'battle_start.png'))

    for k in range(50):
        d.snap(os.path.join(out, f'f{k:02d}.png'))
        time.sleep(0.07)
    print('captured')
finally:
    if h: dm.k32.CloseHandle(h)
    d.close()
