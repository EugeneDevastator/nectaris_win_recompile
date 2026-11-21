"""Boot into a battle, then drive the whole combat and capture every frame
plus the live sprite queue, to find large/animated combat sprites."""
import ctypes, ctypes.wintypes as wt, os, sys, time, struct, subprocess, json
sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
GAME = os.path.join(ROOT, 'game')
OUT = os.path.join(ROOT, 'dump', 'battle_sprites_mem')
os.makedirs(os.path.join(OUT, 'frames'), exist_ok=True)

K32 = ctypes.WinDLL('kernel32', use_last_error=True)
K32.CreateFileMappingW.restype = wt.HANDLE
K32.CreateFileMappingW.argtypes = [wt.HANDLE, ctypes.c_void_p, wt.DWORD, wt.DWORD, wt.DWORD, wt.LPCWSTR]
K32.MapViewOfFile.restype = ctypes.c_void_p
K32.MapViewOfFile.argtypes = [wt.HANDLE, wt.DWORD, wt.DWORD, wt.DWORD, ctypes.c_size_t]
hmap = K32.CreateFileMappingW(wt.HANDLE(-1).value, None, 4, 0, 4096, "Local\\NecFakeInput")
ptr = K32.MapViewOfFile(hmap, 0xF001F, 0, 0, 4096)
ctypes.memset(ptr, 0, 4096)


def i32(off, v=None):
    if v is None:
        return ctypes.c_long.from_address(ptr + off).value
    ctypes.c_long.from_address(ptr + off).value = v


def key(vk, d):
    ctypes.c_ubyte.from_address(ptr + 36 + vk).value = 1 if d else 0


i32(0, 0x4E454331)
i32(4, 1)
i32(12, 0)
i32(16, 0)
i32(28, 0)
for _i in range(256):
    key(_i, 0)

U32 = ctypes.WinDLL('user32', use_last_error=True)
U32.EnumWindows.argtypes = [ctypes.WINFUNCTYPE(wt.BOOL, wt.HWND, wt.LPARAM), wt.LPARAM]
U32.GetWindowThreadProcessId.argtypes = [wt.HWND, ctypes.POINTER(wt.DWORD)]
U32.GetWindowTextW.argtypes = [wt.HWND, wt.LPWSTR, ctypes.c_int]
U32.PostMessageW.argtypes = [wt.HWND, wt.UINT, wt.WPARAM, wt.LPARAM]
U32.MapVirtualKeyW.argtypes = [wt.UINT, wt.UINT]
U32.GetForegroundWindow.restype = wt.HWND
U32.AttachThreadInput.argtypes = [wt.DWORD, wt.DWORD, wt.BOOL]
U32.SetForegroundWindow.argtypes = [wt.HWND]
U32.BringWindowToTop.argtypes = [wt.HWND]
U32.ShowWindow.argtypes = [wt.HWND, ctypes.c_int]
K32.GetCurrentThreadId.restype = wt.DWORD


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


for line in subprocess.check_output(['tasklist', '/FI', 'IMAGENAME eq Nec.exe', '/FO', 'CSV', '/NH'],
                                    text=True).splitlines():
    parts = [x.strip('"') for x in line.split('","')]
    if parts and parts[0].lower() == 'nec.exe':
        subprocess.call(['taskkill', '/F', '/PID', parts[1]], stdout=subprocess.DEVNULL)

proc = subprocess.Popen([os.path.join(GAME, 'Nec.exe')], cwd=GAME)
hwnd = None
for _ in range(200):
    hwnd = find_window(proc.pid)
    if hwnd:
        break
    time.sleep(0.05)
print('pid', proc.pid, 'hwnd', hwnd, flush=True)
h = dm.k32.OpenProcess(0x1F0FFF, False, proc.pid)
base = dm.module_base(h)


def u8(va):
    return dm.read(h, base, va, 1)[0]


def u32(va):
    return struct.unpack('<I', dm.read(h, base, va, 4))[0]


def focus():
    fg = U32.GetForegroundWindow()
    ft = U32.GetWindowThreadProcessId(fg, None)
    mt = K32.GetCurrentThreadId()
    U32.AttachThreadInput(mt, ft, True)
    U32.ShowWindow(hwnd, 5)
    U32.BringWindowToTop(hwnd)
    U32.SetForegroundWindow(hwnd)
    U32.AttachThreadInput(mt, ft, False)
    U32.PostMessageW(hwnd, 0x001C, 1, 0)
    U32.PostMessageW(hwnd, 0x0006, 1, 0)
    U32.PostMessageW(hwnd, 0x0007, 0, 0)


def tap(vk, hold=0.12, gap=0.25):
    focus()
    sc = U32.MapVirtualKeyW(vk, 0) & 0xFF
    key(vk, 1)
    U32.PostMessageW(hwnd, 0x0100, vk, 1 | (sc << 16))
    U32.PostMessageW(hwnd, 0x0102, vk, 1 | (sc << 16))
    time.sleep(hold)
    key(vk, 0)
    U32.PostMessageW(hwnd, 0x0101, vk, 1 | (sc << 16) | 0xC0000000)
    time.sleep(gap)


RIGHT, DOWN, LEFT, UP, SPACE, ENTER = 0x27, 0x28, 0x25, 0x26, 0x20, 0x0D


def frame_rgb():
    pal = dm.read(h, base, 0x480f20, 4 + 256 * 4)
    fbp = u32(0x480f18)
    fb = dm.read_abs(h, fbp, 704 * 544)
    rgb = bytearray(768)
    for i in range(256):
        rgb[i * 3] = pal[4 + i * 4]
        rgb[i * 3 + 1] = pal[5 + i * 4]
        rgb[i * 3 + 2] = pal[6 + i * 4]
    im = Image.frombytes('P', (704, 544), fb).transpose(Image.FLIP_TOP_BOTTOM).crop((32, 32, 672, 512))
    im.putpalette(bytes(rgb))
    return im.convert('RGB')


def read_queue():
    out = []
    for bank in (0, 1):
        raw = dm.read(h, base, 0x4836f0 + bank * 0x800, 0x800)
        for i in range(0x100):
            e = raw[i * 8:(i + 1) * 8]
            y, x = struct.unpack_from('<HH', e, 0)
            if x == 0 and y == 0:
                continue
            out.append({'bank': bank, 'slot': i, 'x': x, 'y': y,
                        'pal': e[4], 'sprite': e[5] | (e[7] << 8)})
    return out


# boot to map
focus()
t0 = time.time()
toggle = False
while time.time() - t0 < 50:
    if u8(0x495c02) == 15 and u8(0x495c03) == 10:
        break
    tap(SPACE if not toggle else ENTER, gap=0.40)
    toggle = not toggle
print('map %.1fs' % (time.time() - t0), flush=True)
time.sleep(3.0)
for _ in range(22):
    tap(LEFT, gap=0.08)
for _ in range(22):
    tap(UP, gap=0.08)
for _ in range(7):
    bx = u8(0x481a72)
    for _ in range(3):
        tap(RIGHT, gap=0.14)
        if u8(0x481a72) > bx:
            break
for _ in range(4):
    by = u8(0x481a71)
    for _ in range(3):
        tap(DOWN, gap=0.14)
        if u8(0x481a71) > by:
            break
for vk in [SPACE, SPACE, RIGHT, RIGHT, SPACE, SPACE, RIGHT, DOWN, SPACE]:
    tap(vk, gap=0.35)
print('battle entry', flush=True)

idx = 0
t1 = time.time()
nxt_key = t1 + 1.0
while time.time() - t1 < 9.0:
    q = read_queue()
    json.dump(q, open(os.path.join(OUT, 'frames', 'f%04d.json' % idx), 'w'))
    if idx % 8 == 0:
        frame_rgb().save(os.path.join(OUT, 'frames', 'f%04d.png' % idx))
    idx += 1
    if time.time() >= nxt_key:
        tap(SPACE, gap=0.05)
        nxt_key = time.time() + 0.9
    time.sleep(0.02)
open(os.path.join(OUT, 'palette.bin'), 'wb').write(dm.read(h, base, 0x480f20, 4 + 256 * 4))
sb_ptr = u32(0x480f14)
if sb_ptr:
    open(os.path.join(OUT, 'spritebank.bin'), 'wb').write(dm.read_abs(h, sb_ptr, 0x100000))
print('frames', idx, 'spritebank', hex(sb_ptr), flush=True)
dm.k32.CloseHandle(h)
try:
    proc.terminate()
except Exception:
    pass
print('done', flush=True)
