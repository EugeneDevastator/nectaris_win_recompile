import ctypes, ctypes.wintypes as wt, subprocess, time, os, sys
from ctypes import byref, POINTER

U32 = ctypes.WinDLL('user32', use_last_error=True)
G32 = ctypes.WinDLL('gdi32', use_last_error=True)

class RECT(ctypes.Structure):
    _fields_ = [("left", wt.LONG), ("top", wt.LONG), ("right", wt.LONG), ("bottom", wt.LONG)]

ENUMPROC = ctypes.WINFUNCTYPE(wt.BOOL, wt.HWND, wt.LPARAM)
U32.EnumWindows.argtypes = [ENUMPROC, wt.LPARAM]
U32.EnumChildWindows.argtypes = [wt.HWND, ENUMPROC, wt.LPARAM]
U32.GetWindowThreadProcessId.argtypes = [wt.HWND, POINTER(wt.DWORD)]
U32.GetWindowRect.argtypes = [wt.HWND, POINTER(RECT)]
U32.GetClientRect.argtypes = [wt.HWND, POINTER(RECT)]
U32.PrintWindow.argtypes = [wt.HWND, wt.HDC, wt.UINT]
U32.GetDC.restype = wt.HDC
U32.GetDC.argtypes = [wt.HWND]
U32.ReleaseDC.argtypes = [wt.HWND, wt.HDC]
G32.CreateCompatibleDC.restype = wt.HDC
G32.CreateCompatibleDC.argtypes = [wt.HDC]
G32.CreateCompatibleBitmap.restype = wt.HBITMAP
G32.CreateCompatibleBitmap.argtypes = [wt.HDC, ctypes.c_int, ctypes.c_int]
G32.SelectObject.restype = wt.HGDIOBJ
G32.SelectObject.argtypes = [wt.HDC, wt.HGDIOBJ]
G32.DeleteObject.argtypes = [wt.HGDIOBJ]
G32.DeleteDC.argtypes = [wt.HDC]
G32.GetDIBits.argtypes = [wt.HDC, wt.HBITMAP, wt.UINT, wt.UINT, ctypes.c_void_p, ctypes.c_void_p, wt.UINT]

def wtext(h):
    b = ctypes.create_unicode_buffer(512); U32.GetWindowTextW(h, b, 512); return b.value
def wclass(h):
    b = ctypes.create_unicode_buffer(512); U32.GetClassNameW(h, b, 512); return b.value

def windows_of(pid):
    tops = []
    @ENUMPROC
    def cb(h, lp):
        p = wt.DWORD(); U32.GetWindowThreadProcessId(h, byref(p))
        if p.value == pid:
            r = RECT(); U32.GetWindowRect(h, byref(r))
            tops.append((h, wtext(h), wclass(h), (r.left, r.top, r.right, r.bottom)))
        return True
    U32.EnumWindows(cb, 0)
    return tops

def children(h):
    out = []
    @ENUMPROC
    def cb(c, lp):
        r = RECT(); U32.GetWindowRect(c, byref(r))
        out.append((c, wtext(c), wclass(c), (r.left, r.top, r.right, r.bottom)))
        return True
    U32.EnumChildWindows(h, cb, 0)
    return out

def snap(h, path):
    from PIL import Image
    r = RECT(); U32.GetClientRect(h, byref(r))
    w, hh = r.right - r.left, r.bottom - r.top
    if w <= 0 or hh <= 0: return None
    hdc = U32.GetDC(h); mdc = G32.CreateCompatibleDC(hdc)
    bmp = G32.CreateCompatibleBitmap(hdc, w, hh); old = G32.SelectObject(mdc, bmp)
    U32.PrintWindow(h, mdc, 2)
    class BIH(ctypes.Structure):
        _fields_ = [("biSize", wt.DWORD), ("biWidth", wt.LONG), ("biHeight", wt.LONG),
                    ("biPlanes", wt.WORD), ("biBitCount", wt.WORD), ("biCompression", wt.DWORD),
                    ("biSizeImage", wt.DWORD), ("biXPelsPerMeter", wt.LONG), ("biYPelsPerMeter", wt.LONG),
                    ("biClrUsed", wt.DWORD), ("biClrImportant", wt.DWORD)]
    class BI(ctypes.Structure):
        _fields_ = [("bmiHeader", BIH), ("bmiColors", wt.DWORD * 3)]
    bi = BI(); bi.bmiHeader.biSize = ctypes.sizeof(BIH)
    bi.bmiHeader.biWidth = w; bi.bmiHeader.biHeight = -hh
    bi.bmiHeader.biPlanes = 1; bi.bmiHeader.biBitCount = 32
    buf = ctypes.create_string_buffer(w * hh * 4)
    G32.GetDIBits(mdc, bmp, 0, hh, buf, byref(bi), 0)
    G32.SelectObject(mdc, old); G32.DeleteObject(bmp); G32.DeleteDC(mdc); U32.ReleaseDC(h, hdc)
    Image.frombuffer('RGB', (w, hh), buf, 'raw', 'BGRX', 0, 1).save(path)
    return (w, hh)

if __name__ == '__main__':
    exe = sys.argv[1]
    out = sys.argv[2]
    si = subprocess.STARTUPINFO()
    si.dwFlags |= subprocess.STARTF_USESHOWWINDOW
    si.wShowWindow = 4  # SW_SHOWNOACTIVATE
    p = subprocess.Popen([exe], cwd=os.path.dirname(exe), startupinfo=si)
    time.sleep(3)
    tops = windows_of(p.pid)
    for h, t, c, r in tops:
        print(f"TOP   {h} class={c!r} title={t!r} rect={r} size={r[2]-r[0]}x{r[3]-r[1]}")
    # children of the first visible top-level
    for h, t, c, r in tops:
        kids = children(h)
        if kids:
            print(f"--- children of {h} ({c}) ---")
            for ch, ct, cc, cr in kids:
                print(f"  CHILD {ch} class={cc!r} text={ct!r} rect={cr} size={cr[2]-cr[0]}x{cr[3]-cr[1]}")
    # snapshot biggest top-level
    best = max(tops, key=lambda x: max(0, x[3][2]-x[3][0]) * max(0, x[3][3]-x[3][1]), default=None)
    if best:
        print("SNAP", best[0], snap(best[0], out))
    time.sleep(0.5)
    p.terminate()
