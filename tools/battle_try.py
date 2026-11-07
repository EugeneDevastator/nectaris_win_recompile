"""Try to trigger a real battle from the tactical map.

The combat resolver reads SoA positions, so we teleport the attacker's SoA cell
adjacent to each enemy in turn, then drive the real menu: select unit -> 攻撃 ->
click the enemy icon.  If the battle screen appears we capture the frame stream
and the battle globals.  When the target is not the visible enemy, the attack
just cancels and we try the next one.
"""
import os, sys, time, struct, ctypes
sys.path.insert(0, os.path.dirname(__file__))
sys.path.insert(0, os.path.join(os.path.dirname(__file__), 'fakeinput'))
from control import Driver
import dump_mem as dm
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
game = os.path.join(ROOT, 'game')
out = os.path.join(ROOT, 'ref', 'battle3')
os.makedirs(out, exist_ok=True)

DX = [0, 1, 1, 0, -1, -1]
DY_E = [-1, -1, 0, 1, 0, -1]
DY_O = [-1, 0, 1, 1, 1, 0]
ATTACK_BTN = (585, 163)


def clusters(img, pred, minn=20, box=(10, 35, 535, 455)):
    x0, y0, x1, y1 = box
    px = img.load(); pts = []
    for y in range(y0, y1):
        for x in range(x0, x1):
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


def blue_icon(r, g, b):
    return b > 200 and 90 < g < 200 and r < 160 and b - r > 60


def green_icon(r, g, b):
    return g > 100 and g - r > 18 and g - b > 18 and r > 55 and b > 55


def neighbors(x, y, w, h):
    out = []
    for d in range(6):
        nx = x + DX[d]
        ny = y + (DY_O[d] if (x & 1) else DY_E[d])
        if 0 <= nx < w and 0 <= ny < h:
            out.append((nx, ny))
    return out


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
    time.sleep(1.2)

    t = rd(0x4957a0, 0x38)
    st = rd(0x495ab0, 0x38)
    fl = rd(0x495af0, 0x38)
    units = {i: (t[i] & 0x80, t[i] & 0x1f, st[i] & 0x1f, fl[i] & 0x1f)
             for i in range(0x38) if t[i] != 0xff}
    players = [i for i, v in units.items() if not (v[0] & 0x80)]
    enemies = [i for i, v in units.items() if (v[0] & 0x80)]
    atk = players[0]
    ax, ay = units[atk][2], units[atk][3]
    print('player', atk, (ax, ay), 'enemies', [(e, units[e][2:]) for e in enemies])

    img = d.snap()
    img.save(os.path.join(out, 'map.png'))
    bl = clusters(img, blue_icon)
    gr = clusters(img, green_icon)
    print('blue icons', [(round(c[0]), round(c[1]), c[2]) for c in bl[:5]])
    print('green icons', [(round(c[0]), round(c[1]), c[2]) for c in gr[:8]])
    if not bl or not gr:
        raise SystemExit('could not find icons')
    blue_px = (int(bl[0][0]), int(bl[0][1]))
    green_px = (int(gr[0][0]), int(gr[0][1]))

    def combat_active():
        return u8(0x495f3c) != 0 or u8(0x483274) in units and u8(0x483274) in enemies

    captured = False
    for enemy in enemies:
        ex, ey = units[enemy][2], units[enemy][3]
        dest = None
        for nx, ny in neighbors(ex, ey, mapW, mapH):
            if rd(0x495820 + ny * 32 + nx, 1)[0] == 0xff:
                dest = (nx, ny)
                break
        if not dest:
            continue
        dx, dy = dest
        # teleport attacker
        wr(0x495820 + ay * 32 + ax, bytes([0xff]))
        wr(0x495820 + dy * 32 + dx, bytes([atk & 0x3f]))
        wr(0x495ab0 + atk, bytes([(st[atk] & 0xe0) | (dx & 0x1f)]))
        wr(0x495af0 + atk, bytes([(fl[atk] & 0xe0) | (dy & 0x1f)]))
        ax, ay = dx, dy
        time.sleep(0.2)
        print(f'--- enemy {enemy} at {(ex, ey)}; attacker -> {(dx, dy)}')

        # select attacker
        d.click(*blue_px)
        time.sleep(0.4)
        if u8(0x48327b) != atk:
            d.click(blue_px[0], blue_px[1] - 15)
            time.sleep(0.3)
        print('  selected', u8(0x48327b))
        # attack
        d.click(*ATTACK_BTN)
        time.sleep(0.4)
        # click enemy icon
        d.click(*green_px)
        time.sleep(0.9)
        print('  after target: atk', u8(0x48327b), 'def', u8(0x483274),
              'flag495f3c', u8(0x495f3c), 'task', hex(u32(0x482330)))
        d.snap(os.path.join(out, f'after_{enemy:02d}.png'))
        if u8(0x483274) == enemy:
            print('  BATTLE STARTED with enemy', enemy)
            captured = True
            break
        # cancel / deselect for next attempt
        d.tap(0x1B)  # ESC
        time.sleep(0.3)
        d.click(300, 400)  # empty-ish
        time.sleep(0.3)

    if captured:
        pal = open(os.path.join(ROOT, 'dump', 'palette_bgr.bin'), 'rb').read()
        PAL = [(pal[i * 4 + 2], pal[i * 4 + 1], pal[i * 4]) for i in range(256)]
        sp = u32(0x480f18)
        for k in range(100):
            data = dm.read_abs(h, sp, 704 * 544)
            img = Image.new('RGB', (640, 480))
            px = img.load()
            for yy in range(480):
                bi = (543 - (yy + 32)) * 704 + 32
                row = data[bi:bi + 640]
                for xx in range(640):
                    px[xx, yy] = PAL[row[xx]]
            img.save(os.path.join(out, f'f{k:03d}.png'))
            if k % 20 == 0:
                print('frame', k, 'anim', u8(0x483490), 'tick', u8(0x483492),
                      'def', u8(0x483274))
            time.sleep(0.04)
        print('captured battle frames')
    else:
        print('no battle triggered')
finally:
    if h:
        dm.k32.CloseHandle(h)
    d.close()
