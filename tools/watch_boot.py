"""Catch the mission-load write that populates spritebank slots 128..191.

Launches Nec.exe fresh, attaches before the mission loads, guards the block,
then skips the intro and starts a new game so the unit art is loaded.
"""
import ctypes, ctypes.wintypes as wt, os, sys, time, struct, subprocess, json, threading
sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm
from watch_spritegen import (WOW64_CONTEXT, DEBUG_EVENT, CONTEXT_i386, CONTEXT_DEBUG_REGISTERS,
                             EXCEPTION_DEBUG_EVENT, CREATE_PROCESS_DEBUG_EVENT,
                             CREATE_THREAD_DEBUG_EVENT, k32, read_abs)

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
GAME = os.path.join(ROOT, 'game')
OUT = os.path.join(ROOT, 'dump', 'watchdog_boot')
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

PAGE_READONLY = 0x02
PAGE_READWRITE = 0x04
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


def find_window(pid):
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
    return found[0] if found else None


subprocess.run(['taskkill', '/F', '/IM', 'Nec.exe'], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
time.sleep(0.4)
proc = subprocess.Popen([os.path.join(GAME, 'Nec.exe')], cwd=GAME)
hwnd = None
for _ in range(200):
    hwnd = find_window(proc.pid)
    if hwnd:
        break
    time.sleep(0.05)
print('pid', proc.pid, 'hwnd', hwnd, flush=True)
h = dm.k32.OpenProcess(0x1F0FFF, False, proc.pid)
base = dm.module_base(h)

# spritebank is allocated during startup; wait for it
sb = 0
for _ in range(100):
    try:
        sb = struct.unpack('<I', dm.read(h, base, 0x480f14, 4))[0]
    except OSError:
        sb = 0
    if sb:
        break
    time.sleep(0.05)
print('spritebank', hex(sb), flush=True)
region = (sb + 0x20000) & ~0xFFF
rlen = 0x9000   # slots 128..160 pages
print('guard', hex(region), hex(rlen), flush=True)

# patch the new-game mission immediate so START loads mission 6
try:
    paddr = base + (0x403d5d - 0x400000)
    oldp = wt.DWORD()
    K32.VirtualProtectEx(h, ctypes.c_void_p(paddr), 1, 0x40, ctypes.byref(oldp))
    got = ctypes.c_size_t()
    dm.k32.WriteProcessMemory(h, ctypes.c_void_p(paddr), bytes([5]), 1, ctypes.byref(got))
    K32.VirtualProtectEx(h, ctypes.c_void_p(paddr), 1, oldp.value, ctypes.byref(oldp))
    print('patched mission 6 immediate', flush=True)
except Exception as exc:
    print('patch failed', exc, flush=True)

if not k32.DebugActiveProcess(proc.pid):
    raise SystemExit('attach failed %s' % ctypes.get_last_error())
k32.DebugSetProcessKillOnExit(False)
print('attached', flush=True)

# input: skip intro, start new game
def input_thread():
    time.sleep(2.0)
    U32.PostMessageW(hwnd, 0x001C, 1, 0)
    U32.PostMessageW(hwnd, 0x0006, 1, 0)
    U32.PostMessageW(hwnd, 0x0007, 0, 0)

    def tap(vk, hold=0.12, gap=0.25):
        sc = U32.MapVirtualKeyW(vk, 0) & 0xFF
        ctypes.c_ubyte.from_address(ptr + 36 + vk).value = 1
        U32.PostMessageW(hwnd, 0x0100, vk, 1 | (sc << 16))
        U32.PostMessageW(hwnd, 0x0102, vk, 1 | (sc << 16))
        time.sleep(hold)
        ctypes.c_ubyte.from_address(ptr + 36 + vk).value = 0
        U32.PostMessageW(hwnd, 0x0101, vk, 1 | (sc << 16) | 0xC0000000)
        time.sleep(gap)

    # skip intro
    for _ in range(30):
        wake(hwnd)
        tap(0x20, gap=0.5)
    # start new game
    wake(hwnd)
    tap(0x0D, gap=0.8)
    tap(0x20, gap=0.8)
    for _ in range(40):
        wake(hwnd)
        tap(0x20, gap=0.6)


threading.Thread(target=input_thread, daemon=True).start()

log = open(os.path.join(OUT, 'boot.log'), 'w', encoding='utf-8')


def logline(s):
    print(s, flush=True)
    log.write(s + '\n')
    log.flush()


def arm():
    old = wt.DWORD()
    return K32.VirtualProtectEx(h, ctypes.c_void_p(region), rlen, PAGE_READONLY, ctypes.byref(old))


hits = 0
stepping = False
armed = False
t0 = time.time()
try:
    while time.time() - t0 < 120.0 and hits < 100:
        ev = DEBUG_EVENT()
        if not k32.WaitForDebugEvent(ctypes.byref(ev), 200):
            if not armed:
                armed = arm()
                logline('armed %s' % armed)
            continue
        code = ev.dwDebugEventCode
        action = DBG_CONTINUE
        if code == EXCEPTION_DEBUG_EVENT:
            rec = ev.u.Exception.ExceptionRecord
            ec = rec.ExceptionCode
            if ec == EXCEPTION_ACCESS_VIOLATION:
                info = rec.ExceptionInformation
                addr = int(info[1]) if rec.NumberParameters >= 2 else 0
                if int(info[0]) == 1:
                    ctx = WOW64_CONTEXT()
                    ctx.ContextFlags = CONTEXT_i386 | CONTEXT_CONTROL | CONTEXT_INTEGER
                    ht = k32.OpenThread(0x1F03FF, False, ev.dwThreadId)
                    if ht:
                        k32.Wow64GetThreadContext(ht, ctypes.byref(ctx))
                        k32.CloseHandle(ht)
                    regs = {'eip': ctx.Eip, 'eax': ctx.Eax, 'ebx': ctx.Ebx, 'ecx': ctx.Ecx,
                            'edx': ctx.Edx, 'esi': ctx.Esi, 'edi': ctx.Edi, 'ebp': ctx.Ebp,
                            'esp': ctx.Esp, 'addr': addr,
                            'slot': (addr - sb) // 1024}
                    logline('WRITE ' + json.dumps(regs))
                    open(os.path.join(OUT, 'hit_%03d_code.bin' % hits), 'wb').write(
                        read_abs(h, ctx.Eip - 32, 80))
                    open(os.path.join(OUT, 'hit_%03d_stack.bin' % hits), 'wb').write(
                        read_abs(h, ctx.Esp, 512))
                    open(os.path.join(OUT, 'hit_%03d_ecx.bin' % hits), 'wb').write(
                        read_abs(h, ctx.Ecx, 512))
                    hits += 1
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
                else:
                    old = wt.DWORD()
                    K32.VirtualProtectEx(h, ctypes.c_void_p(region), rlen, PAGE_READWRITE,
                                         ctypes.byref(old))
                    armed = False
            elif ec == EXCEPTION_SINGLE_STEP and stepping:
                stepping = False
                armed = arm()
            elif ec == 0x80000003:
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
        k32.DebugActiveProcessStop(proc.pid)
    except Exception:
        pass
logline('hits %d' % hits)
log.close()
print('done', flush=True)
