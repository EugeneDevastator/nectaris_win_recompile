"""Load a campaign mission in the original game and capture its map.

Uses a remote-thread call to FUN_00405ad0 (mission start) after setting
DAT_00495c52/DAT_00495c6a.  The game is first brought to the map screen by
skipping the intro with SPACE.
"""
import ctypes, ctypes.wintypes as wt, os, sys, time, struct, subprocess
sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
OUT = os.path.join(ROOT, 'dump', 'orig_maps')
os.makedirs(OUT, exist_ok=True)
MAP_STATE = 0x416700
EXE = os.path.join(ROOT, 'game', 'Nec.exe')

# ---- fake input proxy ----
K32 = ctypes.WinDLL('kernel32', use_last_error=True)
K32.CreateFileMappingW.restype = wt.HANDLE
K32.CreateFileMappingW.argtypes = [wt.HANDLE, ctypes.c_void_p, wt.DWORD, wt.DWORD, wt.DWORD, wt.LPCWSTR]
K32.MapViewOfFile.restype = ctypes.c_void_p
K32.MapViewOfFile.argtypes = [wt.HANDLE, wt.DWORD, wt.DWORD, wt.DWORD, ctypes.c_size_t]
hmap = K32.CreateFileMappingW(wt.HANDLE(-1).value, None, 4, 0, 4096, "Local\\NecFakeInput")
ptr = K32.MapViewOfFile(hmap, 0xF001F, 0, 0, 4096)
def i32(off, v=None):
    if v is None: return ctypes.c_long.from_address(ptr + off).value
    ctypes.c_long.from_address(ptr + off).value = v
def key(vk, d): ctypes.c_ubyte.from_address(ptr + 36 + vk).value = 1 if d else 0
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
dm.k32.WriteProcessMemory.argtypes = [wt.HANDLE, ctypes.c_void_p, ctypes.c_void_p,
                                      ctypes.c_size_t, ctypes.POINTER(ctypes.c_size_t)]
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

pid = find_pid()
if pid is None:
    subprocess.Popen([EXE], cwd=os.path.dirname(EXE))
    time.sleep(5)
    pid = find_pid()
hwnd = find_window(pid)
h = dm.k32.OpenProcess(0x1F0FFF, False, pid)
base = dm.module_base(h)
def u32(va): return struct.unpack('<I', dm.read(h, base, va, 4))[0]
def wr(va, data):
    got = ctypes.c_size_t()
    dm.k32.WriteProcessMemory(h, ctypes.c_void_p(base + (va - 0x400000)), data, len(data), ctypes.byref(got))
def rd(va, n): return dm.read(h, base, va, n)

def focus():
    fg = U32.GetForegroundWindow(); ft = U32.GetWindowThreadProcessId(fg, None); mt = K32.GetCurrentThreadId()
    U32.AttachThreadInput(mt, ft, True); U32.ShowWindow(hwnd, 5); U32.BringWindowToTop(hwnd); U32.SetForegroundWindow(hwnd)
    U32.AttachThreadInput(mt, ft, False)
    U32.PostMessageW(hwnd, 0x001C, 1, 0); U32.PostMessageW(hwnd, 0x0006, 1, 0); U32.PostMessageW(hwnd, 0x0007, 0, 0)
def tap(vk, hold=0.10, gap=0.22):
    focus(); sc = U32.MapVirtualKeyW(vk, 0) & 0xFF
    key(vk, 1); U32.PostMessageW(hwnd, 0x0100, vk, 1 | (sc << 16)); U32.PostMessageW(hwnd, 0x0102, vk, 1 | (sc << 16))
    time.sleep(hold); key(vk, 0); U32.PostMessageW(hwnd, 0x0101, vk, 1 | (sc << 16) | 0xC0000000); time.sleep(gap)

def is_map(): return u32(0x483270) == MAP_STATE

# Skip title + intro until the map screen appears.
for _ in range(40):
    if is_map(): break
    tap(0x20)
time.sleep(0.8)
print('at map:', is_map(), 'state', hex(u32(0x483270)))

# ---- remote call to FUN_00405ad0 ----
kernel32 = ctypes.WinDLL('kernel32', use_last_error=True)
kernel32.VirtualAllocEx.restype = ctypes.c_void_p
kernel32.VirtualAllocEx.argtypes = [wt.HANDLE, ctypes.c_void_p, ctypes.c_size_t, wt.DWORD, wt.DWORD]
kernel32.WriteProcessMemory.argtypes = [wt.HANDLE, ctypes.c_void_p, ctypes.c_void_p, ctypes.c_size_t, ctypes.POINTER(ctypes.c_size_t)]
kernel32.CreateRemoteThread.restype = wt.HANDLE
kernel32.CreateRemoteThread.argtypes = [wt.HANDLE, ctypes.c_void_p, ctypes.c_size_t, ctypes.c_void_p, ctypes.c_void_p, wt.DWORD, ctypes.c_void_p]
kernel32.WaitForSingleObject.argtypes = [wt.HANDLE, wt.DWORD]

def rcall_va(target_va, argc=0):
    target = base + (target_va - 0x400000)
    mem = kernel32.VirtualAllocEx(h, None, 0x1000, 0x3000, 0x40)
    code = bytearray()
    for _ in range(argc):
        code += bytes([0x6A, 0])
    code += bytes([0xE8, 0, 0, 0, 0, 0x83, 0xC4, argc * 4, 0xC3])
    off = argc * 2
    struct.pack_into('<i', code, off + 1, target - (mem + off + 5))
    got = ctypes.c_size_t()
    kernel32.WriteProcessMemory(h, ctypes.c_void_p(mem), bytes(code), len(code), ctypes.byref(got))
    th = kernel32.CreateRemoteThread(h, None, 0, ctypes.c_void_p(mem), None, 0, None)
    kernel32.WaitForSingleObject(th, 5000)
    kernel32.CloseHandle(th)
    kernel32.VirtualFree = getattr(kernel32, 'VirtualFree', None)
    time.sleep(0.9)

def call_mission(mission, clean=False):
    wr(0x495c52, bytes([mission & 0xF]))
    wr(0x495c6a, bytes([mission >> 4]))
    wr(0x42fb74, b'\x00')
    wr(0x42a4e0, b'\x00')
    if clean:
        # Keep the real terrain/backdrop loader but give the mission an empty
        # unit list (a single 0xFF terminator) so no units are placed.
        if not hasattr(call_mission, '_empty'):
            mem = kernel32.VirtualAllocEx(h, None, 0x10, 0x3000, 0x04)
            got = ctypes.c_size_t()
            kernel32.WriteProcessMemory(h, ctypes.c_void_p(mem), b'\xff', 1, ctypes.byref(got))
            call_mission._empty = mem
        wr(0x46b988 + mission * 4, struct.pack('<I', call_mission._empty))
    rcall_va(0x405ad0, 5)
    if clean:
        # Park the cursor off-map and redraw so the selection frame is gone.
        wr(0x481a72, b'\xff')
        wr(0x481a71, b'\xff')
        rcall_va(0x405b70, 5)

def capture(name):
    fbp = u32(0x480f18)
    fb = dm.read_abs(h, fbp, 704 * 544)
    open(os.path.join(OUT, name + '.bin'), 'wb').write(fb)
    pal = rd(0x480f20, 1028)
    rgb = bytearray(768)
    for i in range(256):
        rgb[i*3] = pal[4+i*4]; rgb[i*3+1] = pal[5+i*4]; rgb[i*3+2] = pal[6+i*4]
    im = Image.frombytes('P', (704, 544), fb).transpose(Image.FLIP_TOP_BOTTOM).crop((32, 32, 672, 512))
    im.putpalette(bytes(rgb))
    im.convert('RGB').save(os.path.join(OUT, name + '.png'))
    print('captured', name)

def probe_objects():
    for i in range(0x42):
        o = 0x482c30 + i * 0x18
        idb = rd(o + 9, 1)[0]
        if idb:
            x = struct.unpack('<H', rd(o + 0xc, 2))[0]
            y = struct.unpack('<H', rd(o + 0x10, 2))[0]
            print('obj %2d @%#x id=%d sub=%d x=%d y=%d col=%#x' %
                  (i, o, idb, rd(o + 10, 1)[0], x, y, rd(o + 0x14, 1)[0]))

def probe_tilemap():
    w = rd(0x495c02, 1)[0]
    h = rd(0x495c03, 1)[0]
    stride = 16 if w <= 15 else 32
    rows = 11 if h <= 10 else 22
    tm = rd(0x4808a8, stride * rows * 2)
    print('tilemap w/h', w, h, 'stride', stride, 'rows', rows)
    for y in range(rows):
        vals = [struct.unpack_from('<H', tm, (y * stride + x) * 2)[0] for x in range(stride)]
        print('%2d: ' % y + ' '.join('%03x' % v for v in vals))

def hide_structs():
    # Mission 1's HQ is at cell (2,8); its descriptors live at row 8, x=2..3.
    w = rd(0x495c02, 1)[0]
    stride = 16 if w <= 15 else 32
    plain = rd(0x4808a8 + (8 * stride + 6) * 2, 2)
    for dx in (1, 2, 3, 4):
        wr(0x4808a8 + (8 * stride + dx) * 2, plain)
    rcall_va(0x419e90, 0)
    time.sleep(0.6)

if len(sys.argv) > 1:
    m = int(sys.argv[1])
    mode = sys.argv[2] if len(sys.argv) > 2 else ''
    clean = mode in ('clean', 'pan', 'hide')
    call_mission(m, clean)
    capture('mission_%02d%s' % (m + 1, '_clean' if clean else ''))
    if mode == 'pan':
        for _ in range(18):
            tap(0x27)   # right
        for _ in range(12):
            tap(0x28)   # down
        time.sleep(0.4)
        capture('mission_%02d_clean_b' % (m + 1))
    if len(sys.argv) > 3 and sys.argv[3] == 'probe':
        probe_objects()
    if mode == 'tilemap':
        probe_tilemap()
    if mode == 'hide':
        hide_structs()
        capture('mission_%02d_nostruct' % (m + 1))
else:
    for m in range(16):
        call_mission(m)
        capture('mission_%02d' % (m + 1))
dm.k32.CloseHandle(h)
print('done')
