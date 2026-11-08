"""Launch the port, press N/B, and test RMB panning; capture the window."""
import ctypes, ctypes.wintypes as wt, os, sys, time, subprocess
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
EXE = os.path.join(ROOT, 'port', 'build', 'nectaris.exe')
CWD = os.path.join(ROOT, 'port', 'build')
OUT = os.path.join(ROOT, 'dump', 'port_test')
os.makedirs(OUT, exist_ok=True)

U32 = ctypes.WinDLL('user32', use_last_error=True)
G32 = ctypes.WinDLL('gdi32', use_last_error=True)
U32.EnumWindows.argtypes = [ctypes.WINFUNCTYPE(wt.BOOL, wt.HWND, wt.LPARAM), wt.LPARAM]
U32.GetWindowThreadProcessId.argtypes = [wt.HWND, ctypes.POINTER(wt.DWORD)]
U32.GetWindowTextW.argtypes = [wt.HWND, wt.LPWSTR, ctypes.c_int]
U32.GetClientRect.argtypes = [wt.HWND, ctypes.POINTER(wt.RECT)]
U32.PostMessageW.argtypes = [wt.HWND, wt.UINT, wt.WPARAM, wt.LPARAM]
U32.GetForegroundWindow.restype = wt.HWND
U32.SetForegroundWindow.argtypes = [wt.HWND]
U32.SetCursorPos.argtypes = [ctypes.c_int, ctypes.c_int]
U32.ClientToScreen.argtypes = [wt.HWND, ctypes.POINTER(wt.POINT)]
U32.PrintWindow.argtypes = [wt.HWND, wt.HDC, wt.UINT]
U32.GetDC.restype = wt.HDC; U32.GetDC.argtypes = [wt.HWND]
U32.ReleaseDC.argtypes = [wt.HWND, wt.HDC]
G32.CreateCompatibleDC.restype = wt.HDC; G32.CreateCompatibleDC.argtypes = [wt.HDC]
G32.CreateCompatibleBitmap.restype = wt.HBITMAP; G32.CreateCompatibleBitmap.argtypes = [wt.HDC, ctypes.c_int, ctypes.c_int]
G32.SelectObject.restype = wt.HGDIOBJ; G32.SelectObject.argtypes = [wt.HDC, wt.HGDIOBJ]
G32.DeleteObject.argtypes = [wt.HGDIOBJ]; G32.DeleteDC.argtypes = [wt.HDC]
G32.GetDIBits.argtypes = [wt.HDC, wt.HBITMAP, wt.UINT, wt.UINT, ctypes.c_void_p, ctypes.c_void_p, wt.UINT]

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

proc = subprocess.Popen([EXE, 'assets', 'assets/maps/all_units_test.json'], cwd=CWD)
time.sleep(2.5)
hwnd = find_window(proc.pid)
print('hwnd', hwnd)
r = wt.RECT(); U32.GetClientRect(hwnd, ctypes.byref(r)); cw, ch = r.right, r.bottom
print('client', cw, ch)

def snap(name):
    hdc = U32.GetDC(hwnd); mdc = G32.CreateCompatibleDC(hdc)
    bmp = G32.CreateCompatibleBitmap(hdc, cw, ch); old = G32.SelectObject(mdc, bmp)
    U32.PrintWindow(hwnd, mdc, 2)
    bi = BI(); bi.bmiHeader.biSize = ctypes.sizeof(BIH); bi.bmiHeader.biWidth = cw
    bi.bmiHeader.biHeight = -ch; bi.bmiHeader.biPlanes = 1; bi.bmiHeader.biBitCount = 32
    buf = ctypes.create_string_buffer(cw * ch * 4)
    G32.GetDIBits(mdc, bmp, 0, ch, buf, ctypes.byref(bi), 0)
    G32.SelectObject(mdc, old); G32.DeleteObject(bmp); G32.DeleteDC(mdc); U32.ReleaseDC(hwnd, hdc)
    Image.frombuffer('RGB', (cw, ch), buf, 'raw', 'BGRX', 0, 1).save(os.path.join(OUT, name))
    print('snap', name)

def key(vk, name):
    U32.SetForegroundWindow(hwnd)
    sc = U32.MapVirtualKeyW(vk, 0) & 0xFF
    U32.PostMessageW(hwnd, 0x0100, vk, 1 | (sc << 16))
    U32.PostMessageW(hwnd, 0x0102, vk, 1 | (sc << 16))
    time.sleep(0.05)
    U32.PostMessageW(hwnd, 0x0101, vk, 1 | (sc << 16) | 0xC0000000)
    time.sleep(0.5)

snap('01_test.png')
key(0x4E, 'N')
snap('02_after_n.png')
key(0x42, 'B')
snap('03_after_b.png')
# RMB drag across the map
pt = wt.POINT(100, 200); U32.ClientToScreen(hwnd, ctypes.byref(pt))
U32.SetForegroundWindow(hwnd)
U32.SetCursorPos(pt.x, pt.y)
U32.mouse_event(0x0008, 0, 0, 0, 0)   # right down
for i in range(10):
    U32.SetCursorPos(pt.x + i * 8, pt.y + i * 4)
    time.sleep(0.03)
U32.mouse_event(0x0010, 0, 0, 0, 0)   # right up
time.sleep(0.4)
snap('04_after_pan.png')
proc.terminate()
print('done')
