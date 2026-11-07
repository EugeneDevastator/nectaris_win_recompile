"""Drive the original into a real battle and capture the scene."""
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

d = Driver(game)
h = None
try:
    d.fake_activate()
    h = dm.k32.OpenProcess(0x1F0FFF, False, d.proc.pid)
    base = dm.module_base(h)
    def rd(va, n): return dm.read(h, base, va, n)
    def u8(va): return rd(va, 1)[0]
    def u32(va): return struct.unpack('<I', rd(va, 4))[0]
    def wr(va, data):
        got = ctypes.c_size_t()
        return dm.k32.WriteProcessMemory(h, ctypes.c_void_p(base + (va - 0x400000)),
                                         data, len(data), ctypes.byref(got))

    # 1. drive to the tactical map
    mapW = mapH = 0
    for i in range(90):
        d.tap(0x20)
        if i % 3 == 0:
            d.click(320, 240)
        d.wake()
        time.sleep(0.35)
        mapW = u8(0x495c02)
        mapH = u8(0x495c03)
        if mapW and mapH:
            break
    print('map', mapW, mapH, 'iter', i)
    time.sleep(0.8)

    # 2. calibrate cell -> screen pixel via the game's cursor-cell globals
    cells = {}
    for py in range(40, 470, 14):
        for px in range(2, 520, 14):
            d.move(px, py)
            time.sleep(0.022)
            cx = u8(0x480ef8)
            cy = u8(0x480efc)
            if cx < mapW and cy < mapH and (cx, cy) not in cells:
                cells[(cx, cy)] = (px, py)
    print('calibrated cells:', len(cells))
    for k in list(cells)[:8]:
        print('  ', k, cells[k])

    # 3. units
    t = rd(0x4957a0, 0x38); x = rd(0x495ab0, 0x38); y = rd(0x495af0, 0x38)
    atk = defn = -1
    for i in range(0x38):
        if t[i] == 0xff: continue
        if (t[i] & 0x80) == 0 and atk < 0: atk = i
        if (t[i] & 0x80) != 0 and defn < 0: defn = i
    print('atk', atk, (x[atk] & 0x1f, y[atk] & 0x1f) if atk >= 0 else None, 'def', defn)
    if atk < 0:
        raise SystemExit('no player unit')

    # 4. inject an enemy adjacent to the attacker if needed
    if defn < 0:
        defn = 9
        ax, ay = x[atk] & 0x1f, y[atk] & 0x1f
        ex, ey = ax + 1, ay
        if ex >= mapW: ex, ey = ax - 1, ay
        wr(0x4957a0 + defn, bytes([0x80 | 4]))
        wr(0x4957e0 + defn, bytes([7]))
        wr(0x495ab0 + defn, bytes([ex & 0x1f]))
        wr(0x495af0 + defn, bytes([ey & 0x1f]))
        wr(0x495820 + ey * 32 + ex, bytes([0x80 | defn]))
        print(f'injected enemy slot {defn} at {ex},{ey}')
    tc = (x[atk] & 0x1f, y[atk] & 0x1f)
    dc = (x[defn] & 0x1f, y[defn] & 0x1f)
    print('cells', tc, dc, 'pixels', cells.get(tc), cells.get(dc))

    # 5. select the attacker
    if tc in cells:
        d.click(*cells[tc])
        time.sleep(0.6)
        print('selected slot', u8(0x48327b))
        d.snap(os.path.join(out, 'step_selected.png'))

    # 6. click 攻撃 in the command panel (screen coords, calibrated from screenshots)
    d.click(545, 150)
    time.sleep(0.6)
    d.snap(os.path.join(out, 'step_attack.png'))
    print('after attack cmd: 0x481a68', u8(0x481a68), '0x483276',
          u8(0x483276) if True else 0)

    # 7. click the enemy cell
    if dc in cells:
        d.click(*cells[dc])
        time.sleep(1.2)
        d.snap(os.path.join(out, 'step_battle.png'))
        print('battle? dims', u8(0x495c02), u8(0x495c03))

    # 8. capture frames
    for k in range(40):
        d.snap(os.path.join(out, f'f{k:02d}.png'))
        time.sleep(0.08)
    print('captured frames')
finally:
    if h: dm.k32.CloseHandle(h)
    d.close()
