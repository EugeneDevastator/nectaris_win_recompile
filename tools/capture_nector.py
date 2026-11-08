"""Start mission 16 (NECTOR) with the intro patch and capture its scrolling map."""
import ctypes, ctypes.wintypes as wt, os, sys, time, struct, subprocess
sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
OUT = os.path.join(ROOT, 'dump', 'orig_maps')
os.makedirs(OUT, exist_ok=True)
EXE = os.path.join(ROOT, 'game', 'Nec.exe')
MAP_STATE = 0x416700

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

mission = int(sys.argv[1]) if len(sys.argv) > 1 else 15
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
# empty unit list for this mission so terrain loads with no units
kernel32 = ctypes.WinDLL('kernel32', use_last_error=True)
kernel32.VirtualAllocEx.restype = ctypes.c_void_p
kernel32.VirtualAllocEx.argtypes = [wt.HANDLE, ctypes.c_void_p, ctypes.c_size_t, wt.DWORD, wt.DWORD]
kernel32.WriteProcessMemory.argtypes = [wt.HANDLE, ctypes.c_void_p, ctypes.c_void_p, ctypes.c_size_t, ctypes.POINTER(ctypes.c_size_t)]
empty = kernel32.VirtualAllocEx(h, None, 0x10, 0x3000, 0x04)
got = ctypes.c_size_t()
kernel32.WriteProcessMemory(h, ctypes.c_void_p(empty), b'\xff', 1, ctypes.byref(got))
wr(0x46b988 + mission * 4, struct.pack('<I', empty))
def focus():
    fg = U32.GetForegroundWindow(); ft = U32.GetWindowThreadProcessId(fg, None); mt = K32.GetCurrentThreadId()
    U32.AttachThreadInput(mt, ft, True); U32.ShowWindow(hwnd, 5); U32.BringWindowToTop(hwnd); U32.SetForegroundWindow(hwnd)
    U32.AttachThreadInput(mt, ft, False)
    U32.PostMessageW(hwnd, 0x001C, 1, 0); U32.PostMessageW(hwnd, 0x0006, 1, 0); U32.PostMessageW(hwnd, 0x0007, 0, 0)
def tap(vk, hold=0.10, gap=0.20):
    focus(); sc = U32.MapVirtualKeyW(vk, 0) & 0xFF
    key(vk, 1); U32.PostMessageW(hwnd, 0x0100, vk, 1 | (sc << 16)); U32.PostMessageW(hwnd, 0x0102, vk, 1 | (sc << 16))
    time.sleep(hold); key(vk, 0); U32.PostMessageW(hwnd, 0x0101, vk, 1 | (sc << 16) | 0xC0000000); time.sleep(gap)
def is_map(): return u32(0x483270) == MAP_STATE
def state():
    return dict(c52=u8(0x495c52), cx=u8(0x481a72), cy=u8(0x481a71),
                camx=u8(0x4834ba), camy=u8(0x4834c0), ox=u8(0x495c05), oy=u8(0x495c06),
                w=u8(0x495c02), h=u8(0x495c03))
def capture(name):
    fb = dm.read_abs(h, u32(0x480f18), 704 * 544)
    pal = dm.read(h, base, 0x480f20, 1028)
    rgb = bytearray(768)
    for i in range(256):
        rgb[i*3] = pal[4+i*4]; rgb[i*3+1] = pal[5+i*4]; rgb[i*3+2] = pal[6+i*4]
    im = Image.frombytes('P', (704, 544), fb).transpose(Image.FLIP_TOP_BOTTOM).crop((32, 32, 672, 512))
    im.putpalette(bytes(rgb)); im.convert('RGB').save(os.path.join(OUT, name + '.png'))
    print('captured', name, state())

# run the intro, patching the mission index every poll, stop as soon as the map appears
i = 0
while i < 120:
    if is_map():
        break
    wr(0x495c52, bytes([mission & 0xF]))
    tap(0x20)
    i += 1
print('map?', is_map(), state())
time.sleep(1.0)
tap(0x20)          # enter action selection so the cursor/scroll responds
time.sleep(0.4)
capture('nector_00_default')
# move to the top-left
for _ in range(40): tap(0x25)
for _ in range(40): tap(0x26)
time.sleep(0.4)
capture('nector_01_tl')
# move right across
for _ in range(40): tap(0x27)
time.sleep(0.4)
capture('nector_02_tr')
# move down
for _ in range(40): tap(0x28)
time.sleep(0.4)
capture('nector_03_br')
# move left
for _ in range(40): tap(0x25)
time.sleep(0.4)
capture('nector_04_bl')
dm.k32.CloseHandle(h)
print('done')
