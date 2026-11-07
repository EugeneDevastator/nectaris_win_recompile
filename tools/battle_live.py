"""Drive the original into a real combat and capture the animation.

Strategy: the combat resolver reads unit positions from the SoA; the map icons are
drawn from a separate object list.  So we (1) click the enemy icon to learn which
slot it is, (2) move the attacker's SoA cell adjacent to that enemy, (3) select the
attacker via its icon, (4) press 攻撃 and click the enemy icon.  The battle should
then start from the SoA data even though the sprite did not visually move.
"""
import os, sys, time, struct, ctypes
sys.path.insert(0, os.path.dirname(__file__))
sys.path.insert(0, os.path.join(os.path.dirname(__file__), 'fakeinput'))
from control import Driver
import dump_mem as dm
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
game = os.path.join(ROOT, 'game')
out = os.path.join(ROOT, 'ref', 'battle2')
os.makedirs(out, exist_ok=True)

DX = [0, 1, 1, 0, -1, -1]
DY_E = [-1, -1, 0, 1, 0, -1]
DY_O = [-1, 0, 1, 1, 1, 0]

# map icon positions discovered from screenshots
BLUE_ICON = (100, 352)
GREEN_ICON = (456, 303)
ATTACK_BTN = (585, 163)
MOVE_BTN = (585, 131)


def hex_neighbors(x, y, w, h):
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
    print('map', mapW, mapH, 'iter', i)
    time.sleep(1.2)
    d.snap(os.path.join(out, '00_map.png'))

    t = rd(0x4957a0, 0x38)
    st = rd(0x495ab0, 0x38)
    fl = rd(0x495af0, 0x38)
    units = {i: (t[i] & 0x80, t[i] & 0x1f, st[i] & 0x1f, fl[i] & 0x1f)
             for i in range(0x38) if t[i] != 0xff}
    print('units', units)

    # --- click the green icon and see what gets selected
    d.click(*GREEN_ICON)
    time.sleep(0.5)
    sel = u8(0x48327b)
    print('green click -> slot', sel)
    if sel not in units:
        # scan a small grid around the icon
        found = None
        for oy in range(-30, 31, 6):
            for ox in range(-30, 31, 6):
                d.click(GREEN_ICON[0] + ox, GREEN_ICON[1] + oy)
                time.sleep(0.16)
                s2 = u8(0x48327b)
                if s2 in units and (units[s2][0] & 0x80):
                    found = (s2, GREEN_ICON[0] + ox, GREEN_ICON[1] + oy)
                    break
            if found:
                break
        if not found:
            print('could not select an enemy icon')
        else:
            sel = found[0]
            print('enemy selected via', found)
    enemy = sel
    if enemy not in units or not (units[enemy][0] & 0x80):
        raise SystemExit('no enemy selected')
    ex, ey = units[enemy][2], units[enemy][3]
    print('enemy slot', enemy, 'cell', (ex, ey))
    d.snap(os.path.join(out, '01_enemy_selected.png'))

    # --- find a player slot and move its SoA cell next to the enemy
    atk = None
    for i, v in units.items():
        if not (v[0] & 0x80):
            atk = i
            break
    if atk is None:
        raise SystemExit('no player unit')
    ax, ay = units[atk][2], units[atk][3]
    dest = None
    for nx, ny in hex_neighbors(ex, ey, mapW, mapH):
        occ = rd(0x495820 + ny * 32 + nx, 1)[0]
        if occ == 0xff:
            dest = (nx, ny)
            break
    if dest is None:
        raise SystemExit('no free neighbour')
    dx, dy = dest
    print(f'teleport attacker slot {atk} {(ax, ay)} -> {dest}')
    wr(0x495820 + ay * 32 + ax, bytes([0xff]))
    wr(0x495820 + dy * 32 + dx, bytes([atk & 0x3f]))
    wr(0x495ab0 + atk, bytes([(st[atk] & 0xe0) | (dx & 0x1f)]))
    wr(0x495af0 + atk, bytes([(fl[atk] & 0xe0) | (dy & 0x1f)]))
    time.sleep(0.3)

    # --- select the attacker via its icon (still drawn at the old spot)
    d.click(*BLUE_ICON)
    time.sleep(0.5)
    sel2 = u8(0x48327b)
    print('blue click -> slot', sel2)
    if sel2 != atk:
        for oy in range(-30, 31, 6):
            for ox in range(-30, 31, 6):
                d.click(BLUE_ICON[0] + ox, BLUE_ICON[1] + oy)
                time.sleep(0.16)
                if u8(0x48327b) == atk:
                    print('attacker selected via offset', (ox, oy))
                    break
            else:
                continue
            break
    d.snap(os.path.join(out, '02_attacker_selected.png'))
    print('flags: 0x481a68', u8(0x481a68), '0x481a70', u8(0x481a70),
          '0x483276', u8(0x483276))

    # --- attack command
    d.click(*ATTACK_BTN)
    time.sleep(0.6)
    print('after 攻撃: 0x481a68', u8(0x481a68), '0x483276', u8(0x483276),
          '0x48327b', u8(0x48327b), '0x483274', u8(0x483274))
    d.snap(os.path.join(out, '03_attack_mode.png'))

    # --- click the enemy icon
    d.click(*GREEN_ICON)
    time.sleep(0.7)
    print('after target: mode', u8(0x481a68), 'atk', u8(0x48327b), 'def', u8(0x483274))
    d.snap(os.path.join(out, '04_battle_start.png'))

    # --- capture frames
    screen_ptr = u32(0x480f18)
    pal = open(os.path.join(ROOT, 'dump', 'palette_bgr.bin'), 'rb').read()
    PAL = [(pal[i * 4 + 2], pal[i * 4 + 1], pal[i * 4]) for i in range(256)]
    print('screen', hex(screen_ptr), 'battle flags',
          'atk', u8(0x48327b), 'def', u8(0x483274), 'type', u8(0x48327c),
          'ta', u8(0x481db3), 'td', u8(0x481db4), 'ind', u8(0x481dbc))
    for k in range(120):
        try:
            data = dm.read_abs(h, screen_ptr, 704 * 544)
        except OSError:
            break
        img = Image.new('RGB', (640, 480))
        px = img.load()
        for yy in range(480):
            bi = (543 - (yy + 32)) * 704 + 32
            row = data[bi:bi + 640]
            for xx in range(640):
                px[xx, yy] = PAL[row[xx]]
        img.save(os.path.join(out, f'f{k:03d}.png'))
        if k % 20 == 0:
            print('frame', k, 'anim', u8(0x483490), 'tick', u8(0x483492))
        time.sleep(0.045)
    print('captured')
finally:
    if h:
        dm.k32.CloseHandle(h)
    d.close()
