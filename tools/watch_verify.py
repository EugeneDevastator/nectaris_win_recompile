"""Robust BP verification: attach, pump fake input, hash framebuffer, detach always."""
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

K32 = ctypes.WinDLL('kernel32', use_last_error=True)
K32.CreateFileMappingW.restype = wt.HANDLE
K32.CreateFileMappingW.argtypes = [wt.HANDLE, ctypes.c_void_p, wt.DWORD, wt.DWORD, wt.DWORD, wt.LPCWSTR]
K32.MapViewOfFile.restype = ctypes.c_void_p
K32.MapViewOfFile.argtypes = [wt.HANDLE, wt.DWORD, wt.DWORD, wt.DWORD, ctypes.c_size_t]
hmap = K32.CreateFileMappingW(wt.HANDLE(-1).value, None, 4, 0, 4096, "Local\\NecFakeInput")
ptr = K32.MapViewOfFile(hmap, 0xF001F, 0, 0, 4096)
ctypes.memset(ptr, 0, 4096)
ctypes.c_long.from_address(ptr + 0).value = 0x4E454331
ctypes.c_long.from_address(ptr + 4).value = 1
seq = [0]


def wake(hwnd):
    seq[0] += 1
    ctypes.c_long.from_address(ptr + 8).value = seq[0]
    U32.PostMessageW(hwnd, 0x0200, 0, 0)
    U32.PostMessageW(hwnd, 0x0000, 0, 0)


pid = find_pid()
h = dm.k32.OpenProcess(0x1F0FFF, False, pid)
base = dm.module_base(h)
fb = struct.unpack('<I', dm.read(h, base, 0x480f18, 4))[0]
blit = base + 0x11c50
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
print('hwnd', hwnd, 'fb', hex(fb), 'blit', hex(blit), flush=True)


def fbhash():
    try:
        return hashlib.md5(dm.read_abs(h, fb, 704 * 544)).hexdigest()[:8]
    except OSError:
        return 'ERR'


print('pre', fbhash(), flush=True)
for _ in range(20):
    wake(hwnd)
    time.sleep(0.05)
print('wake', fbhash(), flush=True)


def set_exec(hthread, addrs):
    ctx = WOW64_CONTEXT()
    ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS
    if not k32.Wow64GetThreadContext(hthread, ctypes.byref(ctx)):
        return
    ctx.Dr0, ctx.Dr1, ctx.Dr2, ctx.Dr3 = (list(addrs) + [0, 0, 0, 0])[:4]
    ctx.Dr7 = 0
    for i in range(len(addrs)):
        ctx.Dr7 |= (1 << (i * 2))
    ctx.Dr6 = 0
    k32.Wow64SetThreadContext(hthread, ctypes.byref(ctx))


def pump():
    while True:
        wake(hwnd)
        time.sleep(0.15)


attach = k32.DebugActiveProcess(pid)
print('attach', attach, flush=True)
k32.DebugSetProcessKillOnExit(False)
threading.Thread(target=pump, daemon=True).start()
hits = 0
last = ''
try:
    t0 = time.time()
    while time.time() - t0 < 10.0 and hits < 5:
        ev = DEBUG_EVENT()
        if not k32.WaitForDebugEvent(ctypes.byref(ev), 200):
            hh = fbhash()
            if hh != last:
                print('fb', hh, flush=True)
                last = hh
            continue
        code = ev.dwDebugEventCode
        if code == CREATE_PROCESS_DEBUG_EVENT:
            print('create_process', flush=True)
            set_exec(ev.u.CreateProcessInfo.hThread, [blit])
        elif code == CREATE_THREAD_DEBUG_EVENT:
            set_exec(ev.u.CreateThread.hThread, [blit])
        elif code == EXCEPTION_DEBUG_EVENT:
            rec = ev.u.Exception.ExceptionRecord
            if rec.ExceptionCode == EXCEPTION_SINGLE_STEP:
                hits += 1
                print('HIT', hits, hex(rec.ExceptionAddress or 0), flush=True)
        k32.ContinueDebugEvent(ev.dwProcessId, ev.dwThreadId, 0x00010002)
finally:
    try:
        k32.DebugActiveProcessStop(pid)
    except Exception:
        pass
print('hits', hits, 'fb', fbhash(), flush=True)
