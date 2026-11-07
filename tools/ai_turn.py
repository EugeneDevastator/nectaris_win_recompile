"""Calibrate fake cursor -> game logical coords, then click çµ‚äº† and capture the AI turn."""
import ctypes, ctypes.wintypes as wt, os, sys, time, struct, subprocess
sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
OUT = os.path.join(ROOT, 'ref', 'ai')
os.makedirs(OUT, exist_ok=True)

K32 = ctypes.WinDLL('kernel32', use_last_error=True)
K32.CreateFileMappingW.restype = wt.HANDLE
K32.CreateFileMappingW.argtypes = [wt.HANDLE, ctypes.c_void_p, wt.DWORD, wt.DWORD, wt.DWORD, wt.LPCWSTR]
K32.MapViewOfFile.restype = ctypes.c_void_p
K32.MapViewOfFile.argtypes = [wt.HANDLE, wt.DWORD, wt.DWORD, wt.DWORD, ctypes.c_size_t]
hmap = K32.CreateFileMappingW(wt.HANDLE(-1).value, None, 4, 0, 4096, "Local\\NecFakeInput")
ptr = K32.MapViewOfFile(hmap, 0xF001F, 0, 0, 4096)

def set_i32(off, v): ctypes.c_long.from_address(ptr + off).value = v
def get_i32(off): return ctypes.c_long.from_address(ptr + off).value
def set_key(vk, d): ctypes.c_ubyte.from_address(ptr + 36 + vk).value = 1 if d else 0
set_i32(4, 1)
for i in range(256): set_key(i, 0)
set_i32(28, 0)

U32 = ctypes.WinDLL('user32', use_last_error=True)
G32 = ctypes.WinDLL('gdi32', use_last_error=True)
U32.EnumWindows.argtypes = [ctypes.WINFUNCTYPE(wt.BOOL, wt.HWND, wt.LPARAM), wt.LPARAM]
U32.GetWindowThreadProcessId.argtypes = [wt.HWND, ctypes.POINTER(wt.DWORD)]
U32.GetWindowTextW.argtypes = [wt.HWND, wt.LPWSTR, ctypes.c_int]
U32.ClientToScreen.argtypes = [wt.HWND, ctypes.POINTER(wt.POINT)]
U32.GetClientRect.argtypes = [wt.HWND, ctypes.POINTER(wt.RECT)]
U32.PostMessageW.argtypes = [wt.HWND, wt.UINT, wt.WPARAM, wt.LPARAM]
U32.GetForegroundWindow.restype = wt.HWND
U32.GetWindowThreadProcessId.argtypes = [wt.HWND, ctypes.POINTER(wt.DWORD)]
K32.GetCurrentThreadId.restype = wt.DWORD
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
print('hwnd', hwnd)

# force foreground
fg = U32.GetForegroundWindow()
ft = U32.GetWindowThreadProcessId(fg, None)
mt = K32.GetCurrentThreadId()
U32.AttachThreadInput(mt, ft, True)
U32.ShowWindow(hwnd, 5); U32.BringWindowToTop(hwnd)
print('setfg', U32.SetForegroundWindow(hwnd))
U32.AttachThreadInput(mt, ft, False)
time.sleep(0.4)
print('fg now', U32.GetForegroundWindow(), 'target', hwnd)
U32.PostMessageW(hwnd, 0x001C, 1, 0); U32.PostMessageW(hwnd, 0x0006, 1, 0)
U32.PostMessageW(hwnd, 0x0007, 0, 0)

r = wt.RECT(); U32.GetClientRect(hwnd, ctypes.byref(r))
cw, ch = r.right - r.left, r.bottom - r.top
print('client', cw, ch)

h = dm.k32.OpenProcess(0x1F0FFF, False, pid)
base = dm.module_base(h)
def u8(va): return dm.read(h, base, va, 1)[0]
def u32(va): return struct.unpack('<I', dm.read(h, base, va, 4))[0]
def wx(va, data):
    got = ctypes.c_size_t()
    return dm.k32.WriteProcessMemory(h, ctypes.c_void_p(base + (va - 0x400000)), data, len(data), ctypes.byref(got))

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
    buf = ctypes.create_string_buffer(cw * ch * 4)
    G32.GetDIBits(mdc, bmp, 0, ch, buf, ctypes.byref(bi), 0)
    G32.SelectObject(mdc, old); G32.DeleteObject(bmp); G32.DeleteDC(mdc); U32.ReleaseDC(hwnd, hdc)
    Image.frombuffer('RGB', (cw, ch), buf, 'raw', 'BGRX', 0, 1).save(path)

def set_cursor(cx, cy):
    org = wt.POINT(0, 0); U32.ClientToScreen(hwnd, ctypes.byref(org))
    set_i32(12, org.x + cx); set_i32(16, org.y + cy)
    set_i32(20, cx); set_i32(24, cy)
    lp = ((cy & 0xFFFF) << 16) | (cx & 0xFFFF)
    U32.PostMessageW(hwnd, 0x0200, 0, lp)

def click(cx, cy):
    set_cursor(cx, cy); time.sleep(0.12)
    set_i32(28, 1); U32.PostMessageW(hwnd, 0x0201, 1, ((cy & 0xFFFF) << 16) | (cx & 0xFFFF))
    time.sleep(0.12)
    set_i32(28, 0); U32.PostMessageW(hwnd, 0x0202, 0, ((cy & 0xFFFF) << 16) | (cx & 0xFFFF))
    time.sleep(0.3)

# calibration: cursor global is in logical coords
print('--- calibration ---')
for cx, cy in [(200, 200), (1000, 400), (1150, 600)]:
    set_cursor(cx, cy); time.sleep(0.25)
    print('client', (cx, cy), '-> cursor', (u8(0x480ef8), u8(0x480efc)))
print('--- state ---')
print('atk', u8(0x48327b), 'def', u8(0x483274), 'turn?', u8(0x495c45))

# assume logical = client / 2; çµ‚äº† at logical (585, 225)
scale = 2.0
tx, ty = int(585 * scale), int(225 * scale)
print('clicking çµ‚äº† at client', (tx, ty))
snap(os.path.join(OUT, 'before.png'))
click(tx, ty)
time.sleep(0.5)
snap(os.path.join(OUT, 'after_endturn.png'))
print('after: atk', u8(0x48327b), 'def', u8(0x483274))

# capture the AI turn
for i in range(120):
    snap(os.path.join(OUT, f'ai_{i:03d}.png'))
    if i % 10 == 0:
        print('capture', i, 'atk', u8(0x48327b), 'def', u8(0x483274),
              'ta', u8(0x481db3), 'td', u8(0x481db4))
    time.sleep(0.2)
dm.k32.CloseHandle(h)
print('done')

