"""Force the PLAYER unit to attack with a chosen type, capture its combat block.

Mission 1: player slot 5 at (10,2); enemy slot 2 relocated to (10,3) adjacent.
Patch player slot 5 type to T, put the cursor on it, then issue: space, down, space.
"""
import ctypes, ctypes.wintypes as wt, os, sys, time, struct, subprocess, hashlib
sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
OUT = os.path.join(ROOT, 'dump', 'player_attacks')
ASSETS = os.path.join(ROOT, 'assets', 'art', 'combat_units_real')
os.makedirs(OUT, exist_ok=True)
MAP_STATE = 0x416700
TYPES = [int(x) for x in sys.argv[1:]] or [9]

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

raw_pal = open(os.path.join(ROOT, 'dump', 'battle_sprites', 'palette.bin'), 'rb').read()
PAL = [(raw_pal[4 + i * 4], raw_pal[5 + i * 4], raw_pal[6 + i * 4]) for i in range(256)]


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


def export(block, typ):
    for bank, tag in ((9, 'blue'), (10, 'green')):
        c = Image.new('RGBA', (64, 64), (0, 0, 0, 0))
        for j in range(4):
            t = Image.new('RGBA', (32, 32), (0, 0, 0, 0))
            px = t.load()
            for y in range(32):
                for x in range(32):
                    v = block[j * 1024 + y * 32 + x]
                    if v:
                        px[x, 31 - y] = PAL[((bank << 4) | (v & 15)) & 255] + (255,)
            c.alpha_composite(t, ((j % 2) * 32, (j // 2) * 32))
        c.save(os.path.join(ASSETS, 'combat_type_%02d_%s.png' % (typ, tag)))


def one(typ):
    subprocess.run(['taskkill', '/F', '/IM', 'Nec.exe'], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    time.sleep(0.3)
    subprocess.call([sys.executable, os.path.join(ROOT, 'tools', 'orig_play_mission.py'), '1', '--no-preview'],
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

    def wr8(va, v):
        got = ctypes.c_size_t()
        dm.k32.WriteProcessMemory(h, ctypes.c_void_p(base + (va - 0x400000)), bytes([v & 0xFF]), 1, ctypes.byref(got))

    def tap(vk, hold=0.12, gap=0.20):
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
    # player slot 5 at (10,2); enemy slot 2 -> (10,3); player type = T
    sx, sy = dm.read(h, base, 0x495ab0 + 2, 1)[0] & 31, dm.read(h, base, 0x495af0 + 2, 1)[0] & 31
    wr8(0x495ab0 + 2, 10)
    wr8(0x495af0 + 2, 3)
    wr8(0x495820 + sy * 32 + sx, 0)
    wr8(0x495820 + 3 * 32 + 10, 2)
    wr8(0x4957a0 + 5, 0x80 | (typ & 0x1F))
    wr8(0x481a72, 10)   # cursor x
    wr8(0x481a71, 2)    # cursor y
    time.sleep(0.4)
    # open command, choose attack, confirm target
    wake(); tap(0x20, gap=0.5)
    tap(0x28, gap=0.3)   # 攻擊
    tap(0x20, gap=0.6)
    tap(0x20, gap=0.6)
    sb = u32(0x480f14)
    try:
        prev = hashlib.md5(dm.read_abs(h, sb + 0x20000, 0x8000)).hexdigest()[:8]
    except OSError:
        prev = None
    result = 'timeout'
    t1 = time.time()
    while time.time() - t1 < 40:
        wake()
        try:
            block = dm.read_abs(h, sb + 0x20000, 0x8000)
            state = u32(0x483270)
        except OSError:
            result = 'gone'
            break
        hh = hashlib.md5(block).hexdigest()[:8]
        if prev is not None and hh != prev:
            d = os.path.join(OUT, 'type_%02d_%s' % (typ, hh))
            os.makedirs(d, exist_ok=True)
            open(os.path.join(d, 'block.bin'), 'wb').write(block)
            open(os.path.join(d, 'spritebank.bin'), 'wb').write(dm.read_abs(h, sb, 0x100000))
            export(block, typ)
            result = 'ok'
            break
        if state != MAP_STATE:
            tap(0x20, gap=0.1)
        time.sleep(0.05)
    dm.k32.CloseHandle(h)
    subprocess.run(['taskkill', '/F', '/IM', 'Nec.exe'], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    return result


for t in TYPES:
    try:
        print('type %2d: %s' % (t, one(t)), flush=True)
    except Exception as exc:
        print('type %2d: error %s' % (t, exc), flush=True)
print('done', flush=True)
