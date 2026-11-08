"""Capture the WEAPON DATA guide screen for all 24 unit types.

The guide is opened from the map: select the unit at (7,4), then choose
MOVE -> STATS (Space, Down, Down, Space).  The selected slot's type is patched
between captures.
"""
import ctypes, ctypes.wintypes as wt, os, sys, time, struct, subprocess
from PIL import Image
sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
OUT = os.path.join(ROOT, 'dump', 'guide')
os.makedirs(OUT, exist_ok=True)
GUIDE_STATE = 0x418d20

K32 = ctypes.WinDLL('kernel32', use_last_error=True)
K32.CreateFileMappingW.restype = wt.HANDLE
K32.CreateFileMappingW.argtypes = [wt.HANDLE, ctypes.c_void_p, wt.DWORD, wt.DWORD, wt.DWORD, wt.LPCWSTR]
K32.MapViewOfFile.restype = ctypes.c_void_p
K32.MapViewOfFile.argtypes = [wt.HANDLE, wt.DWORD, wt.DWORD, wt.DWORD, ctypes.c_size_t]
hmap = K32.CreateFileMappingW(wt.HANDLE(-1).value, None, 4, 0, 4096, "Local\\NecFakeInput")
ptr = K32.MapViewOfFile(hmap, 0xF001F, 0, 0, 4096)
def i32(off, v=None):
    if v is None: return ctypes.c_long.from_address(ptr + off).value
    ctypes.c_long.from_address(ptr + off).value = v
def key(vk, d):
    ctypes.c_ubyte.from_address(ptr + 36 + vk).value = 1 if d else 0
i32(0, 0x4E454331); i32(4, 1); i32(12, 0); i32(16, 0); i32(28, 0)

U32 = ctypes.WinDLL('user32', use_last_error=True)
G32 = ctypes.WinDLL('gdi32', use_last_error=True)
U32.EnumWindows.argtypes = [ctypes.WINFUNCTYPE(wt.BOOL, wt.HWND, wt.LPARAM), wt.LPARAM]
U32.GetWindowThreadProcessId.argtypes = [wt.HWND, ctypes.POINTER(wt.DWORD)]
U32.GetWindowTextW.argtypes = [wt.HWND, wt.LPWSTR, ctypes.c_int]
U32.GetClientRect.argtypes = [wt.HWND, ctypes.POINTER(wt.RECT)]
U32.PostMessageW.argtypes = [wt.HWND, wt.UINT, wt.WPARAM, wt.LPARAM]
U32.MapVirtualKeyW.argtypes = [wt.UINT, wt.UINT]
U32.GetForegroundWindow.restype = wt.HWND
U32.AttachThreadInput.argtypes = [wt.DWORD, wt.DWORD, wt.BOOL]
U32.SetForegroundWindow.argtypes = [wt.HWND]
U32.BringWindowToTop.argtypes = [wt.HWND]
U32.ShowWindow.argtypes = [wt.HWND, ctypes.c_int]
U32.PrintWindow.argtypes = [wt.HWND, wt.HDC, wt.UINT]
U32.GetDC.restype = wt.HDC; U32.GetDC.argtypes = [wt.HWND]
U32.ReleaseDC.argtypes = [wt.HWND, wt.HDC]
G32.CreateCompatibleDC.restype = wt.HDC; G32.CreateCompatibleDC.argtypes = [wt.HDC]
G32.CreateCompatibleBitmap.restype = wt.HBITMAP; G32.CreateCompatibleBitmap.argtypes = [wt.HDC, ctypes.c_int, ctypes.c_int]
G32.SelectObject.restype = wt.HGDIOBJ; G32.SelectObject.argtypes = [wt.HDC, wt.HGDIOBJ]
G32.DeleteObject.argtypes = [wt.HGDIOBJ]; G32.DeleteDC.argtypes = [wt.HDC]
G32.GetDIBits.argtypes = [wt.HDC, wt.HBITMAP, wt.UINT, wt.UINT, ctypes.c_void_p, ctypes.c_void_p, wt.UINT]
K32.GetCurrentThreadId.restype = wt.DWORD
dm.k32.WriteProcessMemory.argtypes = [wt.HANDLE, ctypes.c_void_p, ctypes.c_void_p,
                                      ctypes.c_size_t, ctypes.POINTER(ctypes.c_size_t)]
dm.k32.WriteProcessMemory.restype = wt.BOOL

class BIH(ctypes.Structure):
    _fields_ = [("biSize", wt.DWORD), ("biWidth", wt.LONG), ("biHeight", wt.LONG),
                ("biPlanes", wt.WORD), ("biBitCount", wt.WORD), ("biCompression", wt.DWORD),
                ("biSizeImage", wt.DWORD), ("biXPelsPerMeter", wt.LONG), ("biYPelsPerMeter", wt.LONG),
                ("biClrUsed", wt.DWORD), ("biClrImportant", wt.DWORD)]
class BI(ctypes.Structure):
    _fields_ = [("bmiHeader", BIH), ("bmiColors", wt.DWORD * 3)]

def find_window(pid):
    found = []
    @ctypes.WINFUNCTYPE(wt.BOOL, wt.HWND, wt.LPARAM)
    def cb(hwnd, lp):
        p = wt.DWORD(); U32.GetWindowThreadProcessId(hwnd, ctypes.byref(p))
        if p.value == pid:
            b = ctypes.create_unicode_buffer(256); U32.GetWindowTextW(hwnd, b, 256)
            if 'nectaris' in b.value.lower(): found.append(hwnd)
        return True
    U32.EnumWindows(cb, 0)
    return found[0] if found else None

pid = None
for line in subprocess.check_output(['tasklist', '/FI', 'IMAGENAME eq Nec.exe', '/FO', 'CSV', '/NH'],
                                    text=True).splitlines():
    parts = [x.strip('"') for x in line.split('","')]
    if parts and parts[0].lower() == 'nec.exe': pid = int(parts[1])
if not pid: raise SystemExit('Nec.exe not running')
hwnd = find_window(pid)
r = wt.RECT(); U32.GetClientRect(hwnd, ctypes.byref(r)); cw, ch = r.right, r.bottom
h = dm.k32.OpenProcess(0x1F0FFF, False, pid); base = dm.module_base(h)
def u8(va): return dm.read(h, base, va, 1)[0]
def u32(va): return struct.unpack('<I', dm.read(h, base, va, 4))[0]
def wr(va, data):
    got = ctypes.c_size_t()
    if not dm.k32.WriteProcessMemory(h, ctypes.c_void_p(base + (va - 0x400000)),
                                     data, len(data), ctypes.byref(got)):
        raise OSError('write failed %#x' % va)

def focus():
    fg = U32.GetForegroundWindow()
    ft = U32.GetWindowThreadProcessId(fg, None)
    mt = K32.GetCurrentThreadId()
    U32.AttachThreadInput(mt, ft, True)
    U32.ShowWindow(hwnd, 5); U32.BringWindowToTop(hwnd)
    U32.SetForegroundWindow(hwnd)
    U32.AttachThreadInput(mt, ft, False)
    U32.PostMessageW(hwnd, 0x001C, 1, 0)
    U32.PostMessageW(hwnd, 0x0006, 1, 0)
    U32.PostMessageW(hwnd, 0x0007, 0, 0)

def snap(path):
    hdc = U32.GetDC(hwnd); mdc = G32.CreateCompatibleDC(hdc)
    bmp = G32.CreateCompatibleBitmap(hdc, cw, ch); old = G32.SelectObject(mdc, bmp)
    U32.PrintWindow(hwnd, mdc, 2)
    bi = BI(); bi.bmiHeader.biSize = ctypes.sizeof(BIH); bi.bmiHeader.biWidth = cw
    bi.bmiHeader.biHeight = -ch; bi.bmiHeader.biPlanes = 1; bi.bmiHeader.biBitCount = 32
    buf = ctypes.create_string_buffer(cw * ch * 4)
    G32.GetDIBits(mdc, bmp, 0, ch, buf, ctypes.byref(bi), 0)
    G32.SelectObject(mdc, old); G32.DeleteObject(bmp); G32.DeleteDC(mdc); U32.ReleaseDC(hwnd, hdc)
    Image.frombuffer('RGB', (cw, ch), buf, 'raw', 'BGRX', 0, 1).save(path)

def tap(vk, hold=0.14, gap=0.35):
    focus()
    sc = U32.MapVirtualKeyW(vk, 0) & 0xFF
    key(vk, 1)
    U32.PostMessageW(hwnd, 0x0100, vk, 1 | (sc << 16))
    U32.PostMessageW(hwnd, 0x0102, vk, 1 | (sc << 16))
    time.sleep(hold)
    key(vk, 0)
    U32.PostMessageW(hwnd, 0x0101, vk, 1 | (sc << 16) | 0xC0000000)
    time.sleep(gap)

RIGHT, DOWN, LEFT, UP, SPACE, ESC = 0x27, 0x28, 0x25, 0x26, 0x20, 0x1B

def is_guide():
    return u32(0x483270) == GUIDE_STATE

def close_guide():
    # RIGHT closes the guide back to the map; a second RIGHT closes the menu.
    if is_guide():
        tap(RIGHT, gap=0.35)
    tap(RIGHT, gap=0.25)

def reset_cursor():
    for _ in range(24): tap(LEFT, gap=0.06)
    for _ in range(24): tap(UP, gap=0.06)
    for _ in range(7):
        bx = u8(0x481a72)
        for _ in range(3):
            tap(RIGHT, gap=0.10)
            if u8(0x481a72) > bx: break
    for _ in range(4):
        by = u8(0x481a71)
        for _ in range(3):
            tap(DOWN, gap=0.10)
            if u8(0x481a71) > by: break

# ensure map, cursor at 7,4
close_guide()
reset_cursor()
if (u8(0x481a72), u8(0x481a71)) != (7, 4):
    print('WARNING cursor', u8(0x481a72), u8(0x481a71), flush=True)
cell = u8(0x495820 + 4 * 32 + 7)
slot = cell & 0x3F
print('cell', hex(cell), 'slot', slot, 'orig type', u8(0x4957a0 + slot) & 0x1F, flush=True)
orig = u8(0x4957a0 + slot)

for t in range(24):
    if is_guide():
        close_guide()
    # cursor should still be 7,4
    cx, cy = u8(0x481a72), u8(0x481a71)
    if (cx, cy) != (7, 4):
        reset_cursor()
    wr(0x4957a0 + slot, bytes([(orig & 0xE0) | t]))
    tap(SPACE, gap=0.35)
    tap(DOWN, gap=0.30)
    tap(DOWN, gap=0.30)
    tap(SPACE, gap=0.65)
    ok = is_guide()
    if not ok:
        close_guide()
        reset_cursor()
        wr(0x4957a0 + slot, bytes([(orig & 0xE0) | t]))
        tap(SPACE, gap=0.35)
        tap(DOWN, gap=0.30)
        tap(DOWN, gap=0.30)
        tap(SPACE, gap=0.65)
        ok = is_guide()
    snap(os.path.join(OUT, 'type_%02d.png' % t))
    print('type %2d guide=%s state=%08x' % (t, ok, u32(0x483270)), flush=True)

# restore and close
wr(0x4957a0 + slot, bytes([orig]))
close_guide()
dm.k32.CloseHandle(h)
print('done; left running', flush=True)
