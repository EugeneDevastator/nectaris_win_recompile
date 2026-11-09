"""Use the fake-input Driver to open MANUAL, but dump frames from memory."""
import os, sys, time, struct
sys.path.insert(0, os.path.dirname(__file__))
sys.path.insert(0, os.path.join(os.path.dirname(__file__), 'fakeinput'))
import dump_mem as dm
from control import Driver
from PIL import Image, ImageChops

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
GAME = os.path.join(ROOT, 'game')
OUT = os.path.join(ROOT, 'dump', 'manual_mem2')
os.makedirs(OUT, exist_ok=True)
REF = Image.open(os.path.join(ROOT, 'dump', 'boot_mem', 'f_088.png')).convert('L')
REF_OPTS = REF.crop((100, 350, 560, 435))

d = Driver(GAME)
try:
    d.fake_activate()
    time.sleep(2.0)
    h = dm.k32.OpenProcess(0x1F0FFF, False, d.proc.pid)
    base = dm.module_base(h)
    def u32(va): return struct.unpack('<I', dm.read(h, base, va, 4))[0]
    for _ in range(100):
        if u32(0x480f18) != 0:
            break
        time.sleep(0.1)
    def frame():
        pal = dm.read(h, base, 0x480f20, 4 + 256 * 4)
        fb = dm.read_abs(h, u32(0x480f18), 704 * 544)
        rgb = bytearray(768)
        for i in range(256):
            rgb[i*3] = pal[4+i*4]; rgb[i*3+1] = pal[5+i*4]; rgb[i*3+2] = pal[6+i*4]
        im = Image.frombytes('P', (704,544), fb).transpose(Image.FLIP_TOP_BOTTOM).crop((32,32,672,512))
        im.putpalette(bytes(rgb))
        return im.convert('RGB')
    def is_menu(im):
        g = im.convert('L').crop((100, 350, 560, 435))
        dd = ImageChops.difference(g, REF_OPTS)
        return sum(dd.get_flattened_data()) / (460*85) < 10.0
    def capture(name):
        frame().save(os.path.join(OUT, name))
        print('capture', name, flush=True)

    # advance intro to menu
    t0 = time.time()
    ok = False
    while time.time() - t0 < 60:
        im = frame()
        if is_menu(im):
            im.save(os.path.join(OUT, 'menu.png'))
            print('menu at %.1fs' % (time.time()-t0), flush=True)
            ok = True
            break
        d.tap(0x20, hold=0.18)
        time.sleep(0.2)
    if not ok:
        print('menu not found', flush=True)
    capture('menu2.png')
    # click MANUAL (once highlights it, twice confirms)
    d.click(210, 405)
    time.sleep(0.6)
    capture('menu_manual_selected.png')
    d.tap(0x0D, hold=0.2)
    time.sleep(1.0)
    capture('manual_000.png')
    d.tap(0x20, hold=0.2)
    time.sleep(1.0)
    capture('manual_001.png')
    for i in range(1, 70):
        time.sleep(0.35)
        capture('manual_%03d.png' % i)
        if i % 10 == 0:
            d.tap(0x20, hold=0.18)
finally:
    d.close()
