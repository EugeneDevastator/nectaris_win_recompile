"""Boot to the map, open WEAPON DATA, dump the live banks and test closing."""
import ctypes, ctypes.wintypes as wt, os, sys, time, struct, subprocess
sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
GAME = os.path.join(ROOT, 'game')
OUT = os.path.join(ROOT, 'dump', 'guide')
os.makedirs(OUT, exist_ok=True)

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

for line in subprocess.check_output(['tasklist', '/FI', 'IMAGENAME eq Nec.exe', '/FO', 'CSV', '/NH'],
                                    text=True).splitlines():
    parts = [x.strip('"') for x in line.split('","')]
    if parts and parts[0].lower() == 'nec.exe':
        subprocess.call(['taskkill', '/F', '/PID', parts[1]], stdout=subprocess.DEVNULL)

proc = subprocess.Popen([os.path.join(GAME, 'Nec.exe')], cwd=GAME)
hwnd = None
for _ in range(200):
    hwnd = find_window(proc.pid)
    if hwnd: break
    time.sleep(0.05)
r = wt.RECT(); U32.GetClientRect(hwnd, ctypes.byref(r)); cw, ch = r.right, r.bottom
h = dm.k32.OpenProcess(0x1F0FFF, False, proc.pid); base = dm.module_base(h)
def u8(va): return dm.read(h, base, va, 1)[0]
def u32(va): return struct.unpack('<I', dm.read(h, base, va, 4))[0]

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

RIGHT, DOWN, LEFT, UP, SPACE, ENTER = 0x27, 0x28, 0x25, 0x26, 0x20, 0x0D

# boot to map
focus()
t0 = time.time(); toggle = False
while time.time() - t0 < 50:
    if u8(0x495c02) == 15 and u8(0x495c03) == 10: break
    tap(SPACE if not toggle else ENTER, gap=0.45)
    toggle = not toggle
print('map', u8(0x495c02), u8(0x495c03), flush=True)
time.sleep(3.0)
for _ in range(22): tap(LEFT, gap=0.08)
for _ in range(22): tap(UP, gap=0.08)
for _ in range(7):
    bx = u8(0x481a72)
    for _ in range(3):
        tap(RIGHT, gap=0.14)
        if u8(0x481a72) > bx: break
for _ in range(4):
    by = u8(0x481a71)
    for _ in range(3):
        tap(DOWN, gap=0.14)
        if u8(0x481a71) > by: break
print('cursor', u8(0x481a72), u8(0x481a71), flush=True)
# open guide: select, down, down, space
tap(SPACE, gap=0.35); tap(DOWN, gap=0.30); tap(DOWN, gap=0.30); tap(SPACE, gap=0.8)
print('guide state', hex(u32(0x483270)), flush=True)
snap(os.path.join(OUT, 'bank_guide.png'))

# dump the live banks
sp = u32(0x480f14)
tp = u32(0x481500)
fb = u32(0x480f18)
print('sprite ptr', hex(sp), 'tile ptr', hex(tp), 'fb ptr', hex(fb), flush=True)
if 0x10000 < sp < 0x7fffffff:
    open(os.path.join(OUT, 'spritebank_guide.bin'), 'wb').write(dm.read_abs(h, sp, 0x100000))
if 0x10000 < tp < 0x7fffffff:
    open(os.path.join(OUT, 'tilebank_guide.bin'), 'wb').write(dm.read_abs(h, tp, 0x80000))
if 0x10000 < fb < 0x7fffffff:
    open(os.path.join(OUT, 'framebuffer_guide.bin'), 'wb').write(dm.read_abs(h, fb, 704 * 544))
open(os.path.join(OUT, 'palette_guide.bin'), 'wb').write(dm.read(h, base, 0x480f20, 1028))
open(os.path.join(OUT, 'atlas_guide.bin'), 'wb').write(dm.read(h, base, 0x484730, 0x12000))
print('banks dumped', flush=True)

# test safe closing: Right, Right, Right
for i in range(1, 4):
    tap(RIGHT, gap=0.5)
    snap(os.path.join(OUT, 'close_right_%d.png' % i))
    try:
        print('after right', i, 'state', hex(u32(0x483270)),
              'cursor', u8(0x481a72), u8(0x481a71), flush=True)
    except Exception as e:
        print('read failed', e, flush=True)
        break
print('done; left running', flush=True)
