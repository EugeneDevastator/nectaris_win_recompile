"""Run the original Nec.exe with REAL input and record a session.

Creates the proxy's shared section with `enabled = 0`, so the winmm proxy passes
real mouse/keyboard through to the game.  A polling recorder logs cursor
positions, mouse buttons, key transitions and periodic screenshots.

Stop with the Pause/Break key, or by creating ref/session/STOP.
"""
import ctypes, ctypes.wintypes as wt, os, sys, time, json, struct
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
GAME = os.path.join(ROOT, 'game')
OUT = os.path.join(ROOT, 'ref', 'session')
os.makedirs(OUT, exist_ok=True)
STOP = os.path.join(OUT, 'STOP')
if os.path.exists(STOP):
    os.remove(STOP)

# ---- shared memory: keep proxy disabled (magic set so it doesn't wait, enabled 0)
K32 = ctypes.WinDLL('kernel32', use_last_error=True)
K32.CreateFileMappingW.restype = wt.HANDLE
K32.CreateFileMappingW.argtypes = [wt.HANDLE, ctypes.c_void_p, wt.DWORD, wt.DWORD, wt.DWORD, wt.LPCWSTR]
K32.MapViewOfFile.restype = ctypes.c_void_p
K32.MapViewOfFile.argtypes = [wt.HANDLE, wt.DWORD, wt.DWORD, wt.DWORD, ctypes.c_size_t]
INVALID_HANDLE_VALUE = wt.HANDLE(-1).value
hmap = K32.CreateFileMappingW(INVALID_HANDLE_VALUE, None, 0x04, 0, 4096, "Local\\NecFakeInput")
ptr = K32.MapViewOfFile(hmap, 0xF001F, 0, 0, 4096)
ctypes.memset(ptr, 0, 4096)
ctypes.c_long.from_address(ptr).value = 0x4E454331  # magic
ctypes.c_long.from_address(ptr + 4).value = 0        # enabled = 0 -> real input

U32 = ctypes.WinDLL('user32', use_last_error=True)
G32 = ctypes.WinDLL('gdi32', use_last_error=True)
U32.GetCursorPos.argtypes = [ctypes.POINTER(wt.POINT)]
U32.ScreenToClient.argtypes = [wt.HWND, ctypes.POINTER(wt.POINT)]
U32.ClientToScreen.argtypes = [wt.HWND, ctypes.POINTER(wt.POINT)]
U32.GetClientRect.argtypes = [wt.HWND, ctypes.POINTER(wt.RECT)]
U32.GetForegroundWindow.restype = wt.HWND
U32.SetForegroundWindow.argtypes = [wt.HWND]
U32.SetForegroundWindow.restype = wt.BOOL
U32.ShowWindow.argtypes = [wt.HWND, ctypes.c_int]
U32.SetWindowPos.argtypes = [wt.HWND, wt.HWND, ctypes.c_int, ctypes.c_int,
                             ctypes.c_int, ctypes.c_int, ctypes.c_uint]
U32.GetAsyncKeyState.argtypes = [ctypes.c_int]
U32.GetAsyncKeyState.restype = ctypes.c_short
U32.EnumWindows.argtypes = [ctypes.WINFUNCTYPE(wt.BOOL, wt.HWND, wt.LPARAM), wt.LPARAM]
U32.GetWindowThreadProcessId.argtypes = [wt.HWND, ctypes.POINTER(wt.DWORD)]
U32.GetWindowTextW.argtypes = [wt.HWND, wt.LPWSTR, ctypes.c_int]
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
G32.GetDIBits.argtypes = [wt.HDC, wt.HBITMAP, wt.UINT, wt.UINT, ctypes.c_void_p,
                          ctypes.c_void_p, wt.UINT]


def find_window(pid):
    found = []

    @ctypes.WINFUNCTYPE(wt.BOOL, wt.HWND, wt.LPARAM)
    def cb(hwnd, lp):
        p = wt.DWORD()
        U32.GetWindowThreadProcessId(hwnd, ctypes.byref(p))
        if p.value == pid:
            buf = ctypes.create_unicode_buffer(256)
            U32.GetWindowTextW(hwnd, buf, 256)
            if 'nectaris' in buf.value.lower():
                found.append(hwnd)
        return True

    U32.EnumWindows(cb, 0)
    return found[0] if found else None


class BIH(ctypes.Structure):
    _fields_ = [("biSize", wt.DWORD), ("biWidth", wt.LONG), ("biHeight", wt.LONG),
                ("biPlanes", wt.WORD), ("biBitCount", wt.WORD), ("biCompression", wt.DWORD),
                ("biSizeImage", wt.DWORD), ("biXPelsPerMeter", wt.LONG),
                ("biYPelsPerMeter", wt.LONG), ("biClrUsed", wt.DWORD),
                ("biClrImportant", wt.DWORD)]


class BI(ctypes.Structure):
    _fields_ = [("bmiHeader", BIH), ("bmiColors", wt.DWORD * 3)]


def snapshot(hwnd, path):
    r = wt.RECT()
    U32.GetClientRect(hwnd, ctypes.byref(r))
    w, h = r.right - r.left, r.bottom - r.top
    if w <= 0 or h <= 0:
        return None
    hdc = U32.GetDC(hwnd)
    mdc = G32.CreateCompatibleDC(hdc)
    bmp = G32.CreateCompatibleBitmap(hdc, w, h)
    old = G32.SelectObject(mdc, bmp)
    U32.PrintWindow(hwnd, mdc, 2)
    bi = BI()
    bi.bmiHeader.biSize = ctypes.sizeof(BIH)
    bi.bmiHeader.biWidth = w
    bi.bmiHeader.biHeight = -h
    bi.bmiHeader.biPlanes = 1
    bi.bmiHeader.biBitCount = 32
    bi.bmiHeader.biCompression = 0
    buf = ctypes.create_string_buffer(w * h * 4)
    G32.GetDIBits(mdc, bmp, 0, h, buf, ctypes.byref(bi), 0)
    G32.SelectObject(mdc, old)
    G32.DeleteObject(bmp)
    G32.DeleteDC(mdc)
    U32.ReleaseDC(hwnd, hdc)
    img = Image.frombuffer('RGB', (w, h), buf, 'raw', 'BGRX', 0, 1)
    img.save(path)
    return (w, h)


# candidate keys (avoid scanning all 256 every tick)
VKS = list(range(0x08, 0x0E)) + [0x10, 0x11, 0x12, 0x13, 0x14, 0x1B, 0x20,
        0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x2C, 0x2D, 0x2E,
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
        0x5B, 0x5C, 0x5D, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
        0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71]
VKS += list(range(0x41, 0x5B))
VKS += list(range(0x72, 0x88))
VKS = sorted(set(VKS))
MOUSE_VK = {0x01: 'left', 0x02: 'right', 0x04: 'middle'}


def main():
    import subprocess
    p = subprocess.Popen([os.path.join(GAME, 'Nec.exe')], cwd=GAME)
    hwnd = None
    for _ in range(200):
        hwnd = find_window(p.pid)
        if hwnd:
            break
        time.sleep(0.05)
    if not hwnd:
        print('window not found', flush=True)
        p.terminate()
        return
    # bring it to the front without stealing the mouse
    U32.ShowWindow(hwnd, 5)  # SW_SHOW
    U32.SetWindowPos(hwnd, wt.HWND(-1), 40, 40, 0, 0, 0x0001 | 0x0040)  # TOPMOST + NOSIZE
    U32.SetWindowPos(hwnd, wt.HWND(-2), 40, 40, 0, 0, 0x0002 | 0x0040)  # NOTOPMOST
    U32.SetForegroundWindow(hwnd)
    time.sleep(0.8)

    log = open(os.path.join(OUT, 'input.jsonl'), 'w', encoding='utf-8')
    meta = {'hwnd': int(hwnd), 'pid': p.pid, 'client_origin': [0, 0]}
    org = wt.POINT(0, 0)
    U32.ClientToScreen(hwnd, ctypes.byref(org))
    meta['client_origin'] = [org.x, org.y]
    log.write(json.dumps({'meta': meta}) + '\n')
    log.flush()

    prev = {}
    prev_pos = None
    t0 = time.time()
    last_shot = 0.0
    shot_idx = 0
    print('recording; press Pause/Break or create ref/session/STOP to stop', flush=True)

    while True:
        now = time.time()
        if now - t0 > 1800:
            print('timeout', flush=True)
            break
        if os.path.exists(STOP):
            print('stop file', flush=True)
            break
        if (U32.GetAsyncKeyState(0x13) & 0x8000):  # Pause/Break
            print('pause key', flush=True)
            break

        pt = wt.POINT()
        U32.GetCursorPos(ctypes.byref(pt))
        cp = wt.POINT(pt.x, pt.y)
        U32.ScreenToClient(hwnd, ctypes.byref(cp))
        fg = (U32.GetForegroundWindow() == hwnd)

        # mouse buttons
        for vk, name in ((0x01, 'left'), (0x02, 'right'), (0x04, 'middle')):
            down = bool(U32.GetAsyncKeyState(vk) & 0x8000)
            if down != prev.get('m_' + name, False):
                prev['m_' + name] = down
                log.write(json.dumps({'t': round(now - t0, 3), 'ev': 'mdown' if down else 'mup',
                                      'btn': name, 'cx': cp.x, 'cy': cp.y,
                                      'fg': fg}) + '\n')
        # movement
        if prev_pos != (cp.x, cp.y):
            if prev_pos is None or abs(cp.x - prev_pos[0]) + abs(cp.y - prev_pos[1]) >= 2:
                log.write(json.dumps({'t': round(now - t0, 3), 'ev': 'move',
                                      'cx': cp.x, 'cy': cp.y, 'fg': fg}) + '\n')
            prev_pos = (cp.x, cp.y)
        # keys
        for vk in VKS:
            down = bool(U32.GetAsyncKeyState(vk) & 0x8000)
            if down != prev.get(vk, False):
                prev[vk] = down
                log.write(json.dumps({'t': round(now - t0, 3),
                                      'ev': 'kdown' if down else 'kup',
                                      'vk': vk, 'fg': fg}) + '\n')
        log.flush()

        if now - last_shot >= 0.25:
            path = os.path.join(OUT, f'frame_{shot_idx:05d}.png')
            try:
                sz = snapshot(hwnd, path)
                log.write(json.dumps({'t': round(now - t0, 3), 'ev': 'shot',
                                      'idx': shot_idx, 'size': sz}) + '\n')
                log.flush()
                shot_idx += 1
            except Exception as e:
                print('shot err', e, flush=True)
            last_shot = now

        time.sleep(0.03)

    log.write(json.dumps({'t': round(time.time() - t0, 3), 'ev': 'end',
                          'frames': shot_idx}) + '\n')
    log.close()
    print('recording stopped, frames', shot_idx, flush=True)
    # leave the game running for the user to continue if desired


if __name__ == '__main__':
    main()
