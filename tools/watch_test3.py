"""Exec-BP test with input pumping so the game actually renders."""
import ctypes, ctypes.wintypes as wt, os, sys, time, struct, threading
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

pid = find_pid()
h = dm.k32.OpenProcess(0x1F0FFF, False, pid)
base = dm.module_base(h)
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
print('hwnd', hwnd, 'blit', hex(blit), flush=True)


def set_exec(hthread, addr):
    ctx = WOW64_CONTEXT()
    ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS
    k32.Wow64GetThreadContext(hthread, ctypes.byref(ctx))
    ctx.Dr0 = addr
    ctx.Dr1 = ctx.Dr2 = ctx.Dr3 = 0
    ctx.Dr7 = 1
    ctx.Dr6 = 0
    k32.Wow64SetThreadContext(hthread, ctypes.byref(ctx))


def pump():
    for _ in range(120):
        if hwnd:
            U32.PostMessageW(hwnd, 0x0200, 0, 0)
            U32.PostMessageW(hwnd, 0x0000, 0, 0)
        time.sleep(0.1)


if not k32.DebugActiveProcess(pid):
    raise SystemExit('attach failed')
k32.DebugSetProcessKillOnExit(False)
print('attached', flush=True)
threading.Thread(target=pump, daemon=True).start()
hits = 0
t0 = time.time()
while time.time() - t0 < 12.0 and hits < 6:
    ev = DEBUG_EVENT()
    if not k32.WaitForDebugEvent(ctypes.byref(ev), 500):
        continue
    code = ev.dwDebugEventCode
    if code == CREATE_PROCESS_DEBUG_EVENT:
        set_exec(ev.u.CreateProcessInfo.hThread, blit)
    elif code == CREATE_THREAD_DEBUG_EVENT:
        set_exec(ev.u.CreateThread.hThread, blit)
    elif code == EXCEPTION_DEBUG_EVENT:
        rec = ev.u.Exception.ExceptionRecord
        if rec.ExceptionCode == EXCEPTION_SINGLE_STEP:
            hits += 1
            print('EXEC HIT', hits, 'addr', hex(rec.ExceptionAddress or 0), flush=True)
    k32.ContinueDebugEvent(ev.dwProcessId, ev.dwThreadId, 0x00010002)
print('hits', hits)
k32.DebugActiveProcessStop(pid)
