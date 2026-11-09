"""Open the 性能 info screen for a chosen unit type and dump generated chunks.

Mission 1: exit action selection, patch blue slot 2 to type T, cursor to it,
choose 性能, then dump spritebank slots 64..191.
"""
import ctypes, ctypes.wintypes as wt, os, sys, time, struct, subprocess, json
sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
OUT = os.path.join(ROOT, 'dump', 'perf_chunks')
os.makedirs(OUT, exist_ok=True)
TYPES = [int(x) for x in sys.argv[1:]] or list(range(23))

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

    def rd8(va):
        return dm.read(h, base, va, 1)[0]

    def wr8(va, v):
        got = ctypes.c_size_t()
        dm.k32.WriteProcessMemory(h, ctypes.c_void_p(base + (va - 0x400000)), bytes([v & 0xFF]), 1, ctypes.byref(got))

    def tap(vk, hold=0.12, gap=0.22):
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

    def frame(name):
        pal = dm.read(h, base, 0x480f20, 4 + 256 * 4)
        fb = dm.read_abs(h, u32(0x480f18), 704 * 544)
        rgb = bytearray(768)
        for i in range(256):
            rgb[i * 3] = pal[4 + i * 4]
            rgb[i * 3 + 1] = pal[5 + i * 4]
            rgb[i * 3 + 2] = pal[6 + i * 4]
        im = Image.frombytes('P', (704, 544), fb).transpose(Image.FLIP_TOP_BOTTOM).crop((32, 32, 672, 512))
        im.putpalette(bytes(rgb))
        im.convert('RGB').save(os.path.join(OUT, name))

    t0 = time.time()
    while time.time() - t0 < 30:
        wake()
        if u32(0x483270) == 0x416700:
            break
        time.sleep(0.15)
    time.sleep(2.5)
    tap(0x20, gap=0.6)  # exit action selection
    wr8(0x4957a0 + 2, typ & 31)  # blue slot 2 type
    for _ in range(40):
        x, y = rd8(0x481a72), rd8(0x481a71)
        if x == 4 and y == 8:
            break
        if x < 4:
            tap(0x27, gap=0.10)
        elif x > 4:
            tap(0x25, gap=0.10)
        elif y < 8:
            tap(0x28, gap=0.10)
        elif y > 8:
            tap(0x26, gap=0.10)
    tap(0x20, gap=0.5)   # menu
    tap(0x28, gap=0.25)  # 攻擊
    tap(0x28, gap=0.25)  # 性能
    tap(0x20, gap=1.0)   # open
    frame('unit_%02d.png' % typ)
    sb = u32(0x480f14)
    block = dm.read_abs(h, sb + 64 * 1024, 0x20000)  # slots 64..191
    open(os.path.join(OUT, 'unit_%02d_block.bin' % typ), 'wb').write(block)
    open(os.path.join(OUT, 'unit_%02d_spritebank.bin' % typ), 'wb').write(dm.read_abs(h, sb, 0x100000))
    dm.k32.CloseHandle(h)
    subprocess.run(['taskkill', '/F', '/IM', 'Nec.exe'], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    return 'ok'


for t in TYPES:
    try:
        print('type %2d: %s' % (t, one(t)), flush=True)
    except Exception as exc:
        print('type %2d: error %s' % (t, exc), flush=True)
print('done', flush=True)
