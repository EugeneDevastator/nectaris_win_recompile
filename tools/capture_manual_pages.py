"""Capture every MANUAL/tutorial page from the 8bpp framebuffer in memory.

Flow:
  1. boot, advance intro to the main menu
  2. real mouse click on MANUAL -> manual top menu
  3. for each submenu item: real click, then advance pages with Space,
     capturing every distinct frame until the manual menu reappears
  4. "タイトルに戻る" returns to the title screen

The framebuffer (0x480f18 pointer, 704x544) is read from process memory;
no PrintWindow screenshots are used.
"""
import ctypes, ctypes.wintypes as wt, os, sys, time, struct, subprocess, hashlib
sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm
from PIL import Image, ImageChops

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
GAME = os.path.join(ROOT, 'game')
OUT = os.path.join(ROOT, 'dump', 'manual_pages')
os.makedirs(OUT, exist_ok=True)
REF = Image.open(os.path.join(ROOT, 'dump', 'boot_mem', 'f_088.png')).convert('L')
REF_OPTS = REF.crop((100, 350, 560, 435))

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
i32(4, 0)
i32(12, 0)
i32(16, 0)
i32(28, 0)
for _i in range(256):
    key(_i, 0)

U32 = ctypes.WinDLL('user32', use_last_error=True)
U32.SetProcessDPIAware()
U32.EnumWindows.argtypes = [ctypes.WINFUNCTYPE(wt.BOOL, wt.HWND, wt.LPARAM), wt.LPARAM]
U32.GetWindowThreadProcessId.argtypes = [wt.HWND, ctypes.POINTER(wt.DWORD)]
U32.GetWindowTextW.argtypes = [wt.HWND, wt.LPWSTR, ctypes.c_int]
U32.GetClientRect.argtypes = [wt.HWND, ctypes.POINTER(wt.RECT)]
U32.ClientToScreen.argtypes = [wt.HWND, ctypes.POINTER(wt.POINT)]
U32.PostMessageW.argtypes = [wt.HWND, wt.UINT, wt.WPARAM, wt.LPARAM]
U32.MapVirtualKeyW.argtypes = [wt.UINT, wt.UINT]
U32.GetForegroundWindow.restype = wt.HWND
U32.AttachThreadInput.argtypes = [wt.DWORD, wt.DWORD, wt.BOOL]
U32.SetForegroundWindow.argtypes = [wt.HWND]
U32.BringWindowToTop.argtypes = [wt.HWND]
U32.ShowWindow.argtypes = [wt.HWND, ctypes.c_int]
U32.SetCursorPos.argtypes = [ctypes.c_int, ctypes.c_int]
U32.mouse_event.argtypes = [wt.DWORD, wt.DWORD, wt.DWORD, wt.DWORD, ctypes.c_void_p]
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


subprocess.run(['taskkill', '/F', '/IM', 'Nec.exe'], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
time.sleep(0.4)
p = subprocess.Popen([os.path.join(GAME, 'Nec.exe')], cwd=GAME)
hwnd = None
for _ in range(200):
    hwnd = find_window(p.pid)
    if hwnd:
        break
    time.sleep(0.05)
print('hwnd', hwnd, 'pid', p.pid, flush=True)
time.sleep(0.8)
h = dm.k32.OpenProcess(0x1F0FFF, False, p.pid)
base = dm.module_base(h)


def u32(va):
    return struct.unpack('<I', dm.read(h, base, va, 4))[0]


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


def tap(vk, hold=0.14, gap=0.35):
    sc = U32.MapVirtualKeyW(vk, 0) & 0xFF
    key(vk, 1)
    U32.PostMessageW(hwnd, 0x0100, vk, 1 | (sc << 16))
    U32.PostMessageW(hwnd, 0x0102, vk, 1 | (sc << 16))
    time.sleep(hold)
    key(vk, 0)
    U32.PostMessageW(hwnd, 0x0101, vk, 1 | (sc << 16) | 0xC0000000)
    time.sleep(gap)


def is_main_menu(im):
    g = im.convert('L').crop((100, 350, 560, 435))
    d = ImageChops.difference(g, REF_OPTS)
    return sum(d.get_flattened_data()) / (460 * 85) < 10.0


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


def real_click(cx, cy):
    r = wt.RECT()
    U32.GetClientRect(hwnd, ctypes.byref(r))
    scale = r.right / 640.0
    pt = wt.POINT(int(cx * scale), int(cy * scale))
    U32.ClientToScreen(hwnd, ctypes.byref(pt))
    focus()
    U32.SetCursorPos(pt.x, pt.y)
    time.sleep(0.3)
    U32.mouse_event(0x0002, 0, 0, 0, 0)
    time.sleep(0.15)
    U32.mouse_event(0x0004, 0, 0, 0, 0)
    time.sleep(0.8)


# --- advance intro to the main menu ---
t0 = time.time()
ok = False
while time.time() - t0 < 60:
    im = frame()
    if is_main_menu(im):
        ok = True
        print('main menu %.1fs' % (time.time() - t0), flush=True)
        break
    tap(0x20)
if not ok:
    print('main menu not found', flush=True)

# --- open MANUAL ---
real_click(205, 405)
time.sleep(1.2)
MENU_IMG = frame()
MENU_IMG.save(os.path.join(OUT, 'manual_menu.png'))
MENU_G = MENU_IMG.convert('L')

MENU_Y = [140, 202, 262, 322, 386]
names = ['flow', 'commands', 'transport', 'capture', 'back']


def same_menu(im):
    g = im.convert('L')
    d = ImageChops.difference(g, MENU_G)
    return sum(d.get_flattened_data()) / (640 * 480) < 4.0


seen = {hashlib.md5(MENU_IMG.tobytes()).hexdigest()}
cap_n = [0]


def capture_until_menu(prefix, timeout=75.0, interval=0.28, advance_every=1.1):
    n0 = cap_n[0]
    t_start = time.time()
    nxt_cap = t_start
    nxt_adv = t_start + 1.4
    while time.time() - t_start < timeout:
        now = time.time()
        if now >= nxt_cap:
            im = frame()
            digest = hashlib.md5(im.tobytes()).hexdigest()
            if digest not in seen:
                seen.add(digest)
                im.save(os.path.join(OUT, '%s_%03d.png' % (prefix, cap_n[0])))
                cap_n[0] += 1
            nxt_cap = now + interval
        if now >= nxt_adv:
            tap(0x20, gap=0.10)
            nxt_adv = now + advance_every
        # only accept "back at menu" after the section actually started
        if now - t_start > 4.0 and same_menu(frame()):
            print(prefix, 'returned to menu after %.1fs' % (now - t_start), flush=True)
            break
        time.sleep(0.02)
    return cap_n[0] - n0


for idx, name in enumerate(names[:4]):
    real_click(300, MENU_Y[idx])
    time.sleep(0.8)
    frame().save(os.path.join(OUT, 'enter_%s.png' % name))
    n = capture_until_menu(name)
    print(name, 'new frames', n, flush=True)
    # nudge back in case the section stopped on a final page
    if not same_menu(frame()):
        tap(0x1B, gap=0.8)
        frame().save(os.path.join(OUT, 'after_back_%s.png' % name))
    time.sleep(0.5)

# --- title return ---
real_click(300, MENU_Y[4])
time.sleep(1.5)
frame().save(os.path.join(OUT, 'title_after_back.png'))
print('total unique frames', cap_n[0], flush=True)
print('done', flush=True)
dm.k32.CloseHandle(h)
