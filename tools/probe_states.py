"""Poll the original game's screen-state pointer through the intro to find the
pre-loading screen and the mission-index timing."""
import ctypes, ctypes.wintypes as wt, os, sys, time, struct, subprocess
sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
EXE = os.path.join(ROOT, 'game', 'Nec.exe')
K32 = ctypes.WinDLL('kernel32', use_last_error=True)
K32.CreateFileMappingW.restype = wt.HANDLE
K32.CreateFileMappingW.argtypes = [wt.HANDLE, ctypes.c_void_p, wt.DWORD, wt.DWORD, wt.DWORD, wt.LPCWSTR]
K32.MapViewOfFile.restype = ctypes.c_void_p
K32.MapViewOfFile.argtypes = [wt.HANDLE, wt.DWORD, wt.DWORD, wt.DWORD, ctypes.c_size_t]
hmap = K32.CreateFileMappingW(wt.HANDLE(-1).value, None, 4, 0, 4096, "Local\\NecFakeInput")
fptr = K32.MapViewOfFile(hmap, 0xF001F, 0, 0, 4096)
def i32(off, v=None):
    if v is None: return ctypes.c_long.from_address(fptr + off).value
    ctypes.c_long.from_address(fptr + off).value = v
def key(vk, d): ctypes.c_ubyte.from_address(fptr + 36 + vk).value = 1 if d else 0
i32(0, 0x4E454331); i32(4, 1); i32(12, 0); i32(16, 0); i32(28, 0)
U32 = ctypes.WinDLL('user32', use_last_error=True)
U32.EnumWindows.argtypes = [ctypes.WINFUNCTYPE(wt.BOOL, wt.HWND, wt.LPARAM), wt.LPARAM]
U32.GetWindowThreadProcessId.argtypes = [wt.HWND, ctypes.POINTER(wt.DWORD)]
U32.GetWindowTextW.argtypes = [wt.HWND, wt.LPWSTR, ctypes.c_int]
U32.PostMessageW.argtypes = [wt.HWND, wt.UINT, wt.WPARAM, wt.LPARAM]
U32.MapVirtualKeyW.argtypes = [wt.UINT, wt.UINT]
U32.GetForegroundWindow.restype = wt.HWND
U32.AttachThreadInput.argtypes = [wt.DWORD, wt.DWORD, wt.BOOL]
U32.SetForegroundWindow.argtypes = [wt.HWND]
U32.BringWindowToTop.argtypes = [wt.HWND]
U32.ShowWindow.argtypes = [wt.HWND, ctypes.c_int]
K32.GetCurrentThreadId.restype = wt.DWORD
dm.k32.WriteProcessMemory.argtypes = [wt.HANDLE, ctypes.c_void_p, ctypes.c_void_p, ctypes.c_size_t, ctypes.POINTER(ctypes.c_size_t)]
dm.k32.WriteProcessMemory.restype = wt.BOOL

def find_pid():
    for line in subprocess.check_output(['tasklist', '/FI', 'IMAGENAME eq Nec.exe', '/FO', 'CSV', '/NH'], text=True).splitlines():
        p = [x.strip('"') for x in line.split('","')]
        if p and p[0].lower() == 'nec.exe': return int(p[1])
    return None
def find_window(pid):
    found = []
    @ctypes.WINFUNCTYPE(wt.BOOL, wt.HWND, wt.LPARAM)
    def cb(hwnd, lp):
        p = wt.DWORD(); U32.GetWindowThreadProcessId(hwnd, ctypes.byref(p))
        if p.value == pid:
            b = ctypes.create_unicode_buffer(256); U32.GetWindowTextW(hwnd, b, 256)
            if 'nectaris' in b.value.lower(): found.append(hwnd)
        return True
    U32.EnumWindows(cb, 0)
    return found[0] if found else None

subprocess.run(['taskkill', '/F', '/IM', 'Nec.exe'], capture_output=True)
time.sleep(0.4)
subprocess.Popen([EXE], cwd=os.path.dirname(EXE))
time.sleep(4.5)
pid = find_pid(); hwnd = find_window(pid)
h = dm.k32.OpenProcess(0x1F0FFF, False, pid); base = dm.module_base(h)
def u32(va): return struct.unpack('<I', dm.read(h, base, va, 4))[0]
def u8(va): return dm.read(h, base, va, 1)[0]
def wr(va, data):
    got = ctypes.c_size_t()
    dm.k32.WriteProcessMemory(h, ctypes.c_void_p(base + (va - 0x400000)), data, len(data), ctypes.byref(got))
def focus():
    fg = U32.GetForegroundWindow(); ft = U32.GetWindowThreadProcessId(fg, None); mt = K32.GetCurrentThreadId()
    U32.AttachThreadInput(mt, ft, True); U32.ShowWindow(hwnd, 5); U32.BringWindowToTop(hwnd); U32.SetForegroundWindow(hwnd)
    U32.AttachThreadInput(mt, ft, False)
    U32.PostMessageW(hwnd, 0x001C, 1, 0); U32.PostMessageW(hwnd, 0x0006, 1, 0); U32.PostMessageW(hwnd, 0x0007, 0, 0)
def tap(vk, hold=0.08, gap=0.10):
    focus(); sc = U32.MapVirtualKeyW(vk, 0) & 0xFF
    key(vk, 1); U32.PostMessageW(hwnd, 0x0100, vk, 1 | (sc << 16)); U32.PostMessageW(hwnd, 0x0102, vk, 1 | (sc << 16))
    time.sleep(hold); key(vk, 0); U32.PostMessageW(hwnd, 0x0101, vk, 1 | (sc << 16) | 0xC0000000); time.sleep(gap)

target = int(sys.argv[1]) if len(sys.argv) > 1 else 15
focus()
start = time.time()
last = None
for i in range(160):
    # advance the intro/title with space
    if i % 3 == 0:
        tap(0x20)
    st = u32(0x483270)
    c52 = u8(0x495c52)
    c6a = u8(0x495c6a)
    cur = (u8(0x481a72), u8(0x481a71))
    tag = '%08x c52=%2d ch=%d cur=%s' % (st, c52, c6a, cur)
    if tag != last:
        print('%5.2fs %s' % (time.time() - start, tag), flush=True)
        last = tag
    # patch the mission index once we are in the intro/loading (before map)
    if st != 0x416700:
        wr(0x495c52, bytes([target & 0xF]))
    if st == 0x416700 and time.time() - start > 5:
        break
    time.sleep(0.12)
print('final state', hex(u32(0x483270)), 'mission', u8(0x495c52), 'cursor', u8(0x481a72), u8(0x481a71))
dm.k32.CloseHandle(h)
