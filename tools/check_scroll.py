"""Check the running original map's cursor/scroll state and capture frames."""
import ctypes, ctypes.wintypes as wt, os, sys, time, struct, subprocess
sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
OUT = os.path.join(ROOT, 'dump', 'orig_maps')
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
def tap(vk, hold=0.10, gap=0.22):
    focus(); sc = U32.MapVirtualKeyW(vk, 0) & 0xFF
    key(vk, 1); U32.PostMessageW(hwnd, 0x0100, vk, 1 | (sc << 16)); U32.PostMessageW(hwnd, 0x0102, vk, 1 | (sc << 16))
    time.sleep(hold); key(vk, 0); U32.PostMessageW(hwnd, 0x0101, vk, 1 | (sc << 16) | 0xC0000000); time.sleep(gap)
def capture(name):
    fb = dm.read_abs(h, u32(0x480f18), 704 * 544)
    pal = dm.read(h, base, 0x480f20, 1028)
    rgb = bytearray(768)
    for i in range(256):
        rgb[i*3] = pal[4+i*4]; rgb[i*3+1] = pal[5+i*4]; rgb[i*3+2] = pal[6+i*4]
    im = Image.frombytes('P', (704, 544), fb).transpose(Image.FLIP_TOP_BOTTOM).crop((32, 32, 672, 512))
    im.putpalette(bytes(rgb)); im.convert('RGB').save(os.path.join(OUT, name + '.png'))
    print('captured', name)

def state():
    return (u8(0x495c52), u8(0x481a72), u8(0x481a71),
            u8(0x4834ba), u8(0x4834c0), u8(0x495c05), u8(0x495c06))
print('state c52,cx,cy,camx,camy,ox,oy =', state())
capture('nector_default')
for _ in range(20):
    tap(0x27)
print('after right x20:', state())
capture('nector_right')
for _ in range(20):
    tap(0x28)
print('after down x20:', state())
capture('nector_down')
for _ in range(30):
    tap(0x25)
for _ in range(30):
    tap(0x26)
print('after left/up:', state())
capture('nector_tl')
dm.k32.CloseHandle(h)
print('done')
