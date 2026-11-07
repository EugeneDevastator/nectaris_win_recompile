"""Virtual input controller for Nec.exe.

Uses the winmm.dll proxy's shared memory section so the game sees synthetic
mouse/keyboard state while the real cursor and keyboard are untouched.

Usage:
    python control.py demo
"""
import ctypes, ctypes.wintypes as wt, os, subprocess, sys, time, struct
from ctypes import c_int32, c_ubyte, POINTER, byref

# ---------------------------------------------------------------- shared mem
K32 = ctypes.WinDLL('kernel32', use_last_error=True)
K32.CreateFileMappingW.restype = wt.HANDLE
K32.CreateFileMappingW.argtypes = [wt.HANDLE, ctypes.c_void_p, wt.DWORD, wt.DWORD, wt.DWORD, wt.LPCWSTR]
K32.MapViewOfFile.restype = ctypes.c_void_p
K32.MapViewOfFile.argtypes = [wt.HANDLE, wt.DWORD, wt.DWORD, wt.DWORD, ctypes.c_size_t]

INVALID_HANDLE_VALUE = wt.HANDLE(-1).value
PAGE_READWRITE = 0x04
FILE_MAP_ALL_ACCESS = 0xF001F
FAKE_MAGIC = 0x4E454331
MAPNAME = "Local\\NecFakeInput"

class FakeInput(ctypes.Structure):
    _fields_ = [
        ("magic", c_int32), ("enabled", c_int32), ("seq", c_int32),
        ("screenX", c_int32), ("screenY", c_int32),
        ("clientX", c_int32), ("clientY", c_int32),
        ("buttons", c_int32), ("wheel", c_int32),
        ("keys", c_ubyte * 256),
    ]

class INPUT_CTX:
    def __init__(self):
        self.hmap = K32.CreateFileMappingW(INVALID_HANDLE_VALUE, None,
                                           PAGE_READWRITE, 0, 4096, MAPNAME)
        if not self.hmap:
            raise ctypes.WinError(ctypes.get_last_error())
        ptr = K32.MapViewOfFile(self.hmap, FILE_MAP_ALL_ACCESS, 0, 0, 4096)
        if not ptr:
            raise ctypes.WinError(ctypes.get_last_error())
        self.ptr = ptr
        self.fi = ctypes.cast(ptr, POINTER(FakeInput)).contents
        self.fi.magic = FAKE_MAGIC
        self.fi.enabled = 0
        self.seq = 0

    def enable(self, on=True):
        self.fi.enabled = 1 if on else 0
        self.bump()

    def bump(self):
        self.seq += 1
        self.fi.seq = self.seq

# ---------------------------------------------------------------- user32
U32 = ctypes.WinDLL('user32', use_last_error=True)
G32 = ctypes.WinDLL('gdi32', use_last_error=True)

class POINT(ctypes.Structure):
    _fields_ = [("x", wt.LONG), ("y", wt.LONG)]
class RECT(ctypes.Structure):
    _fields_ = [("left", wt.LONG), ("top", wt.LONG), ("right", wt.LONG), ("bottom", wt.LONG)]

U32.EnumWindows.argtypes = [ctypes.WINFUNCTYPE(wt.BOOL, wt.HWND, wt.LPARAM), wt.LPARAM]
U32.GetWindowThreadProcessId.argtypes = [wt.HWND, POINTER(wt.DWORD)]
U32.GetWindowTextW.argtypes = [wt.HWND, wt.LPWSTR, ctypes.c_int]
U32.ClientToScreen.argtypes = [wt.HWND, POINTER(POINT)]
U32.GetClientRect.argtypes = [wt.HWND, POINTER(RECT)]
U32.PrintWindow.argtypes = [wt.HWND, wt.HDC, wt.UINT]
U32.GetDC.restype = wt.HDC
U32.GetDC.argtypes = [wt.HWND]
U32.ReleaseDC.argtypes = [wt.HWND, wt.HDC]
U32.PostMessageW.argtypes = [wt.HWND, wt.UINT, wt.WPARAM, wt.LPARAM]
U32.PostMessageW.restype = wt.BOOL
U32.MapVirtualKeyW.argtypes = [wt.UINT, wt.UINT]
U32.MapVirtualKeyW.restype = wt.UINT
WM_NULL = 0x0000
WM_MOUSEMOVE = 0x0200
WM_KEYDOWN = 0x0100
WM_KEYUP = 0x0101
WM_CHAR = 0x0102
WM_ACTIVATE = 0x0006
WM_ACTIVATEAPP = 0x001C
WM_SETFOCUS = 0x0007
G32.CreateCompatibleDC.restype = wt.HDC
G32.CreateCompatibleDC.argtypes = [wt.HDC]
G32.CreateCompatibleBitmap.restype = wt.HBITMAP
G32.CreateCompatibleBitmap.argtypes = [wt.HDC, ctypes.c_int, ctypes.c_int]
G32.SelectObject.restype = wt.HGDIOBJ
G32.SelectObject.argtypes = [wt.HDC, wt.HGDIOBJ]
G32.DeleteObject.argtypes = [wt.HGDIOBJ]
G32.DeleteDC.argtypes = [wt.HDC]
G32.GetDIBits.argtypes = [wt.HDC, wt.HBITMAP, wt.UINT, wt.UINT, ctypes.c_void_p, ctypes.c_void_p, wt.UINT]

def find_window(pid, title="Nectaris"):
    found = []
    @ctypes.WINFUNCTYPE(wt.BOOL, wt.HWND, wt.LPARAM)
    def cb(hwnd, lp):
        p = wt.DWORD()
        U32.GetWindowThreadProcessId(hwnd, byref(p))
        if p.value == pid:
            buf = ctypes.create_unicode_buffer(256)
            U32.GetWindowTextW(hwnd, buf, 256)
            if title.lower() in buf.value.lower():
                found.append((hwnd, buf.value))
        return True
    U32.EnumWindows(cb, 0)
    return found[0][0] if found else None

class BITMAPINFOHEADER(ctypes.Structure):
    _fields_ = [("biSize", wt.DWORD), ("biWidth", wt.LONG), ("biHeight", wt.LONG),
                ("biPlanes", wt.WORD), ("biBitCount", wt.WORD), ("biCompression", wt.DWORD),
                ("biSizeImage", wt.DWORD), ("biXPelsPerMeter", wt.LONG), ("biYPelsPerMeter", wt.LONG),
                ("biClrUsed", wt.DWORD), ("biClrImportant", wt.DWORD)]
class BITMAPINFO(ctypes.Structure):
    _fields_ = [("bmiHeader", BITMAPINFOHEADER), ("bmiColors", wt.DWORD * 3)]

def snapshot(hwnd, path=None):
    from PIL import Image
    r = RECT(); U32.GetClientRect(hwnd, byref(r))
    w, h = r.right - r.left, r.bottom - r.top
    hdc = U32.GetDC(hwnd)
    mdc = G32.CreateCompatibleDC(hdc)
    bmp = G32.CreateCompatibleBitmap(hdc, w, h)
    old = G32.SelectObject(mdc, bmp)
    ok = U32.PrintWindow(hwnd, mdc, 2)
    bi = BITMAPINFO()
    bi.bmiHeader.biSize = ctypes.sizeof(BITMAPINFOHEADER)
    bi.bmiHeader.biWidth = w
    bi.bmiHeader.biHeight = -h
    bi.bmiHeader.biPlanes = 1
    bi.bmiHeader.biBitCount = 32
    bi.bmiHeader.biCompression = 0
    buf = ctypes.create_string_buffer(w * h * 4)
    G32.GetDIBits(mdc, bmp, 0, h, buf, byref(bi), 0)
    G32.SelectObject(mdc, old)
    G32.DeleteObject(bmp)
    G32.DeleteDC(mdc)
    U32.ReleaseDC(hwnd, hdc)
    img = Image.frombuffer('RGB', (w, h), buf, 'raw', 'BGRX', 0, 1)
    if path:
        img.save(path)
    return img

# ---------------------------------------------------------------- driver
class Driver:
    def __init__(self, game_dir, exe="Nec.exe"):
        self.ctx = INPUT_CTX()
        self.proc = subprocess.Popen([os.path.join(game_dir, exe)], cwd=game_dir)
        self.hwnd = None
        for _ in range(100):
            self.hwnd = find_window(self.proc.pid)
            if self.hwnd:
                break
            time.sleep(0.1)
        if not self.hwnd:
            raise RuntimeError("window not found")
        self.ctx.enable(True)
        self.client_origin = POINT()

    def origin(self):
        p = POINT(0, 0)
        U32.ClientToScreen(self.hwnd, byref(p))
        return p.x, p.y

    def move(self, cx, cy):
        ox, oy = self.origin()
        self.ctx.fi.clientX = cx
        self.ctx.fi.clientY = cy
        self.ctx.fi.screenX = ox + cx
        self.ctx.fi.screenY = oy + cy
        self.ctx.bump()
        # wake the game's WaitMessage loop; lParam carries client coords for
        # any WM_MOUSEMOVE handling it may do itself
        lp = ((cy & 0xFFFF) << 16) | (cx & 0xFFFF)
        U32.PostMessageW(self.hwnd, WM_MOUSEMOVE, 0, lp)
        U32.PostMessageW(self.hwnd, WM_NULL, 0, 0)

    def wake(self):
        U32.PostMessageW(self.hwnd, WM_NULL, 0, 0)

    def post_key(self, vk, char=True):
        """Synthesize a keystroke through the window message queue."""
        sc = U32.MapVirtualKeyW(vk, 0) & 0xFF
        down = 1 | (sc << 16)
        up = 1 | (sc << 16) | 0xC0000000
        U32.PostMessageW(self.hwnd, WM_KEYDOWN, vk, down)
        if char:
            U32.PostMessageW(self.hwnd, WM_CHAR, vk, down)
        U32.PostMessageW(self.hwnd, WM_KEYUP, vk, up)

    def post_key_down(self, vk):
        sc = U32.MapVirtualKeyW(vk, 0) & 0xFF
        U32.PostMessageW(self.hwnd, WM_KEYDOWN, vk, 1 | (sc << 16))

    def post_key_up(self, vk):
        sc = U32.MapVirtualKeyW(vk, 0) & 0xFF
        U32.PostMessageW(self.hwnd, WM_KEYUP, vk, 1 | (sc << 16) | 0xC0000000)

    def tap(self, vk, hold=0.18):
        """Hold a key for >1 game tick (20 ms) so the poller sees it."""
        self.post_key_down(vk)
        time.sleep(hold)
        self.post_key_up(vk)
        time.sleep(0.06)

    def fake_activate(self):
        """Tell the game it is active/focused without stealing real focus."""
        U32.PostMessageW(self.hwnd, WM_ACTIVATEAPP, 1, 0)
        U32.PostMessageW(self.hwnd, WM_ACTIVATE, 1, 0)  # WA_ACTIVE
        U32.PostMessageW(self.hwnd, WM_SETFOCUS, 0, 0)

    def buttons(self, mask):
        self.ctx.fi.buttons = mask
        self.ctx.bump()

    def key(self, vk, down):
        self.ctx.fi.keys[vk] = 1 if down else 0
        self.ctx.bump()

    def click(self, cx, cy, settle=0.25, hold=0.15):
        self.move(cx, cy)
        time.sleep(settle)
        self.buttons(1)
        time.sleep(hold)
        self.buttons(0)
        time.sleep(0.15)

    def snap(self, path=None):
        return snapshot(self.hwnd, path)

    def diag(self):
        raw = ctypes.string_at(self.ctx.ptr + 512, 24)
        return struct.unpack('<6i', raw)

    def vkseen(self):
        raw = ctypes.string_at(self.ctx.ptr + 1024, 256)
        return [i for i, b in enumerate(raw) if b]

    def close(self):
        self.ctx.fi.enabled = 0
        try:
            self.proc.terminate()
            self.proc.wait(timeout=3)
        except Exception:
            self.proc.kill()

def menu_metrics(img):
    px = img.load()
    w, h = img.size
    sx, sy = w / 640.0, h / 480.0
    x0, x1 = int(130 * sx), min(w, int(520 * sx))
    bands = [(int(366 * sy), int(386 * sy)), (int(414 * sy), int(434 * sy))]
    total = black = 0
    whites = []
    for (y0, y1) in bands:
        wcount = 0; b = 0; t = 0
        for y in range(max(0, y0), min(h, y1)):
            for x in range(max(0, x0), x1):
                r, g, bl = px[x, y]
                t += 1
                if r < 24 and g < 24 and bl < 24: b += 1
                if r > 170 and g > 170 and bl > 170: wcount += 1
        whites.append(wcount); total += t; black += b
    if total == 0:
        return 0.0, whites
    return black / total, whites

if __name__ == "__main__":
    game = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..', 'game'))
    d = Driver(game)
    try:
        print("hwnd", d.hwnd, "pid", d.proc.pid)
        menu_img = None
        for i in range(200):
            d.wake()
            img = d.snap()
            bf, whites = menu_metrics(img)
            if i % 5 == 0:
                print(f"t={i*0.4:4.1f}s black={bf:.2f} white={whites}")
            if bf > 0.55 and whites[0] > 800 and whites[1] > 700:
                print(f"menu detected at {i*0.4:.1f}s black={bf:.2f} white={whites}")
                img.save(r"C:\OpenCode-Drive\SharedF\nectariswin\ctl_menu.png")
                menu_img = img
                break
            if bf > 0.6 and whites[0] > 300 and i % 5 == 0:
                img.save(rf"C:\OpenCode-Drive\SharedF\nectariswin\ctl_t{i:03d}.png")
            if i % 3 == 2:
                d.click(320, 240)      # advance prologue / attract with fake click
            time.sleep(0.4)
        if not menu_img:
            print("menu not detected")
            d.snap(r"C:\OpenCode-Drive\SharedF\nectariswin\ctl_timeout.png")
        else:
            d.click(205, 376)
            time.sleep(2.5)
            d.snap(r"C:\OpenCode-Drive\SharedF\nectariswin\ctl_start.png")
            print("clicked START")
    finally:
        d.close()
