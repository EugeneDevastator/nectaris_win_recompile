"""Boot to the tactical map, dump units, try moving an existing unit in memory."""
import os, sys, time, struct, ctypes
sys.path.insert(0, os.path.dirname(__file__))
sys.path.insert(0, os.path.join(os.path.dirname(__file__), 'fakeinput'))
from control import Driver
import dump_mem as dm
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
game = os.path.join(ROOT, 'game')
out = os.path.join(ROOT, 'ref', 'live')
os.makedirs(out, exist_ok=True)

DX = [0, 1, 1, 0, -1, -1]
DY_E = [-1, -1, 0, 1, 0, -1]
DY_O = [-1, 0, 1, 1, 1, 0]


def clusters(img, pred, minn=25):
    px = img.load(); pts = []
    for y in range(img.height):
        for x in range(img.width):
            if pred(*px[x, y]):
                pts.append((x, y))
    cl = []
    for p in pts:
        for c in cl:
            if abs(p[0] - c[0]) < 22 and abs(p[1] - c[1]) < 22:
                c[0] = (c[0] * c[2] + p[0]) / (c[2] + 1)
                c[1] = (c[1] * c[2] + p[1]) / (c[2] + 1)
                c[2] += 1
                break
        else:
            cl.append([p[0], p[1], 1])
    return sorted([c for c in cl if c[2] >= minn], key=lambda c: -c[2])


def blue(r, g, b):
    return b > 150 and g > 90 and b > r + 50 and r < 140


def green(r, g, b):
    return g > 110 and g - r > 35 and g - b > 35 and r < 160


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

    def wr(va, data):
        got = ctypes.c_size_t()
        return dm.k32.WriteProcessMemory(h, ctypes.c_void_p(base + (va - 0x400000)),
                                         data, len(data), ctypes.byref(got))

    mapW = mapH = 0
    for i in range(90):
        d.tap(0x20)
        if i % 3 == 0:
            d.click(320, 240)
        d.wake(); time.sleep(0.35)
        mapW = u8(0x495c02); mapH = u8(0x495c03)
        if mapW and mapH:
            break
    print('map', mapW, mapH, 'iter', i)
    time.sleep(1.0)

    t = rd(0x4957a0, 0x38)
    hp = rd(0x4957e0, 0x38)
    st = rd(0x495ab0, 0x38)
    fl = rd(0x495af0, 0x38)
    units = []
    for i in range(0x38):
        if t[i] == 0xff:
            continue
        units.append((i, t[i] & 0x80, t[i] & 0x1f, st[i] & 0x1f, fl[i] & 0x1f,
                      hp[i] & 7, st[i], fl[i]))
    print('units:', len(units))
    for u in units:
        print('  slot=%d owner=%d type=%d x=%d y=%d hp=%d st=%02x fl=%02x' % u)

    img = d.snap()
    img.save(os.path.join(out, 'map0.png'))
    bl = clusters(img, blue)
    gr = clusters(img, green)
    print('blue clusters', [(round(c[0]), round(c[1]), c[2]) for c in bl[:12]])
    print('green clusters', [(round(c[0]), round(c[1]), c[2]) for c in gr[:12]])

    players = [u for u in units if u[1] == 0]
    enemies = [u for u in units if u[1] != 0]
    if not players or not enemies:
        raise SystemExit('need both sides')
    atk = players[0]
    # move it next to the first enemy (same row)
    ax, ay = atk[3], atk[4]
    ex, ey = enemies[0][3], enemies[0][4]
    # find a hex adjacent to enemy
    dest = None
    for dd in range(6):
        nx = ex + DX[dd]
        ny = ey + (DY_O[dd] if (ex & 1) else DY_E[dd])
        if 0 <= nx < mapW and 0 <= ny < mapH:
            dest = (nx, ny)
            break
    print('moving slot', atk[0], 'from', (ax, ay), 'to', dest, 'enemy at', (ex, ey))
    dx, dy = dest
    # clear old occupancy, set new
    old = rd(0x495820 + ay * 32 + ax, 1)[0]
    wr(0x495820 + ay * 32 + ax, bytes([0xff]))
    wr(0x495820 + dy * 32 + dx, bytes([0x00 | atk[0]]))
    wr(0x495ab0 + atk[0], bytes([(atk[6] & 0xe0) | (dx & 0x1f)]))
    wr(0x495af0 + atk[0], bytes([(atk[7] & 0xe0) | (dy & 0x1f)]))
    print('old occupancy', hex(old))
    time.sleep(1.2)
    img2 = d.snap()
    img2.save(os.path.join(out, 'map_moved.png'))
    bl2 = clusters(img2, blue)
    print('blue clusters after', [(round(c[0]), round(c[1]), c[2]) for c in bl2[:12]])
finally:
    if h:
        dm.k32.CloseHandle(h)
    d.close()
