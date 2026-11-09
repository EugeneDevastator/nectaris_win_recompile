"""Watchdog: catch writes into the combat-generated spritebank slots.

Attaches as a debugger, sets hardware write breakpoints on spritebank slots
128, 160, 192 and 256, then skips the player turn so the AI starts a combat.
Every write logs EIP, DR6 and register values plus memory around candidate
source pointers, so we can identify where the original sprite data comes from.
"""
import ctypes, ctypes.wintypes as wt, os, sys, time, struct, subprocess, json, threading
sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
OUT = os.path.join(ROOT, 'dump', 'watchdog')
os.makedirs(OUT, exist_ok=True)

k32 = ctypes.WinDLL('kernel32', use_last_error=True)
k32.DebugActiveProcess.argtypes = [wt.DWORD]
k32.DebugActiveProcess.restype = wt.BOOL
k32.DebugActiveProcessStop.argtypes = [wt.DWORD]
k32.DebugActiveProcessStop.restype = wt.BOOL
k32.DebugSetProcessKillOnExit.argtypes = [wt.BOOL]
k32.DebugSetProcessKillOnExit.restype = wt.BOOL
k32.WaitForDebugEvent.argtypes = [ctypes.c_void_p, wt.DWORD]
k32.WaitForDebugEvent.restype = wt.BOOL
k32.ContinueDebugEvent.argtypes = [wt.DWORD, wt.DWORD, wt.DWORD]
k32.ContinueDebugEvent.restype = wt.BOOL
k32.CreateToolhelp32Snapshot.argtypes = [wt.DWORD, wt.DWORD]
k32.CreateToolhelp32Snapshot.restype = wt.HANDLE
k32.Thread32First.argtypes = [wt.HANDLE, ctypes.c_void_p]
k32.Thread32Next.argtypes = [wt.HANDLE, ctypes.c_void_p]
k32.OpenThread.argtypes = [wt.DWORD, wt.BOOL, wt.DWORD]
k32.OpenThread.restype = wt.HANDLE
k32.CloseHandle.argtypes = [wt.HANDLE]
k32.Wow64GetThreadContext.argtypes = [wt.HANDLE, ctypes.c_void_p]
k32.Wow64SetThreadContext.argtypes = [wt.HANDLE, ctypes.c_void_p]
k32.ReadProcessMemory.argtypes = [wt.HANDLE, ctypes.c_void_p, ctypes.c_void_p,
                                  ctypes.c_size_t, ctypes.POINTER(ctypes.c_size_t)]

CONTEXT_i386 = 0x00010000
CONTEXT_DEBUG_REGISTERS = CONTEXT_i386 | 0x00000010
DBG_CONTINUE = 0x00010002
EXCEPTION_DEBUG_EVENT = 1
CREATE_THREAD_DEBUG_EVENT = 2
CREATE_PROCESS_DEBUG_EVENT = 3
EXIT_THREAD_DEBUG_EVENT = 4
EXIT_PROCESS_DEBUG_EVENT = 5
LOAD_DLL_DEBUG_EVENT = 6
UNLOAD_DLL_DEBUG_EVENT = 7
OUTPUT_DEBUG_STRING_EVENT = 8
RIP_EVENT = 9
EXCEPTION_SINGLE_STEP = 0x80000004


class WOW64_CONTEXT(ctypes.Structure):
    _fields_ = [
        ('ContextFlags', wt.DWORD),
        ('Dr0', wt.DWORD), ('Dr1', wt.DWORD), ('Dr2', wt.DWORD), ('Dr3', wt.DWORD),
        ('Dr6', wt.DWORD), ('Dr7', wt.DWORD),
        ('FloatSave', ctypes.c_ubyte * 112),
        ('SegGs', wt.DWORD), ('SegFs', wt.DWORD), ('SegEs', wt.DWORD), ('SegDs', wt.DWORD),
        ('Edi', wt.DWORD), ('Esi', wt.DWORD), ('Ebx', wt.DWORD), ('Edx', wt.DWORD),
        ('Ecx', wt.DWORD), ('Eax', wt.DWORD), ('Ebp', wt.DWORD), ('Eip', wt.DWORD),
        ('SegCs', wt.DWORD), ('EFlags', wt.DWORD), ('Esp', wt.DWORD), ('SegSs', wt.DWORD),
        ('ExtendedRegisters', ctypes.c_ubyte * 512),
    ]


class EXCEPTION_RECORD(ctypes.Structure):
    pass


EXCEPTION_RECORD._fields_ = [
    ('ExceptionCode', wt.DWORD), ('ExceptionFlags', wt.DWORD),
    ('ExceptionRecord', ctypes.POINTER(EXCEPTION_RECORD)),
    ('ExceptionAddress', ctypes.c_void_p), ('NumberParameters', wt.DWORD),
    ('ExceptionInformation', ctypes.c_size_t * 15),
]


class EXCEPTION_DEBUG_INFO(ctypes.Structure):
    _fields_ = [('ExceptionRecord', EXCEPTION_RECORD), ('dwFirstChance', wt.DWORD)]


class CREATE_THREAD_DEBUG_INFO(ctypes.Structure):
    _fields_ = [('hThread', wt.HANDLE), ('lpThreadLocalBase', ctypes.c_void_p),
                ('lpStartAddress', ctypes.c_void_p)]


class CREATE_PROCESS_DEBUG_INFO(ctypes.Structure):
    _fields_ = [('hFile', wt.HANDLE), ('hProcess', wt.HANDLE), ('hThread', wt.HANDLE),
                ('lpBaseOfImage', ctypes.c_void_p), ('dwDebugInfoFileOffset', wt.DWORD),
                ('nDebugInfoSize', wt.DWORD), ('lpThreadLocalBase', ctypes.c_void_p),
                ('lpStartAddress', ctypes.c_void_p), ('lpImageName', ctypes.c_void_p),
                ('fUnicode', wt.WORD)]


class EXIT_THREAD_DEBUG_INFO(ctypes.Structure):
    _fields_ = [('dwExitCode', wt.DWORD)]


class EXIT_PROCESS_DEBUG_INFO(ctypes.Structure):
    _fields_ = [('dwExitCode', wt.DWORD)]


class LOAD_DLL_DEBUG_INFO(ctypes.Structure):
    _fields_ = [('hFile', wt.HANDLE), ('lpBaseOfDll', ctypes.c_void_p),
                ('dwDebugInfoFileOffset', wt.DWORD), ('nDebugInfoSize', wt.DWORD),
                ('lpImageName', ctypes.c_void_p), ('fUnicode', wt.WORD)]


class UNLOAD_DLL_DEBUG_INFO(ctypes.Structure):
    _fields_ = [('lpBaseOfDll', ctypes.c_void_p)]


class RIP_INFO(ctypes.Structure):
    _fields_ = [('dwType', wt.DWORD), ('dwError', wt.DWORD)]


class U(ctypes.Union):
    _fields_ = [('Exception', EXCEPTION_DEBUG_INFO),
                ('CreateThread', CREATE_THREAD_DEBUG_INFO),
                ('CreateProcessInfo', CREATE_PROCESS_DEBUG_INFO),
                ('ExitThread', EXIT_THREAD_DEBUG_INFO),
                ('ExitProcess', EXIT_PROCESS_DEBUG_INFO),
                ('LoadDll', LOAD_DLL_DEBUG_INFO),
                ('UnloadDll', UNLOAD_DLL_DEBUG_INFO),
                ('RipInfo', RIP_INFO)]


class DEBUG_EVENT(ctypes.Structure):
    _fields_ = [('dwDebugEventCode', wt.DWORD), ('dwProcessId', wt.DWORD),
                ('dwThreadId', wt.DWORD), ('u', U)]


class THREADENTRY32(ctypes.Structure):
    _fields_ = [('dwSize', wt.DWORD), ('cntUsage', wt.DWORD), ('th32ThreadID', wt.DWORD),
                ('th32OwnerProcessID', wt.DWORD), ('tpBasePri', wt.LONG),
                ('tpDeltaPri', wt.LONG), ('dwFlags', wt.DWORD)]


def find_pid():
    for line in subprocess.check_output(['tasklist', '/FI', 'IMAGENAME eq Nec.exe', '/FO', 'CSV', '/NH'],
                                        text=True).splitlines():
        p = [x.strip('"') for x in line.split('","')]
        if p and p[0].lower() == 'nec.exe':
            return int(p[1])
    return None


def read_abs(h, addr, size):
    if not addr or addr < 0x10000 or addr > 0x7fffffff:
        return b''
    buf = ctypes.create_string_buffer(size)
    got = ctypes.c_size_t()
    ok = k32.ReadProcessMemory(h, ctypes.c_void_p(addr), buf, size, ctypes.byref(got))
    if not ok:
        return b''
    return buf.raw[:got.value]


def set_bps(thread_handle, addrs):
    ctx = WOW64_CONTEXT()
    ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS
    if not k32.Wow64GetThreadContext(thread_handle, ctypes.byref(ctx)):
        return False
    ctx.Dr0, ctx.Dr1, ctx.Dr2, ctx.Dr3 = (list(addrs) + [0, 0, 0, 0])[:4]
    dr7 = 0
    # local enable + RW=write(01) + LEN=4(11) for each used slot
    for i in range(len(addrs)):
        dr7 |= (1 << (i * 2))
        dr7 |= (0b01 << (16 + i * 4))
        dr7 |= (0b11 << (18 + i * 4))
    ctx.Dr7 = dr7
    ctx.Dr6 = 0
    ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS
    return bool(k32.Wow64SetThreadContext(thread_handle, ctypes.byref(ctx)))


def main():
    pid = find_pid()
    if pid is None:
        raise SystemExit('Nec.exe is not running; load mission 6 first')
    h = dm.k32.OpenProcess(0x1F0FFF, False, pid)
    base = dm.module_base(h)
    sb = struct.unpack('<I', dm.read(h, base, 0x480f14, 4))[0]
    print('pid', pid, 'base', hex(base), 'spritebank', hex(sb), flush=True)
    if not sb:
        raise SystemExit('spritebank not allocated yet')
    addrs = [sb + 0x20000, sb + 0x28000, sb + 0x30000, sb + 0x40000]
    print('breakpoints', [hex(a) for a in addrs], flush=True)

    if not k32.DebugActiveProcess(pid):
        raise SystemExit('DebugActiveProcess failed: %s' % ctypes.get_last_error())
    k32.DebugSetProcessKillOnExit(False)
    print('attached', flush=True)

    # trigger input in the background (skip turn: down x3 + space)
    def input_thread():
        time.sleep(2.0)
        for _ in range(40):
            yield_skip = True
            for vk in (0x28, 0x28, 0x28, 0x20):
                sc = ctypes.windll.user32.MapVirtualKeyW(vk, 0) & 0xFF
                ctypes.windll.user32.PostMessageW(hwnd, 0x0100, vk, 1 | (sc << 16))
                ctypes.windll.user32.PostMessageW(hwnd, 0x0102, vk, 1 | (sc << 16))
                time.sleep(0.12)
                ctypes.windll.user32.PostMessageW(hwnd, 0x0101, vk, 1 | (sc << 16) | 0xC0000000)
                time.sleep(0.18)
            time.sleep(4.0)

    found = []
    t0 = time.time()
    log = open(os.path.join(OUT, 'watch.log'), 'w', encoding='utf-8')

    def logline(s):
        print(s, flush=True)
        log.write(s + '\n')
        log.flush()

    # find window for input thread
    U32 = ctypes.WinDLL('user32', use_last_error=True)
    U32.EnumWindows.argtypes = [ctypes.WINFUNCTYPE(wt.BOOL, wt.HWND, wt.LPARAM), wt.LPARAM]
    U32.GetWindowThreadProcessId.argtypes = [wt.HWND, ctypes.POINTER(wt.DWORD)]
    U32.GetWindowTextW.argtypes = [wt.HWND, wt.LPWSTR, ctypes.c_int]
    foundw = []

    @ctypes.WINFUNCTYPE(wt.BOOL, wt.HWND, wt.LPARAM)
    def cb(hwnd, lp):
        p = wt.DWORD()
        U32.GetWindowThreadProcessId(hwnd, ctypes.byref(p))
        if p.value == pid:
            b = ctypes.create_unicode_buffer(256)
            U32.GetWindowTextW(hwnd, b, 256)
            if 'nectaris' in b.value.lower():
                foundw.append(hwnd)
        return True

    U32.EnumWindows(cb, 0)
    hwnd = foundw[0] if foundw else None
    print('hwnd', hwnd, flush=True)
    th = threading.Thread(target=input_thread, daemon=True)
    th.start()

    armed = True
    while time.time() - t0 < 150.0:
        ev = DEBUG_EVENT()
        if not k32.WaitForDebugEvent(ctypes.byref(ev), 500):
            continue
        code = ev.dwDebugEventCode
        if code == CREATE_PROCESS_DEBUG_EVENT:
            try:
                set_bps(ev.u.CreateProcessInfo.hThread, addrs)
            except Exception as exc:
                logline('set bps failed: %s' % exc)
        elif code == CREATE_THREAD_DEBUG_EVENT:
            set_bps(ev.u.CreateThread.hThread, addrs)
        elif code == EXCEPTION_DEBUG_EVENT:
            rec = ev.u.Exception.ExceptionRecord
            if rec.ExceptionCode == EXCEPTION_SINGLE_STEP:
                tctx = WOW64_CONTEXT()
                tctx.ContextFlags = CONTEXT_DEBUG_REGISTERS | CONTEXT_i386 | 2
                hthread = k32.OpenThread(0x1F03FF, False, ev.dwThreadId)
                if hthread:
                    k32.Wow64GetThreadContext(hthread, ctypes.byref(tctx))
                eip = tctx.Eip
                regs = {'eip': eip, 'eax': tctx.Eax, 'ebx': tctx.Ebx, 'ecx': tctx.Ecx,
                        'edx': tctx.Edx, 'esi': tctx.Esi, 'edi': tctx.Edi,
                        'ebp': tctx.Ebp, 'esp': tctx.Esp, 'dr6': tctx.Dr6}
                logline('HIT ' + json.dumps(regs))
                open(os.path.join(OUT, 'hit_%03d_code.bin' % len(found)), 'wb').write(
                    read_abs(h, eip - 32, 64))
                for name in ('esi', 'edi', 'edx', 'ecx', 'eax', 'ebx', 'esp'):
                    val = regs[name]
                    buf = read_abs(h, val, 256)
                    if buf:
                        open(os.path.join(OUT, 'hit_%03d_mem_%s_%08x.bin' %
                                          (len(found), name, val)), 'wb').write(buf)
                found.append(regs)
                if hthread:
                    k32.CloseHandle(hthread)
                if len(found) >= 40:
                    break
            # continue
        elif code == EXIT_PROCESS_DEBUG_EVENT:
            logline('process exited')
            break
        k32.ContinueDebugEvent(ev.dwProcessId, ev.dwThreadId, DBG_CONTINUE)

    logline('hits %d' % len(found))
    k32.DebugActiveProcessStop(pid)
    log.close()
    print('done', flush=True)


if __name__ == '__main__':
    main()
