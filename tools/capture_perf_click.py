"""Open the 性能 unit info screen with a real mouse click and capture memory frames."""
import os, sys, time, struct, subprocess
sys.path.insert(0, os.path.dirname(__file__))
sys.path.insert(0, os.path.join(os.path.dirname(__file__), 'fakeinput'))
import dump_mem as dm
from control import Driver
from PIL import Image, ImageChops

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
GAME = os.path.join(ROOT, 'game')
OUT = os.path.join(ROOT, 'dump', 'perf_screen')
os.makedirs(OUT, exist_ok=True)

d = Driver(GAME)
try:
    d.fake_activate()
    time.sleep(2.0)
    h = dm.k32.OpenProcess(0x1F0FFF, False, d.proc.pid)
    base = dm.module_base(h)

    def u32(va):
        return struct.unpack('<I', dm.read(h, base, va, 4))[0]

    def u8(va):
        return dm.read(h, base, va, 1)[0]

    def frame():
        pal = dm.read(h, base, 0x480f20, 4 + 256 * 4)
        fb = dm.read_abs(h, u32(0x480f18), 704 * 544)
        rgb = bytearray(768)
        for i in range(256):
            rgb[i * 3] = pal[4 + i * 4]
            rgb[i * 3 + 1] = pal[5 + i * 4]
            rgb[i * 3 + 2] = pal[6 + i * 4]
        im = Image.frombytes('P', (704, 544), fb).transpose(Image.FLIP_TOP_BOTTOM).crop((32, 32, 672, 512))
        im.putpalette(bytes(rgb))
        return im.convert('RGB')

    # advance intro to the map
    t0 = time.time()
    while time.time() - t0 < 60:
        if u8(0x495c02) == 15 and u8(0x495c03) == 10:
            break
        d.tap(0x20, hold=0.18)
        time.sleep(0.2)
    print('map %.1fs' % (time.time() - t0), flush=True)
    time.sleep(2.5)
    frame().save(os.path.join(OUT, 'map.png'))

    # select the unit at the cursor, then click 性能
    d.click(320, 240)
    time.sleep(0.5)
    frame().save(os.path.join(OUT, 'after_select.png'))
    for y in (148, 150, 145, 155):
        d.click(570, y)
        time.sleep(0.8)
        frame().save(os.path.join(OUT, 'after_perf_%d.png' % y))
        print('clicked 性能 at y', y, flush=True)
    for i in range(24):
        time.sleep(0.3)
        frame().save(os.path.join(OUT, 'p_%02d.png' % i))
    print('done', flush=True)
finally:
    d.close()
