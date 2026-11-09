"""Page-guard watchdog for the generated combat spritebank block.

Guards [spritebank+0x20000, spritebank+0x60000) with PAGE_GUARD, attaches as a
debugger, and logs the faulting EIP + registers on the first write. Single-steps
over the write and re-arms the guard.

Usage: load mission 6, then run this; it skips the turn to trigger combat.
"""
import ctypes, ctypes.wintypes as wt, os, sys, time, struct, subprocess, json, threading
sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm
from watch_spritegen import (WOW64_CONTEXT, DEBUG_EVENT, CONTEXT_i386, CONTEXT_DEBUG_REGISTERS,
                             EXCEPTION_DEBUG_EVENT, CREATE_PROCESS_DEBUG_EVENT,
                             CREATE_THREAD_DEBUG_EVENT, k32, find_pid, read_abs)

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
OUT = os.path.join(ROOT, 'dump', 'watchdog')
os.makedirs(OUT, exist_ok=True)

U32 = ctypes.WinDLL('user32', use_last_error=True)
U32.EnumWindows.argtypes = [ctypes.WINFUNCTYPE(wt.BOOL, wt.HWND, wt.LPARAM), wt.LPARAM]
U32.GetWindowThreadProcessId.argtypes = [wt.HWND, ctypes.POINTER(wt.DWORD)]
U32.GetWindowTextW.argtypes = [wt.HWND, wt.LPWSTR, ctypes.c_int]
U32.PostMessageW.argtypes = [wt.HWND, wt.UINT, wt.WPARAM, wt.LPARAM]
U32.MapVirtualKeyW.argtypes = [wt.UINT, wt.UINT]

K32 = ctypes.WinDLL('kernel32', use_last_error=True)
K32.CreateFileMappingW.restype = wt.HANDLE
K32.CreateFileMappingW.argtypes = [wt.HANDLE, ctypes.c_void_p, wt.DWORD, wt.DWORD, wt.DWORD, wt.LPCWSTR]
K32.MapViewOfFile.restype = ctypes.c_void_p
K32.MapViewOfFile.argtypes = [wt.HANDLE, wt.DWORD, wt.DWORD, wt.DWORD, ctypes.c_size_t]
K32.VirtualProtectEx.restype = wt.BOOL
K32.VirtualProtectEx.argtypes = [wt.HANDLE, ctypes.c_void_p, ctypes.c_size_t, wt.DWORD, ctypes.POINTER(wt.DWORD)]

hmap = K32.CreateFileMappingW(wt.HANDLE(-1).value, None, 4, 0, 4096, "Local\\NecFakeInput")
ptr = K32.MapViewOfFile(hmap, 0xF001F, 0, 0, 4096)
ctypes.memset(ptr, 0, 4096)
ctypes.c_long.from_address(ptr + 0).value = 0x4E454331
ctypes.c_long.from_address(ptr + 4).value = 1
seq = [0]

PAGE_READWRITE = 0x04
PAGE_READONLY = 0x02
PAGE_GUARD = 0x100
EXCEPTION_GUARD_PAGE = 0x80000001
EXCEPTION_ACCESS_VIOLATION = 0xC0000005
EXCEPTION_SINGLE_STEP = 0x80000004
CONTEXT_CONTROL = 0x00000001
CONTEXT_INTEGER = 0x00000002
DBG_CONTINUE = 0x00010002
DBG_EXCEPTION_NOT_HANDLED = 0x80010001


def wake(hwnd):
    seq[0] += 1
    ctypes.c_long.from_address(ptr + 8).value = seq[0]
    U32.PostMessageW(hwnd, 0x0200, 0, 0)
    U32.PostMessageW(hwnd, 0x0000, 0, 0)


pid = find_pid()
if pid is None:
    raise SystemExit('Nec.exe is not running')
h = dm.k32.OpenProcess(0x1F0FFF, False, pid)
base = dm.module_base(h)
sb = struct.unpack('<I', dm.read(h, base, 0x480f14, 4))[0]
region = sb + 0x20000
rlen = 0x8000
print('pid', pid, 'spritebank', hex(sb), 'guard', hex(region), hex(rlen), flush=True)

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


def arm():
    old = wt.DWORD()
    ok = K32.VirtualProtectEx(h, ctypes.c_void_p(region), rlen,
                              PAGE_READONLY, ctypes.byref(old))
    return ok


if not k32.DebugActiveProcess(pid):
    raise SystemExit('attach failed %s' % ctypes.get_last_error())
k32.DebugSetProcessKillOnExit(False)
print('attached', flush=True)

# skip turn once the game is running; advance combat dialogs with space
def input_thread():
    time.sleep(4.0)
    # focus / activate without stealing real input
    U32.PostMessageW(hwnd, 0x001C, 1, 0)
    U32.PostMessageW(hwnd, 0x0006, 1, 0)
    U32.PostMessageW(hwnd, 0x0007, 0, 0)
    MAP_STATE = 0x416700
    last_state = None
    map_since = None

    def tap(vk, hold=0.12, gap=0.16):
        sc = U32.MapVirtualKeyW(vk, 0) & 0xFF
        ctypes.c_ubyte.from_address(ptr + 36 + vk).value = 1
        U32.PostMessageW(hwnd, 0x0100, vk, 1 | (sc << 16))
        U32.PostMessageW(hwnd, 0x0102, vk, 1 | (sc << 16))
        time.sleep(hold)
        ctypes.c_ubyte.from_address(ptr + 36 + vk).value = 0
        U32.PostMessageW(hwnd, 0x0101, vk, 1 | (sc << 16) | 0xC0000000)
        time.sleep(gap)

    for _ in range(400):
        wake(hwnd)
        try:
            state = struct.unpack('<I', dm.read(h, base, 0x483270, 4))[0]
        except OSError:
            state = None
        if state != last_state:
            print('STATE %s' % (hex(state) if state is not None else '?'), flush=True)
            last_state = state
        now = time.time()
        if state == MAP_STATE:
            if map_since is None:
                map_since = now
            if now - map_since > 2.0:
                tap(0x28); tap(0x28); tap(0x28); tap(0x20, gap=0.30)
                map_since = now
        else:
            map_since = None
            tap(0x20, gap=0.10)
        time.sleep(0.6)


threading.Thread(target=input_thread, daemon=True).start()
hits = 0
stepping = False
t0 = time.time()
log = open(os.path.join(OUT, 'guard.log'), 'w', encoding='utf-8')


def logline(s):
    print(s, flush=True)
    log.write(s + '\n')
    log.flush()


armed = False
try:
    while time.time() - t0 < 300.0 and hits < 400:
        ev = DEBUG_EVENT()
        if not k32.WaitForDebugEvent(ctypes.byref(ev), 250):
            if not armed:
                armed = arm()
                logline('armed %s' % armed)
            continue
        code = ev.dwDebugEventCode
        action = DBG_CONTINUE
        if code in (CREATE_PROCESS_DEBUG_EVENT, CREATE_THREAD_DEBUG_EVENT):
            action = DBG_CONTINUE
        elif code == EXCEPTION_DEBUG_EVENT:
            rec = ev.u.Exception.ExceptionRecord
            ec = rec.ExceptionCode
            if ec == EXCEPTION_ACCESS_VIOLATION:
                info = rec.ExceptionInformation
                addr = int(info[1]) if rec.NumberParameters >= 2 else 0
                if int(info[0]) == 1:  # write
                    ctx = WOW64_CONTEXT()
                    ctx.ContextFlags = CONTEXT_i386 | CONTEXT_CONTROL | CONTEXT_INTEGER
                    ht = k32.OpenThread(0x1F03FF, False, ev.dwThreadId)
                    if ht:
                        k32.Wow64GetThreadContext(ht, ctypes.byref(ctx))
                        k32.CloseHandle(ht)
                    regs = {'eip': ctx.Eip, 'eax': ctx.Eax, 'ebx': ctx.Ebx, 'ecx': ctx.Ecx,
                            'edx': ctx.Edx, 'esi': ctx.Esi, 'edi': ctx.Edi, 'ebp': ctx.Ebp,
                            'esp': ctx.Esp, 'addr': addr}
                    logline('WRITE ' + json.dumps(regs))
                    open(os.path.join(OUT, 'guard_hit_%03d_code.bin' % hits), 'wb').write(
                        read_abs(h, ctx.Eip - 32, 80))
                    for name in ('esi', 'edi', 'edx', 'ecx', 'eax', 'ebx', 'esp'):
                        buf = read_abs(h, regs[name], 320)
                        if buf:
                            open(os.path.join(OUT, 'guard_hit_%03d_mem_%s_%08x.bin' %
                                              (hits, name, regs[name])), 'wb').write(buf)
                    hits += 1
                    # single-step over the faulting instruction, guard temporarily off
                    old = wt.DWORD()
                    K32.VirtualProtectEx(h, ctypes.c_void_p(region), rlen, PAGE_READWRITE,
                                         ctypes.byref(old))
                    ctx.EFlags |= 0x100
                    ctx.ContextFlags = CONTEXT_i386 | CONTEXT_CONTROL | CONTEXT_INTEGER
                    ht = k32.OpenThread(0x1F03FF, False, ev.dwThreadId)
                    if ht:
                        k32.Wow64SetThreadContext(ht, ctypes.byref(ctx))
                        k32.CloseHandle(ht)
                    stepping = True
                    action = DBG_CONTINUE
                else:
                    # read access: just drop guard and re-arm later
                    old = wt.DWORD()
                    K32.VirtualProtectEx(h, ctypes.c_void_p(region), rlen, PAGE_READWRITE,
                                         ctypes.byref(old))
                    armed = False
                    action = DBG_CONTINUE
            elif ec == EXCEPTION_SINGLE_STEP and stepping:
                stepping = False
                armed = arm()
                action = DBG_CONTINUE
            elif ec in (0x80000003,):
                action = DBG_CONTINUE
            else:
                action = DBG_EXCEPTION_NOT_HANDLED
        k32.ContinueDebugEvent(ev.dwProcessId, ev.dwThreadId, action)
finally:
    try:
        old = wt.DWORD()
        K32.VirtualProtectEx(h, ctypes.c_void_p(region), rlen, PAGE_READWRITE, ctypes.byref(old))
    except Exception:
        pass
    try:
        k32.DebugActiveProcessStop(pid)
    except Exception:
        pass
logline('hits %d' % hits)
log.close()
print('done hits', hits, flush=True)
