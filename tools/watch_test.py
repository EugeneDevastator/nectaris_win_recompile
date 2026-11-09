"""Sanity check: hardware write breakpoint on the framebuffer must fire every frame."""
import ctypes, ctypes.wintypes as wt, os, sys, time, struct, subprocess
sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm
from watch_spritegen import (WOW64_CONTEXT, DEBUG_EVENT, CONTEXT_DEBUG_REGISTERS, CONTEXT_i386,
                             EXCEPTION_DEBUG_EVENT, EXCEPTION_SINGLE_STEP, CREATE_PROCESS_DEBUG_EVENT,
                             CREATE_THREAD_DEBUG_EVENT, set_bps, k32, find_pid, read_abs)

pid = find_pid()
if pid is None:
    raise SystemExit('Nec.exe not running')
h = dm.k32.OpenProcess(0x1F0FFF, False, pid)
base = dm.module_base(h)
fb = struct.unpack('<I', dm.read(h, base, 0x480f18, 4))[0]
print('pid', pid, 'fb', hex(fb), flush=True)
addrs = [fb + 0x10000]
print('bp', hex(addrs[0]))
if not k32.DebugActiveProcess(pid):
    raise SystemExit('attach failed %s' % ctypes.get_last_error())
k32.DebugSetProcessKillOnExit(False)
print('attached', flush=True)
hits = 0
t0 = time.time()
while time.time() - t0 < 6.0 and hits < 5:
    ev = DEBUG_EVENT()
    if not k32.WaitForDebugEvent(ctypes.byref(ev), 500):
        continue
    if ev.dwDebugEventCode == CREATE_PROCESS_DEBUG_EVENT:
        ok = set_bps(ev.u.CreateProcessInfo.hThread, addrs)
        print('set_bps process', ok)
    elif ev.dwDebugEventCode == CREATE_THREAD_DEBUG_EVENT:
        set_bps(ev.u.CreateThread.hThread, addrs)
    elif ev.dwDebugEventCode == EXCEPTION_DEBUG_EVENT:
        rec = ev.u.Exception.ExceptionRecord
        if rec.ExceptionCode == EXCEPTION_SINGLE_STEP:
            ctx = WOW64_CONTEXT()
            ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS | CONTEXT_i386 | 2
            ht = k32.OpenThread(0x1F03FF, False, ev.dwThreadId)
            if ht:
                k32.Wow64GetThreadContext(ht, ctypes.byref(ctx))
                k32.CloseHandle(ht)
            hits += 1
            print('HIT', hits, 'eip', hex(ctx.Eip), 'dr6', hex(ctx.Dr6), flush=True)
        else:
            print('exception', hex(rec.ExceptionCode), flush=True)
    k32.ContinueDebugEvent(ev.dwProcessId, ev.dwThreadId, 0x00010002)
print('hits', hits)
k32.DebugActiveProcessStop(pid)
