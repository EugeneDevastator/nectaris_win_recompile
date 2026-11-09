"""Skip the player turn (down x3 + space) so the AI attacks; capture everything."""
import ctypes, ctypes.wintypes as wt, os, sys, time, struct, subprocess
sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
OUT = os.path.join(ROOT, 'dump', 'ai_turn2')
os.makedirs(OUT, exist_ok=True)
MAP_STATE = 0x416700

U32 = ctypes.WinDLL('user32', use_last_error=True)
U32.SetProcessDPIAware()
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
K32 = ctypes.WinDLL('kernel32', use_last_error=True)
K32.CreateFileMappingW.restype = wt.HANDLE
K32.CreateFileMappingW.argtypes = [wt.HANDLE, ctypes.c_void_p, wt.DWORD, wt.DWORD, wt.DWORD, wt.LPCWSTR]
K32.MapViewOfFile.restype = ctypes.c_void_p
K32.MapViewOfFile.argtypes = [wt.HANDLE, wt.DWORD, wt.DWORD, wt.DWORD, ctypes.c_size_t]
hmap = K32.CreateFileMappingW(wt.HANDLE(-1).value, None, 4, 0, 4096, "Local\\NecFakeInput")
ptr = K32.MapViewOfFile(hmap, 0xF001F, 0, 0, 4096)
ctypes.memset(ptr, 0, 4096)
K32.GetCurrentThreadId.restype = wt.DWORD


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


def find_pid():
    for line in subprocess.check_output(['tasklist', '/FI', 'IMAGENAME eq Nec.exe', '/FO', 'CSV', '/NH'],
                                        text=True).splitlines():
        p = [x.strip('"') for x in line.split('","')]
        if p and p[0].lower() == 'nec.exe':
            return int(p[1])
    return None


pid = find_pid()
if pid is None:
    raise SystemExit('Nec.exe not running')
hwnd = find_window(pid)
h = dm.k32.OpenProcess(0x1F0FFF, False, pid)
base = dm.module_base(h)
print('pid', pid, 'hwnd', hwnd, flush=True)


def u32(va):
    return struct.unpack('<I', dm.read(h, base, va, 4))[0]


def frame():
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
    return im.convert('RGB'), pal


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


def tap(vk, hold=0.12, gap=0.18):
    sc = U32.MapVirtualKeyW(vk, 0) & 0xFF
    focus()
    key(vk, 1)
    U32.PostMessageW(hwnd, 0x0100, vk, 1 | (sc << 16))
    U32.PostMessageW(hwnd, 0x0102, vk, 1 | (sc << 16))
    time.sleep(hold)
    key(vk, 0)
    U32.PostMessageW(hwnd, 0x0101, vk, 1 | (sc << 16) | 0xC0000000)
    time.sleep(gap)


im, pal = frame()
im.save(os.path.join(OUT, 'start.png'))
open(os.path.join(OUT, 'palette.bin'), 'wb').write(pal)
print('state', hex(u32(0x483270)), flush=True)

frame_no = 0
skips = 0
t0 = time.time()
next_cap = t0
next_sb = t0
next_skip = t0 + 1.0
next_space = t0 + 1.0
last_state = None
while time.time() - t0 < 150.0:
    now = time.time()
    st = u32(0x483270)
    if st != last_state:
        print('state ->', hex(st), 'at %.1fs' % (now - t0), flush=True)
        last_state = st
    if now >= next_cap:
        im, pal = frame()
        im.save(os.path.join(OUT, 'f%03d.png' % frame_no))
        frame_no += 1
        next_cap = now + 0.22
    if now >= next_sb:
        sbp = u32(0x480f14)
        if sbp:
            open(os.path.join(OUT, 'sb_%03d.bin' % frame_no), 'wb').write(dm.read_abs(h, sbp, 0x100000))
        next_sb = now + 1.5
    if st == MAP_STATE and now >= next_skip:
        # skip turn: down x3, space
        tap(0x28, gap=0.10)
        tap(0x28, gap=0.10)
        tap(0x28, gap=0.10)
        tap(0x20, gap=0.30)
        skips += 1
        print('skip turn', skips, 'at %.1fs' % (now - t0), flush=True)
        next_skip = now + 6.0
        next_space = now + 2.0
    elif st != MAP_STATE and now >= next_space:
        tap(0x20, gap=0.05)
        next_space = now + 0.8
    time.sleep(0.03)
print('frames', frame_no, 'skips', skips, flush=True)
dm.k32.CloseHandle(h)
