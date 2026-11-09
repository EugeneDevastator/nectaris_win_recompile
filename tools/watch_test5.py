"""Check rendering while debugged with activation + key pumping."""
import ctypes, ctypes.wintypes as wt, os, sys, time, struct, threading, hashlib
sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm
from watch_spritegen import (WOW64_CONTEXT, DEBUG_EVENT, CONTEXT_DEBUG_REGISTERS,
                             EXCEPTION_DEBUG_EVENT, EXCEPTION_SINGLE_STEP, CREATE_PROCESS_DEBUG_EVENT,
                             CREATE_THREAD_DEBUG_EVENT, k32, find_pid)

U32 = ctypes.WinDLL('user32', use_last_error=True)
U32.EnumWindows.argtypes = [ctypes.WINFUNCTYPE(wt.BOOL, wt.HWND, wt.LPARAM), wt.LPARAM]
U32.GetWindowThreadProcessId.argtypes = [wt.HWND, ctypes.POINTER(wt.DWORD)]
U32.GetWindowTextW.argtypes = [wt.HWND, wt.LPWSTR, ctypes.c_int]
U32.PostMessageW.argtypes = [wt.HWND, wt.UINT, wt.WPARAM, wt.LPARAM]
U32.MapVirtualKeyW.argtypes = [wt.UINT, wt.UINT]

pid = find_pid()
h = dm.k32.OpenProcess(0x1F0FFF, False, pid)
base = dm.module_base(h)
fb = struct.unpack('<I', dm.read(h, base, 0x480f18, 4))[0]
found = []


@ctypes.WINFUNCTYPE(wt.BOOL, wt.HWND, wt.LPARAM)
def cb(hwnd, lp):
    p = wt.DWORD()
    U32.GetWindowThreadProcessId(hwnd, ctypes.byref(p))
    if p.value == pid:
        b = ctypes.create_unicode_buffer(256)
        U32.GetWindowTextW(hwnd, b, 256)
        if 'nectaris' in b.value.lower():
            found.append(hwnd)
    return True


U32.EnumWindows(cb, 0)
hwnd = found[0] if found else None
print('hwnd', hwnd, flush=True)


def pump():
    while True:
        U32.PostMessageW(hwnd, 0x001C, 1, 0)
        U32.PostMessageW(hwnd, 0x0006, 1, 0)
        U32.PostMessageW(hwnd, 0x0007, 0, 0)
        U32.PostMessageW(hwnd, 0x0000, 0, 0)
        sc = U32.MapVirtualKeyW(0x20, 0) & 0xFF
        U32.PostMessageW(hwnd, 0x0100, 0x20, 1 | (sc << 16))
        U32.PostMessageW(hwnd, 0x0101, 0x20, 1 | (sc << 16) | 0xC0000000)
        time.sleep(0.4)


def fbhash():
    return hashlib.md5(dm.read_abs(h, fb, 704 * 544)).hexdigest()[:8]


print('pre', fbhash())
threading.Thread(target=pump, daemon=True).start()
time.sleep(2.0)
print('with pump', fbhash())
if not k32.DebugActiveProcess(pid):
    raise SystemExit('attach failed')
k32.DebugSetProcessKillOnExit(False)
print('attached', flush=True)
t0 = time.time()
last = ''
while time.time() - t0 < 8.0:
    ev = DEBUG_EVENT()
    if not k32.WaitForDebugEvent(ctypes.byref(ev), 250):
        hh = fbhash()
        if hh != last:
            print('fb', hh, flush=True)
            last = hh
        continue
    k32.ContinueDebugEvent(ev.dwProcessId, ev.dwThreadId, 0x00010002)
print('done')
k32.DebugActiveProcessStop(pid)
