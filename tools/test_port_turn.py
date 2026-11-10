"""Drive the actual port through one turn and verify the AI turn completes.

Scenario:
  1. launch nectaris.exe with a mission
  2. wait for the map
  3. send SPACE (end player turn)
  4. wait for "[turn] enemy" then "[turn] player" on stdout
  5. make sure the process is still alive and save a screenshot

Usage:
  python tools/test_port_turn.py [--exe PATH] [--mission NAME]

Defaults to port/build/nectaris.exe and assets/maps/campaign_01_revolt.json.
Exit code 0 = pass, 1 = fail.
"""
import argparse
import ctypes
import ctypes.wintypes as wt
import os
import subprocess
import sys
import threading
import time

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
user32 = ctypes.WinDLL('user32', use_last_error=True)
kernel32 = ctypes.WinDLL('kernel32', use_last_error=True)

user32.EnumWindows.argtypes = [ctypes.WINFUNCTYPE(wt.BOOL, wt.HWND, wt.LPARAM), wt.LPARAM]
user32.GetWindowThreadProcessId.argtypes = [wt.HWND, ctypes.POINTER(wt.DWORD)]
user32.GetWindowTextW.argtypes = [wt.HWND, wt.LPWSTR, ctypes.c_int]
user32.GetWindowRect.argtypes = [wt.HWND, ctypes.POINTER(wt.RECT)]
user32.SetForegroundWindow.argtypes = [wt.HWND]
user32.ShowWindow.argtypes = [wt.HWND, ctypes.c_int]
user32.PostMessageW.argtypes = [wt.HWND, wt.UINT, wt.WPARAM, wt.LPARAM]
user32.MapVirtualKeyW.argtypes = [wt.UINT, wt.UINT]
user32.SendInput.argtypes = [wt.UINT, ctypes.c_void_p, ctypes.c_int]
user32.SendInput.restype = wt.UINT
user32.GetWindowDC.restype = wt.HANDLE
user32.GetWindowDC.argtypes = [wt.HWND]
user32.ReleaseDC.argtypes = [wt.HWND, wt.HANDLE]
user32.PrintWindow.argtypes = [wt.HWND, wt.HANDLE, wt.UINT]
gdi32 = ctypes.WinDLL('gdi32', use_last_error=True)
gdi32.CreateCompatibleDC.restype = wt.HANDLE
gdi32.CreateCompatibleDC.argtypes = [wt.HANDLE]
gdi32.CreateCompatibleBitmap.restype = wt.HANDLE
gdi32.CreateCompatibleBitmap.argtypes = [wt.HANDLE, ctypes.c_int, ctypes.c_int]
gdi32.SelectObject.restype = wt.HANDLE
gdi32.SelectObject.argtypes = [wt.HANDLE, wt.HANDLE]
gdi32.GetDIBits.argtypes = [wt.HANDLE, wt.HANDLE, wt.UINT, wt.UINT,
                            ctypes.c_void_p, ctypes.c_void_p, wt.UINT]
gdi32.DeleteObject.argtypes = [wt.HANDLE]
gdi32.DeleteDC.argtypes = [wt.HANDLE]

SW_RESTORE = 9
VK_SPACE = 0x20
WM_KEYDOWN = 0x0100
WM_KEYUP = 0x0101


def find_window(pid):
    found = []

    @ctypes.WINFUNCTYPE(wt.BOOL, wt.HWND, wt.LPARAM)
    def cb(hwnd, _lp):
        p = wt.DWORD()
        user32.GetWindowThreadProcessId(hwnd, ctypes.byref(p))
        if p.value == pid:
            buf = ctypes.create_unicode_buffer(256)
            user32.GetWindowTextW(hwnd, buf, 256)
            if 'nectaris' in buf.value.lower():
                found.append(hwnd)
        return True

    user32.EnumWindows(cb, 0)
    return found[0] if found else None


def focus_window(hwnd):
    user32.ShowWindow(hwnd, SW_RESTORE)
    user32.SetForegroundWindow(hwnd)
    time.sleep(0.25)


def tap_space(hwnd, use_sendinput=True):
    focus_window(hwnd)
    if use_sendinput:
        # SendInput is a bit more reliable than PostMessage for GLFW while the
        # window is focused.
        class KEYBDINPUT(ctypes.Structure):
            _fields_ = [('wVk', wt.WORD), ('wScan', wt.WORD), ('dwFlags', wt.DWORD),
                        ('time', wt.DWORD), ('dwExtraInfo', ctypes.c_size_t)]

        class INPUT(ctypes.Structure):
            class _I(ctypes.Union):
                _fields_ = [('ki', KEYBDINPUT)]
            _anonymous_ = ('i',)
            _fields_ = [('type', wt.DWORD), ('i', _I)]

        def ev(flags):
            inp = INPUT()
            inp.type = 1
            inp.ki.wVk = VK_SPACE
            inp.ki.wScan = user32.MapVirtualKeyW(VK_SPACE, 0) & 0xFF
            inp.ki.dwFlags = flags
            return inp

        arr = (INPUT * 2)(ev(0), ev(2))
        user32.SendInput(2, arr, ctypes.sizeof(INPUT))
    # PostMessage fallback / duplicate while the AI turn is already active.
    sc = user32.MapVirtualKeyW(VK_SPACE, 0) & 0xFF
    user32.PostMessageW(hwnd, WM_KEYDOWN, VK_SPACE, 1 | (sc << 16))
    time.sleep(0.06)
    user32.PostMessageW(hwnd, WM_KEYUP, VK_SPACE, 1 | (sc << 16) | 0xC0000000)
    time.sleep(0.20)


def capture_window(hwnd, path):
    rect = wt.RECT()
    user32.GetWindowRect(hwnd, ctypes.byref(rect))
    w, h = rect.right - rect.left, rect.bottom - rect.top
    if w <= 0 or h <= 0:
        return False
    hdc = user32.GetWindowDC(hwnd)
    mfc = gdi32.CreateCompatibleDC(hdc)
    bmp = gdi32.CreateCompatibleBitmap(hdc, w, h)
    gdi32.SelectObject(mfc, bmp)
    # PW_RENDERFULLCONTENT = 2
    user32.PrintWindow(hwnd, mfc, 2)

    class BITMAPINFOHEADER(ctypes.Structure):
        _fields_ = [('biSize', wt.DWORD), ('biWidth', wt.LONG), ('biHeight', wt.LONG),
                    ('biPlanes', wt.WORD), ('biBitCount', wt.WORD),
                    ('biCompression', wt.DWORD), ('biSizeImage', wt.DWORD),
                    ('biXPelsPerMeter', wt.LONG), ('biYPelsPerMeter', wt.LONG),
                    ('biClrUsed', wt.DWORD), ('biClrImportant', wt.DWORD)]

    bi = BITMAPINFOHEADER()
    bi.biSize = ctypes.sizeof(BITMAPINFOHEADER)
    bi.biWidth = w
    bi.biHeight = -h
    bi.biPlanes = 1
    bi.biBitCount = 32
    buf = ctypes.create_string_buffer(w * h * 4)
    got = gdi32.GetDIBits(mfc, bmp, 0, h, buf, ctypes.byref(bi), 0)
    ok = got != 0
    if ok:
        from PIL import Image
        Image.frombuffer('RGBA', (w, h), bytes(buf), 'raw', 'BGRA', 0, 1).convert('RGB').save(path)
    gdi32.DeleteObject(bmp)
    gdi32.DeleteDC(mfc)
    user32.ReleaseDC(hwnd, hdc)
    return ok


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--exe', default=os.path.join(ROOT, 'port', 'build', 'nectaris.exe'))
    ap.add_argument('--assets', default=os.path.join(ROOT, 'port', 'build', 'assets'))
    ap.add_argument('--mission', default='campaign_01_revolt.json')
    ap.add_argument('--timeout', type=float, default=40.0)
    ap.add_argument('--shot', default=os.path.join(ROOT, 'port', 'build', 'turn_test.png'))
    args = ap.parse_args()

    mission = args.mission
    if not os.path.isabs(mission):
        mission = os.path.join(args.assets, 'maps', mission)
    if not os.path.exists(mission):
        print('FAIL: mission not found:', mission)
        return 1

    print('launch:', args.exe)
    print('mission:', mission)
    proc = subprocess.Popen([args.exe, args.assets, mission],
                            stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                            text=True, bufsize=1)
    lines = []
    lock = threading.Lock()

    def reader():
        for line in proc.stdout:
            with lock:
                lines.append(line.rstrip())
            print('  |', line.rstrip())

    threading.Thread(target=reader, daemon=True).start()

    def wait_for(substr, timeout):
        end = time.time() + timeout
        while time.time() < end:
            with lock:
                for i, ln in enumerate(lines):
                    if substr in ln:
                        return i, ln
            if proc.poll() is not None:
                return None, None
            time.sleep(0.05)
        return None, None

    if wait_for('loaded ', 20)[1] is None:
        print('FAIL: the port did not load the mission')
        proc.terminate()
        return 1

    hwnd = None
    for _ in range(80):
        hwnd = find_window(proc.pid)
        if hwnd:
            break
        time.sleep(0.1)
    if not hwnd:
        print('FAIL: game window not found')
        proc.terminate()
        return 1

    time.sleep(0.8)
    tap_space(hwnd)

    enemy_i, _ = wait_for('[turn] enemy', 8)
    if enemy_i is None:
        tap_space(hwnd)
        enemy_i, _ = wait_for('[turn] enemy', 8)
    if enemy_i is None:
        print('FAIL: SPACE did not start the enemy turn')
        capture_window(hwnd, args.shot)
        proc.terminate()
        return 1

    player_i, _ = wait_for('[turn] player', args.timeout)
    if player_i is None:
        print('FAIL: the AI turn did not hand control back to the player')
        capture_window(hwnd, args.shot)
        proc.terminate()
        return 1

    with lock:
        ai_lines = [ln for ln in lines if ln.startswith('[ai]')]
        print('AI unit actions:', len(ai_lines))
        for ln in ai_lines:
            print('  ', ln)

    alive = proc.poll() is None
    capture_window(hwnd, args.shot)
    proc.terminate()
    try:
        proc.wait(timeout=5)
    except subprocess.TimeoutExpired:
        proc.kill()

    if not alive:
        print('FAIL: the port exited during the test')
        return 1
    if not ai_lines:
        print('FAIL: the AI turn ran but no unit moved')
        return 1
    print('PASS: player turn -> animated AI turn -> player turn')
    print('screenshot:', args.shot)
    return 0


if __name__ == '__main__':
    sys.exit(main())
