"""Drive to the tactical map with virtual input, then dump decoded art/state."""
import os, sys, time, struct, ctypes
from ctypes import byref
sys.path.insert(0, os.path.dirname(__file__))
sys.path.insert(0, os.path.join(os.path.dirname(__file__), 'fakeinput'))
from control import Driver
import dump_mem as dm

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
OUT = os.path.join(ROOT, 'dump')
os.makedirs(OUT, exist_ok=True)
VK_SPACE = 0x20

d = Driver(os.path.join(ROOT, 'game'))
try:
    print('pid', d.proc.pid)
    d.fake_activate()
    shot = None
    for i in range(70):
        if i % 2 == 0:
            d.tap(VK_SPACE)
        d.wake()
        time.sleep(0.35)
        img = d.snap()
        px = img.load(); w, h = img.size
        black = 0; total = 0
        for yy in range(0, h, 4):
            for xx in range(0, w, 4):
                total += 1
                r, g, b = px[xx, yy]
                if r + g + b < 30:
                    black += 1
        frac = black / total
        if i % 5 == 0:
            print(f'  {i:02d} black={frac:.2f}')
            img.save(os.path.join(ROOT, f'map_try_{i:02d}.png'))
        if i > 4 and frac < 0.05:
            print(f'map detected at iteration {i} (black={frac:.3f})')
            img.save(os.path.join(ROOT, 'gameplay.png'))
            shot = img
            break
    if shot is None:
        print('map not detected')
        d.snap(os.path.join(ROOT, 'gameplay.png'))
    time.sleep(2.5)
    d.snap(os.path.join(ROOT, 'gameplay.png'))

    h = dm.k32.OpenProcess(dm.PROCESS_VM_READ | dm.PROCESS_QUERY_INFORMATION, False, d.proc.pid)
    base = dm.module_base(h)
    print('imagebase', hex(base))
    try:
        pal = dm.read(h, base, 0x480f20, 4 + 256*4)
        pal_bgr = dm.read(h, base, 0x481538, 1024)
        atlas = dm.read(h, base, 0x484730, 0x12000)
        dims = dm.read(h, base, 0x495c02, 2)
        print('map dims', list(dims))
        tile_ptr = dm.u32(dm.read(h, base, 0x481500, 4), 0)
        sprite_ptr = dm.u32(dm.read(h, base, 0x480f14, 4), 0)
        screen_ptr = dm.u32(dm.read(h, base, 0x480f18, 4), 0)
        print('banks', hex(tile_ptr), hex(sprite_ptr), 'screen', hex(screen_ptr))
        open(os.path.join(OUT, 'palette.bin'), 'wb').write(pal)
        open(os.path.join(OUT, 'palette_bgr.bin'), 'wb').write(pal_bgr)
        open(os.path.join(OUT, 'atlas.bin'), 'wb').write(atlas)
        open(os.path.join(OUT, 'tilebank.bin'), 'wb').write(dm.read_abs(h, tile_ptr, 0x80000))
        open(os.path.join(OUT, 'spritebank.bin'), 'wb').write(dm.read_abs(h, sprite_ptr, 0x100000))
        open(os.path.join(OUT, 'screen8.bin'), 'wb').write(dm.read_abs(h, screen_ptr, 704*544))
        rl = dm.read(h, base, 0x4836f0, 0x1000)
        cur = dm.read(h, base, 0x4836e8, 1)[0]
        print('render buffer', cur)
        entries = []
        for buf in (0, 1):
            for i in range(256):
                e = rl[buf*0x800 + i*8: buf*0x800 + i*8 + 8]
                if len(e) < 8: continue
                x, y, si, more = struct.unpack('<HHHH', e)
                if x == 0 and y == 0 and si == 0 and more == 0: continue
                sprite = (si >> 8) + 0x40 + (more & 0xff00)
                entries.append((buf, i, x, y, si, more, sprite, (si & 7) + 8, bool(si & 0x80)))
        print('render entries:', len(entries))
        for e in entries[:40]:
            print('  buf=%d i=%3d x=%4d y=%4d sprite=%4d pal=%2d flip=%s raw=%04x/%04x'
                  % (e[0], e[1], e[2], e[3], e[6], e[7], e[8], e[5], e[4]))
        for name, va, n in [('u_typeowner', 0x4957a0, 0x38), ('u_hp_rank', 0x4957e0, 0x38),
                            ('u_x', 0x495ab0, 0x38), ('u_y', 0x495af0, 0x38),
                            ('terrain', 0x481a80, 0x580), ('occupancy', 0x495820, 0x580),
                            ('renderlist', 0x4836f0, 0x1000),
                            ('tilemap', 0x494740, 0x1000)]:
            open(os.path.join(OUT, name + '.bin'), 'wb').write(dm.read(h, base, va, n))
        cam = dm.read(h, base, 0x495c04, 4)
        print('camera/attr', list(cam))
        open(os.path.join(OUT, 'camera.bin'), 'wb').write(cam)
        t = dm.read(h, base, 0x4957a0, 0x38)
        hr = dm.read(h, base, 0x4957e0, 0x38)
        x = dm.read(h, base, 0x495ab0, 0x38)
        y = dm.read(h, base, 0x495af0, 0x38)
        print('units (slot type owner hp rank x y):')
        for i in range(0x38):
            if t[i] != 0 or hr[i] != 0:
                print(f'  {i:2d} t={t[i]&0x1f:2d} o={t[i]&0x80:3d} hp={hr[i]&7} r={hr[i]>>5} x={x[i]&0x1f:2d} y={y[i]&0x1f:2d}')
        print('palette sample', [(pal[4+i*4],pal[5+i*4],pal[6+i*4]) for i in range(1,8)])
    finally:
        dm.k32.CloseHandle(h)
finally:
    d.close()
