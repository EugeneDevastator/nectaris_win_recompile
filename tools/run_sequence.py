"""Attach to the running game and send a keyboard sequence, capturing frames."""
import ctypes, ctypes.wintypes as wt, os, sys, time, struct, subprocess
sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
OUT = os.path.join(ROOT, 'ref', 'seq')
os.makedirs(OUT, exist_ok=True)

K32 = ctypes.WinDLL('kernel32', use_last_error=True)
K32.CreateFileMappingW.restype = wt.HANDLE
K32.CreateFileMappingW.argtypes = [wt.HANDLE, ctypes.c_void_p, wt.DWORD, wt.DWORD, wt.DWORD, wt.LPCWSTR]
K32.MapViewOfFile.restype = ctypes.c_void_p
K32.MapViewOfFile.argtypes = [wt.HANDLE, wt.DWORD, wt.DWORD, wt.DWORD, ctypes.c_size_t]
hmap = K32.CreateFileMappingW(wt.HANDLE(-1).value, None, 4, 0, 4096, "Local\\NecFakeInput")
ptr = K32.MapViewOfFile(hmap, 0xF001F, 0, 0, 4096)
def i32(off, v=None):
    if v is None: return ctypes.c_long.from_address(ptr+off).value
    ctypes.c_long.from_address(ptr+off).value = v
def key(vk, d): ctypes.c_ubyte.from_address(ptr+36+vk).value = 1 if d else 0
i32(4, 1); i32(28, 0); i32(12, 0); i32(16, 0)
for i in range(256): key(i, 0)

U32 = ctypes.WinDLL('user32', use_last_error=True)
G32 = ctypes.WinDLL('gdi32', use_last_error=True)
U32.EnumWindows.argtypes = [ctypes.WINFUNCTYPE(wt.BOOL, wt.HWND, wt.LPARAM), wt.LPARAM]
U32.GetWindowThreadProcessId.argtypes = [wt.HWND, ctypes.POINTER(wt.DWORD)]
U32.GetWindowTextW.argtypes = [wt.HWND, wt.LPWSTR, ctypes.c_int]
U32.GetClientRect.argtypes = [wt.HWND, ctypes.POINTER(wt.RECT)]
U32.PostMessageW.argtypes = [wt.HWND, wt.UINT, wt.WPARAM, wt.LPARAM]
U32.MapVirtualKeyW.argtypes = [wt.UINT, wt.UINT]
U32.PrintWindow.argtypes = [wt.HWND, wt.HDC, wt.UINT]
U32.GetDC.restype = wt.HDC; U32.GetDC.argtypes = [wt.HWND]
U32.ReleaseDC.argtypes = [wt.HWND, wt.HDC]
G32.CreateCompatibleDC.restype = wt.HDC; G32.CreateCompatibleDC.argtypes = [wt.HDC]
G32.CreateCompatibleBitmap.restype = wt.HBITMAP; G32.CreateCompatibleBitmap.argtypes = [wt.HDC, ctypes.c_int, ctypes.c_int]
G32.SelectObject.restype = wt.HGDIOBJ; G32.SelectObject.argtypes = [wt.HDC, wt.HGDIOBJ]
G32.DeleteObject.argtypes = [wt.HGDIOBJ]; G32.DeleteDC.argtypes = [wt.HDC]
G32.GetDIBits.argtypes = [wt.HDC, wt.HBITMAP, wt.UINT, wt.UINT, ctypes.c_void_p, ctypes.c_void_p, wt.UINT]

out = subprocess.check_output(['tasklist', '/FI', 'IMAGENAME eq Nec.exe', '/FO', 'CSV', '/NH'], text=True)
pid = None
for line in out.splitlines():
    p = [x.strip('"') for x in line.split('","')]
    if p and p[0].lower() == 'nec.exe': pid = int(p[1])
print('pid', pid)

found = []
@ctypes.WINFUNCTYPE(wt.BOOL, wt.HWND, wt.LPARAM)
def cb(hwnd, lp):
    p = wt.DWORD(); U32.GetWindowThreadProcessId(hwnd, ctypes.byref(p))
    if p.value == pid:
        b = ctypes.create_unicode_buffer(256); U32.GetWindowTextW(hwnd, b, 256)
        if 'nectaris' in b.value.lower(): found.append(hwnd)
    return True
U32.EnumWindows(cb, 0)
hwnd = found[0]
r = wt.RECT(); U32.GetClientRect(hwnd, ctypes.byref(r))
cw, ch = r.right, r.bottom
print('hwnd', hwnd, 'client', cw, ch)

# force the game window to the foreground so it processes input
U32.GetForegroundWindow.restype = wt.HWND
U32.AttachThreadInput.argtypes = [wt.DWORD, wt.DWORD, wt.BOOL]
U32.SetForegroundWindow.argtypes = [wt.HWND]
U32.BringWindowToTop.argtypes = [wt.HWND]
U32.ShowWindow.argtypes = [wt.HWND, ctypes.c_int]
K32.GetCurrentThreadId.restype = wt.DWORD
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
focus()
time.sleep(0.4)
print('foreground now', U32.GetForegroundWindow(), 'target', hwnd)

h = dm.k32.OpenProcess(0x1F0FFF, False, pid)
base = dm.module_base(h)
def u8(va): return dm.read(h, base, va, 1)[0]
def u32(va): return struct.unpack('<I', dm.read(h, base, va, 4))[0]
pal = open(os.path.join(ROOT, 'dump', 'palette_bgr.bin'), 'rb').read()
PALR = bytearray(768)
for i in range(256):
    PALR[i*3]=pal[i*4+2]; PALR[i*3+1]=pal[i*4+1]; PALR[i*3+2]=pal[i*4]

class BIH(ctypes.Structure):
    _fields_ = [("biSize", wt.DWORD), ("biWidth", wt.LONG), ("biHeight", wt.LONG),
                ("biPlanes", wt.WORD), ("biBitCount", wt.WORD), ("biCompression", wt.DWORD),
                ("biSizeImage", wt.DWORD), ("biXPelsPerMeter", wt.LONG), ("biYPelsPerMeter", wt.LONG),
                ("biClrUsed", wt.DWORD), ("biClrImportant", wt.DWORD)]
class BI(ctypes.Structure):
    _fields_ = [("bmiHeader", BIH), ("bmiColors", wt.DWORD * 3)]

def snap(path):
    hdc = U32.GetDC(hwnd); mdc = G32.CreateCompatibleDC(hdc)
    bmp = G32.CreateCompatibleBitmap(hdc, cw, ch); old = G32.SelectObject(mdc, bmp)
    U32.PrintWindow(hwnd, mdc, 2)
    bi = BI(); bi.bmiHeader.biSize = ctypes.sizeof(BIH); bi.bmiHeader.biWidth = cw
    bi.bmiHeader.biHeight = -ch; bi.bmiHeader.biPlanes = 1; bi.bmiHeader.biBitCount = 32
    buf = ctypes.create_string_buffer(cw*ch*4)
    G32.GetDIBits(mdc, bmp, 0, ch, buf, ctypes.byref(bi), 0)
    G32.SelectObject(mdc, old); G32.DeleteObject(bmp); G32.DeleteDC(mdc); U32.ReleaseDC(hwnd, hdc)
    Image.frombuffer('RGB', (cw, ch), buf, 'raw', 'BGRX', 0, 1).save(path)

def fbshot(path):
    sp = u32(0x480f18)
    data = dm.read_abs(h, sp, 704*544)
    img = Image.frombuffer('P', (704,544), data, 'raw','P',0,1)
    img = img.transpose(Image.FLIP_TOP_BOTTOM).crop((32,32,672,512))
    img.putpalette(bytes(PALR))
    img.convert('RGB').save(path)

def tap(vk, hold=0.14, gap=0.28):
    focus()
    sc = U32.MapVirtualKeyW(vk, 0) & 0xFF
    key(vk, 1)
    U32.PostMessageW(hwnd, 0x0100, vk, 1 | (sc << 16))
    U32.PostMessageW(hwnd, 0x0102, vk, 1 | (sc << 16))
    time.sleep(hold)
    key(vk, 0)
    U32.PostMessageW(hwnd, 0x0101, vk, 1 | (sc << 16) | 0xC0000000)
    time.sleep(gap)

RIGHT, DOWN, LEFT, UP, SPACE = 0x27, 0x28, 0x25, 0x26, 0x20

def cell():
    return u8(0x481a72), u8(0x481a71)

# closed-loop reset to the map origin (arrows clamp at 0)
print('cursor at', cell(), flush=True)
for _ in range(20):
    tap(LEFT, gap=0.10)
for _ in range(20):
    tap(UP, gap=0.10)
print('cursor reset to', cell(), flush=True)

# right x7, verifying each step
for _ in range(7):
    bx = cell()[0]
    for attempt in range(3):
        tap(RIGHT, gap=0.18)
        if cell()[0] > bx:
            break
        print('  right retry', cell(), flush=True)
print('after right x7', cell(), flush=True)

# down x4, verifying each step
for _ in range(4):
    by = cell()[1]
    for attempt in range(3):
        tap(DOWN, gap=0.18)
        if cell()[1] > by:
            break
        print('  down retry', cell(), flush=True)
print('after down x4', cell(), flush=True)

# baseline
snap(os.path.join(OUT, 's_000.png'))
i = 1
rest = [SPACE, SPACE, RIGHT, RIGHT, SPACE, SPACE, RIGHT, DOWN, SPACE]
for vk in rest:
    print('  key 0x%02x  cell=%s atk=%d def=%d' % (vk, cell(), u8(0x48327b),
          u8(0x483274)), flush=True)
    tap(vk)
    snap(os.path.join(OUT, f's_{i:03d}.png')); i += 1

print('--- after sequence ---')
print('atk', u8(0x48327b), 'def', u8(0x483274), 'type', u8(0x48327c),
      'ta', u8(0x481db3), 'td', u8(0x481db4), 'ind', u8(0x481dbc),
      'mode', u8(0x481a68))
# dump the battle sprite/tile banks while combat is up
time.sleep(2.5)
try:
    sp = u32(0x480f14)
    tp = u32(0x481500)
    print('sprite_ptr', hex(sp), 'tile_ptr', hex(tp))
    if 0x10000 < sp < 0x7fffffff:
        open(os.path.join(ROOT, 'dump', 'spritebank_battle.bin'), 'wb').write(
            dm.read_abs(h, sp, 0x100000))
        print('wrote dump/spritebank_battle.bin')
    if 0x10000 < tp < 0x7fffffff:
        open(os.path.join(ROOT, 'dump', 'tilebank_battle.bin'), 'wb').write(
            dm.read_abs(h, tp, 0x80000))
        print('wrote dump/tilebank_battle.bin')
except Exception as e:
    print('bank dump failed', e)
# capture some frames whether or not battle started
for k in range(85):
    snap(os.path.join(OUT, f'after_{k:03d}.png'))
    try:
        fbshot(os.path.join(OUT, f'afterfb_{k:03d}.png'))
    except Exception:
        pass
    time.sleep(0.1)
dm.k32.CloseHandle(h)
print('done')

