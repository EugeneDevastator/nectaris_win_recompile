"""Enumerate combat unit types by forcing each to be the AI attacker.

For each type T: launch mission 6, patch all enemy units to T, skip turns until
the combat block changes, save the full state, and export the attacker's 64x64
quadrant composition (slots 128..131) as a PNG.
"""
import ctypes, ctypes.wintypes as wt, os, sys, time, struct, subprocess, hashlib, json
sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
OUT = os.path.join(ROOT, 'dump', 'type_captures')
ASSETS = os.path.join(ROOT, 'assets', 'art', 'combat_units_real')
os.makedirs(OUT, exist_ok=True)
os.makedirs(ASSETS, exist_ok=True)
MAP_STATE = 0x416700
TYPES = [int(x) for x in sys.argv[1:]] or list(range(23))
TMO = float(os.environ.get('TMO', '55'))

K32 = ctypes.WinDLL('kernel32', use_last_error=True)
K32.CreateFileMappingW.restype = wt.HANDLE
K32.CreateFileMappingW.argtypes = [wt.HANDLE, ctypes.c_void_p, wt.DWORD, wt.DWORD, wt.DWORD, wt.LPCWSTR]
K32.MapViewOfFile.restype = ctypes.c_void_p
K32.MapViewOfFile.argtypes = [wt.HANDLE, wt.DWORD, wt.DWORD, wt.DWORD, ctypes.c_size_t]
hmap = K32.CreateFileMappingW(wt.HANDLE(-1).value, None, 4, 0, 4096, "Local\\NecFakeInput")
ptr = K32.MapViewOfFile(hmap, 0xF001F, 0, 0, 4096)
ctypes.memset(ptr, 0, 4096)
ctypes.c_long.from_address(ptr + 0).value = 0x4E454331
ctypes.c_long.from_address(ptr + 4).value = 1
seq = [0]

U32 = ctypes.WinDLL('user32', use_last_error=True)
U32.EnumWindows.argtypes = [ctypes.WINFUNCTYPE(wt.BOOL, wt.HWND, wt.LPARAM), wt.LPARAM]
U32.GetWindowThreadProcessId.argtypes = [wt.HWND, ctypes.POINTER(wt.DWORD)]
U32.GetWindowTextW.argtypes = [wt.HWND, wt.LPWSTR, ctypes.c_int]
U32.PostMessageW.argtypes = [wt.HWND, wt.UINT, wt.WPARAM, wt.LPARAM]
U32.MapVirtualKeyW.argtypes = [wt.UINT, wt.UINT]


def find_pid():
    for line in subprocess.check_output(['tasklist', '/FI', 'IMAGENAME eq Nec.exe', '/FO', 'CSV', '/NH'],
                                        text=True).splitlines():
        p = [x.strip('"') for x in line.split('","')]
        if p and p[0].lower() == 'nec.exe':
            return int(p[1])
    return None


def find_window(pid):
    found = []

    @ctypes.WINFUNCTYPE(wt.BOOL, wt.HWND, wt.LPARAM)
    def cb(hwnd, lp):
        p = wt.DWORD()
        U32.GetWindowThreadProcessId(hwnd, ctypes.byref(p))
        if p.value == pid:
            b = ctypes.create_unicode_buffer(256)
            U32.GetWindowTextW(hwnd, b, 256)
            if 'nectaris' in b.value.lower():
                found.append(hwnd)
        return True

    U32.EnumWindows(cb, 0)
    return found[0] if found else None


PAL = None


def load_battle_palette():
    global PAL
    raw = open(os.path.join(ROOT, 'dump', 'battle_sprites', 'palette.bin'), 'rb').read()
    PAL = [(raw[4 + i * 4], raw[5 + i * 4], raw[6 + i * 4]) for i in range(256)]


def export_attacker(block, typ):    # quadrants 128..131 composed 2x2 into 64x64
    for bank, tag in ((9, 'blue'), (10, 'green')):
        c = Image.new('RGBA', (64, 64), (0, 0, 0, 0))
        for j in range(4):
            t = Image.new('RGBA', (32, 32), (0, 0, 0, 0))
            px = t.load()
            base = j * 1024
            for y in range(32):
                for x in range(32):
                    v = block[base + y * 32 + x]
                    if v:
                        px[x, 31 - y] = PAL[((bank << 4) | (v & 15)) & 255] + (255,)
            c.alpha_composite(t, ((j % 2) * 32, (j // 2) * 32))
        c.save(os.path.join(ASSETS, 'combat_type_%02d_%s.png' % (typ, tag)))


def one_type(typ):
    subprocess.run(['taskkill', '/F', '/IM', 'Nec.exe'], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    time.sleep(0.3)
    subprocess.call([sys.executable, os.path.join(ROOT, 'tools', 'orig_play_mission.py'), '6', '--no-preview'],
                    stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    time.sleep(0.5)
    pid = find_pid()
    if pid is None:
        return 'no game'
    hwnd = find_window(pid)
    h = dm.k32.OpenProcess(0x1F0FFF, False, pid)
    base = dm.module_base(h)

    def u32(va):
        return struct.unpack('<I', dm.read(h, base, va, 4))[0]

    def tap(vk, hold=0.10, gap=0.14):
        sc = U32.MapVirtualKeyW(vk, 0) & 0xFF
        ctypes.c_ubyte.from_address(ptr + 36 + vk).value = 1
        U32.PostMessageW(hwnd, 0x0100, vk, 1 | (sc << 16))
        U32.PostMessageW(hwnd, 0x0102, vk, 1 | (sc << 16))
        time.sleep(hold)
        ctypes.c_ubyte.from_address(ptr + 36 + vk).value = 0
        U32.PostMessageW(hwnd, 0x0101, vk, 1 | (sc << 16) | 0xC0000000)
        time.sleep(gap)

    def wake():
        seq[0] += 1
        ctypes.c_long.from_address(ptr + 8).value = seq[0]
        U32.PostMessageW(hwnd, 0x0200, 0, 0)
        U32.PostMessageW(hwnd, 0x0000, 0, 0)

    # wait for map
    t0 = time.time()
    while time.time() - t0 < 30:
        wake()
        try:
            if u32(0x483270) == MAP_STATE:
                break
        except OSError:
            pass
        time.sleep(0.15)
    time.sleep(1.5)
    # patch enemies to the requested type
    to = dm.read(h, base, 0x4957a0, 0x38)
    got = ctypes.c_size_t()
    n = 0
    for i, t in enumerate(to):
        if t and not (t & 0x80) and (t & 0x1F) != 0x1F:
            dm.k32.WriteProcessMemory(h, ctypes.c_void_p(base + (0x4957a0 + i - 0x400000)),
                                      bytes([(t & 0x80) | (typ & 0x1F)]), 1, ctypes.byref(got))
            n += 1
    sb = u32(0x480f14)
    try:
        prev = hashlib.md5(dm.read_abs(h, sb + 0x20000, 0x8000)).hexdigest()[:8]
    except OSError:
        prev = None
    # drive turns
    t1 = time.time()
    last_skip = 0.0
    last_space = 0.0
    result = 'timeout'
    while time.time() - t1 < TMO:
        wake()
        try:
            block = dm.read_abs(h, sb + 0x20000, 0x8000)
            state = u32(0x483270)
        except OSError:
            result = 'process gone'
            break
        hh = hashlib.md5(block).hexdigest()[:8]
        if prev is not None and hh != prev and state != MAP_STATE:
            d = os.path.join(OUT, 'type_%02d_%s' % (typ, hh))
            os.makedirs(d, exist_ok=True)
            open(os.path.join(d, 'block.bin'), 'wb').write(block)
            open(os.path.join(d, 'atlas.bin'), 'wb').write(dm.read(h, base, 0x484730, 0x20000))
            open(os.path.join(d, 'palette.bin'), 'wb').write(dm.read(h, base, 0x480f20, 4 + 256 * 4))
            try:
                pal = dm.read(h, base, 0x480f20, 4 + 256 * 4)
                fb = dm.read_abs(h, u32(0x480f18), 704 * 544)
                rgb = bytearray(768)
                for i in range(256):
                    rgb[i * 3] = pal[4 + i * 4]
                    rgb[i * 3 + 1] = pal[5 + i * 4]
                    rgb[i * 3 + 2] = pal[6 + i * 4]
                im = Image.frombytes('P', (704, 544), fb).transpose(Image.FLIP_TOP_BOTTOM).crop((32, 32, 672, 512))
                im.putpalette(bytes(rgb))
                im.convert('RGB').save(os.path.join(d, 'frame.png'))
            except Exception:
                pass
            open(os.path.join(d, 'units_type.bin'), 'wb').write(dm.read(h, base, 0x4957a0, 0x38))
            try:
                export_attacker(block, typ)
            except Exception as exc:
                result = 'export fail %s' % exc
            result = 'ok'
            break
        now = time.time()
        if state == MAP_STATE:
            if now - last_skip > 5.0:
                tap(0x28); tap(0x28); tap(0x28); tap(0x20, gap=0.25)
                last_skip = now
        else:
            if now - last_space > 0.7:
                tap(0x20, gap=0.05)
                last_space = now
        time.sleep(0.03)
    dm.k32.CloseHandle(h)
    subprocess.run(['taskkill', '/F', '/IM', 'Nec.exe'], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    return '%s enemies=%d' % (result, n)


load_battle_palette()
for t in TYPES:
    try:
        r = one_type(t)
    except Exception as exc:
        r = 'error %s' % exc
    print('type %2d: %s' % (t, r), flush=True)
print('done', flush=True)
