"""Guard-watch the mission-1 attack to catch the combat-chunk generator.

Arms PAGE_READONLY on spritebank slots 128..159, performs the reliable blue-unit
attack, and logs EIP/registers/stack on the first write.
"""
import ctypes, ctypes.wintypes as wt, os, sys, time, struct, subprocess, json, threading
sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm
from watch_spritegen import (WOW64_CONTEXT, DEBUG_EVENT, CONTEXT_i386, CONTEXT_DEBUG_REGISTERS,
                             EXCEPTION_DEBUG_EVENT, CREATE_PROCESS_DEBUG_EVENT,
                             CREATE_THREAD_DEBUG_EVENT, k32, find_pid)

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
OUT = os.path.join(ROOT, 'dump', 'watch_attack')
os.makedirs(OUT, exist_ok=True)
TYP = int(sys.argv[1]) if len(sys.argv) > 1 else 18
MAP_STATE = 0x416700

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
time.sleep(0.3)
subprocess.call([sys.executable, os.path.join(ROOT, 'tools', 'orig_play_mission.py'), '1', '--no-preview'],
                stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
time.sleep(0.5)
pid = find_pid()
hwnd = find_window(pid)
h = dm.k32.OpenProcess(0x1F0FFF, False, pid)
base = dm.module_base(h)
sb = 0
for _ in range(100):
    try:
        sb = struct.unpack('<I', dm.read(h, base, 0x480f14, 4))[0]
    except OSError:
        sb = 0
    if sb:
        break
    time.sleep(0.05)
region = (sb + 0x20000) & ~0xFFF
rlen = 0x9000
print('pid', pid, 'spritebank', hex(sb), 'guard', hex(region), flush=True)


def u32(va):
    return struct.unpack('<I', dm.read(h, base, va, 4))[0]


def rd8(va):
    return dm.read(h, base, va, 1)[0]


def wr8(va, v):
    got = ctypes.c_size_t()
    dm.k32.WriteProcessMemory(h, ctypes.c_void_p(base + (va - 0x400000)), bytes([v & 0xFF]), 1, ctypes.byref(got))


def tap(vk, hold=0.12, gap=0.22):
    sc = U32.MapVirtualKeyW(vk, 0) & 0xFF
    ctypes.c_ubyte.from_address(ptr + 36 + vk).value = 1
    U32.PostMessageW(hwnd, 0x0100, vk, 1 | (sc << 16))
    U32.PostMessageW(hwnd, 0x0102, vk, 1 | (sc << 16))
    time.sleep(hold)
    ctypes.c_ubyte.from_address(ptr + 36 + vk).value = 0
    U32.PostMessageW(hwnd, 0x0101, vk, 1 | (sc << 16) | 0xC0000000)
    time.sleep(gap)


def wake():
    seq[0] += 1
    ctypes.c_long.from_address(ptr + 8).value = seq[0]
    U32.PostMessageW(hwnd, 0x0200, 0, 0)
    U32.PostMessageW(hwnd, 0x0000, 0, 0)


if not k32.DebugActiveProcess(pid):
    raise SystemExit('attach failed')
k32.DebugSetProcessKillOnExit(False)
print('attached', flush=True)


def input_thread():
    time.sleep(1.5)
    t0 = time.time()
    while time.time() - t0 < 30:
        wake()
        if u32(0x483270) == MAP_STATE:
            break
        time.sleep(0.1)
    time.sleep(2.0)
    # exit action selection, relocate green slot 5 next to blue slot 2
    tap(0x20, gap=0.6)
    gx, gy = rd8(0x495ab0 + 5) & 31, rd8(0x495af0 + 5) & 31
    wr8(0x495ab0 + 5, 4); wr8(0x495af0 + 5, 7)
    wr8(0x495820 + gy * 32 + gx, 0)
    wr8(0x495820 + 7 * 32 + 4, 0x80 | 5)
    wr8(0x4957a0 + 2, TYP & 31)
    # cursor to (4,8)
    for _ in range(50):
        x, y = rd8(0x481a72), rd8(0x481a71)
        if x == 4 and y == 8:
            break
        if x < 4:
            tap(0x27, gap=0.10)
        elif x > 4:
            tap(0x25, gap=0.10)
        elif y < 8:
            tap(0x28, gap=0.10)
        elif y > 8:
            tap(0x26, gap=0.10)
    # attack: menu, attack, confirm, target up, confirm
    tap(0x20, gap=0.5)
    tap(0x28, gap=0.3)
    tap(0x20, gap=0.5)
    tap(0x26, gap=0.3)
    tap(0x20, gap=0.6)
    print('attack issued', flush=True)


threading.Thread(target=input_thread, daemon=True).start()


def arm():
    old = wt.DWORD()
    return K32.VirtualProtectEx(h, ctypes.c_void_p(region), rlen, PAGE_READONLY, ctypes.byref(old))


hits = 0
stepping = False
armed = False
t0 = time.time()
log = open(os.path.join(OUT, 'watch.log'), 'w', encoding='utf-8')


def logline(s):
    print(s, flush=True)
    log.write(s + '\n')
    log.flush()


try:
    while time.time() - t0 < 120.0 and hits < 40:
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
                            'esp': ctx.Esp, 'addr': addr, 'slot': (addr - sb) // 1024}
                    logline('WRITE ' + json.dumps(regs))
                    open(os.path.join(OUT, 'hit_%03d_code.bin' % hits), 'wb').write(
                        dm.read_abs(h, ctx.Eip - 32, 96))
                    open(os.path.join(OUT, 'hit_%03d_stack.bin' % hits), 'wb').write(
                        dm.read_abs(h, ctx.Esp, 768))
                    hits += 1
                    old = wt.DWORD()
                    K32.VirtualProtectEx(h, ctypes.c_void_p(region), rlen, PAGE_READWRITE, ctypes.byref(old))
                    ctx.EFlags |= 0x100
                    ctx.ContextFlags = CONTEXT_i386 | CONTEXT_CONTROL | CONTEXT_INTEGER
                    ht = k32.OpenThread(0x1F03FF, False, ev.dwThreadId)
                    if ht:
                        k32.Wow64SetThreadContext(ht, ctypes.byref(ctx))
                        k32.CloseHandle(ht)
                    stepping = True
                else:
                    old = wt.DWORD()
                    K32.VirtualProtectEx(h, ctypes.c_void_p(region), rlen, PAGE_READWRITE, ctypes.byref(old))
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
        k32.DebugActiveProcessStop(pid)
    except Exception:
        pass
logline('hits %d' % hits)
log.close()
print('done', flush=True)
