"""Verify hardware breakpoints: readback Dr registers and test an exec BP."""
import ctypes, ctypes.wintypes as wt, os, sys, time, struct
sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm
from watch_spritegen import (WOW64_CONTEXT, DEBUG_EVENT, CONTEXT_DEBUG_REGISTERS, CONTEXT_i386,
                             EXCEPTION_DEBUG_EVENT, EXCEPTION_SINGLE_STEP, CREATE_PROCESS_DEBUG_EVENT,
                             CREATE_THREAD_DEBUG_EVENT, k32, find_pid)

pid = find_pid()
h = dm.k32.OpenProcess(0x1F0FFF, False, pid)
base = dm.module_base(h)
fb = struct.unpack('<I', dm.read(h, base, 0x480f18, 4))[0]
blit = base + 0x11c50          # FUN_00411c50, executes for every sprite blit
print('pid', pid, 'blit', hex(blit), flush=True)


def set_exec(hthread, addr):
    ctx = WOW64_CONTEXT()
    ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS
    if not k32.Wow64GetThreadContext(hthread, ctypes.byref(ctx)):
        return 'getfail'
    ctx.Dr0 = addr
    ctx.Dr1 = ctx.Dr2 = ctx.Dr3 = 0
    ctx.Dr7 = 1          # local enable 0, exec, len 1
    ctx.Dr6 = 0
    ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS
    ok = k32.Wow64SetThreadContext(hthread, ctypes.byref(ctx))
    ctx2 = WOW64_CONTEXT()
    ctx2.ContextFlags = CONTEXT_DEBUG_REGISTERS
    k32.Wow64GetThreadContext(hthread, ctypes.byref(ctx2))
    return 'set=%s dr0=%x dr7=%x' % (ok, ctx2.Dr0, ctx2.Dr7)


if not k32.DebugActiveProcess(pid):
    raise SystemExit('attach failed %s' % ctypes.get_last_error())
k32.DebugSetProcessKillOnExit(False)
hits = 0
t0 = time.time()
while time.time() - t0 < 10.0 and hits < 5:
    ev = DEBUG_EVENT()
    if not k32.WaitForDebugEvent(ctypes.byref(ev), 500):
        continue
    code = ev.dwDebugEventCode
    if code == CREATE_PROCESS_DEBUG_EVENT:
        print('create_process', set_exec(ev.u.CreateProcessInfo.hThread, blit), flush=True)
    elif code == CREATE_THREAD_DEBUG_EVENT:
        print('create_thread', set_exec(ev.u.CreateThread.hThread, blit), flush=True)
    elif code == EXCEPTION_DEBUG_EVENT:
        rec = ev.u.Exception.ExceptionRecord
        if rec.ExceptionCode == EXCEPTION_SINGLE_STEP:
            hits += 1
            print('EXEC HIT', hits, 'addr', hex(rec.ExceptionAddress or 0), flush=True)
        else:
            print('exception', hex(rec.ExceptionCode), flush=True)
    k32.ContinueDebugEvent(ev.dwProcessId, ev.dwThreadId, 0x00010002)
print('hits', hits)
k32.DebugActiveProcessStop(pid)
