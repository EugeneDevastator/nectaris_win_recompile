"""Fresh boot -> skip intro to the tactical map -> send the user's exact sequence
-> capture the whole battle from a fresh launch."""
import ctypes, ctypes.wintypes as wt, os, sys, time, struct, subprocess
sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
GAME = os.path.join(ROOT, 'game')
OUT = os.path.join(ROOT, 'ref', 'battle_clean')
os.makedirs(OUT, exist_ok=True)

K32 = ctypes.WinDLL('kernel32', use_last_error=True)
K32.CreateFileMappingW.restype = wt.HANDLE
K32.CreateFileMappingW.argtypes = [wt.HANDLE, ctypes.c_void_p, wt.DWORD, wt.DWORD, wt.DWORD, wt.LPCWSTR]
K32.MapViewOfFile.restype = ctypes.c_void_p
K32.MapViewOfFile.argtypes = [wt.HANDLE, wt.DWORD, wt.DWORD, wt.DWORD, ctypes.c_size_t]
hmap = K32.CreateFileMappingW(wt.HANDLE(-1).value, None, 4, 0, 4096, "Local\\NecFakeInput")
ptr = K32.MapViewOfFile(hmap, 0xF001F, 0, 0, 4096)
ctypes.memset(ptr, 0, 4096)
def i32(off, v=None):
    if v is None: return ctypes.c_long.from_address(ptr+off).value
    ctypes.c_long.from_address(ptr+off).value = v
def key(vk, d): ctypes.c_ubyte.from_address(ptr+36+vk).value = 1 if d else 0
i32(0, 0x4E454331); i32(4, 1); i32(12, 0); i32(16, 0); i32(28, 0)
for i in range(256): key(i, 0)

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


class BIH(ctypes.Structure):
    _fields_ = [("biSize", wt.DWORD), ("biWidth", wt.LONG), ("biHeight", wt.LONG),
                ("biPlanes", wt.WORD), ("biBitCount", wt.WORD), ("biCompression", wt.DWORD),
                ("biSizeImage", wt.DWORD), ("biXPelsPerMeter", wt.LONG), ("biYPelsPerMeter", wt.LONG),
                ("biClrUsed", wt.DWORD), ("biClrImportant", wt.DWORD)]
class BI(ctypes.Structure):
    _fields_ = [("bmiHeader", BIH), ("bmiColors", wt.DWORD * 3)]


def main():
    p = subprocess.Popen([os.path.join(GAME, 'Nec.exe')], cwd=GAME)
    hwnd = None
    for _ in range(200):
        hwnd = find_window(p.pid)
        if hwnd: break
        time.sleep(0.05)
    print('hwnd', hwnd, flush=True)
    time.sleep(0.6)
    fg = U32.GetForegroundWindow()
    ft = U32.GetWindowThreadProcessId(fg, None)
    mt = K32.GetCurrentThreadId()
    U32.AttachThreadInput(mt, ft, True)
    U32.ShowWindow(hwnd, 5); U32.BringWindowToTop(hwnd)
    print('setfg', U32.SetForegroundWindow(hwnd), flush=True)
    U32.AttachThreadInput(mt, ft, False)
    U32.PostMessageW(hwnd, 0x001C, 1, 0); U32.PostMessageW(hwnd, 0x0006, 1, 0)
    U32.PostMessageW(hwnd, 0x0007, 0, 0)

    r = wt.RECT(); U32.GetClientRect(hwnd, ctypes.byref(r))
    cw, ch = r.right, r.bottom
    print('client', cw, ch, flush=True)

    h = dm.k32.OpenProcess(0x1F0FFF, False, p.pid)
    base = dm.module_base(h)
    def u8(va): return dm.read(h, base, va, 1)[0]

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

    def tap(vk, hold=0.14, gap=0.25):
        sc = U32.MapVirtualKeyW(vk, 0) & 0xFF
        key(vk, 1)
        U32.PostMessageW(hwnd, 0x0100, vk, 1 | (sc << 16))
        U32.PostMessageW(hwnd, 0x0102, vk, 1 | (sc << 16))
        time.sleep(hold)
        key(vk, 0)
        U32.PostMessageW(hwnd, 0x0101, vk, 1 | (sc << 16) | 0xC0000000)
        time.sleep(gap)

    # skip intro until the tactical map is up
    print('skipping intro...', flush=True)
    t0 = time.time()
    while time.time() - t0 < 40:
        if u8(0x495c02) == 15 and u8(0x495c03) == 10:
            break
        tap(0x20, gap=0.35)
    print('map reached after %.1fs, dims %d,%d' % (time.time()-t0, u8(0x495c02), u8(0x495c03)), flush=True)
    time.sleep(1.2)

    snap(os.path.join(OUT, 'map_start.png'))

    RIGHT, DOWN, SPACE = 0x27, 0x28, 0x20
    seq = [RIGHT]*7 + [DOWN]*4 + [SPACE, SPACE, RIGHT, RIGHT, SPACE, SPACE, RIGHT, DOWN, SPACE]
    print('sending sequence', flush=True)
    idx = 0
    for vk in seq:
        tap(vk, gap=0.30)
        snap(os.path.join(OUT, f'step_{idx:02d}.png')); idx += 1
        print('  step', idx, 'atk', u8(0x48327b), 'def', u8(0x483274),
              'ta', u8(0x481db3), 'td', u8(0x481db4), flush=True)

    print('capturing battle...', flush=True)
    for k in range(70):
        snap(os.path.join(OUT, f'f_{k:03d}.png'))
        if k % 10 == 0:
            print('  f', k, 'atk', u8(0x48327b), 'def', u8(0x483274),
                  'ta', u8(0x481db3), 'td', u8(0x481db4), 'ind', u8(0x481dbc), flush=True)
        time.sleep(0.09)
    dm.k32.CloseHandle(h)
    print('done', flush=True)


if __name__ == '__main__':
    main()
