"""Extract full original campaign maps by starting at the target mission with
the intro patch, hiding units, and stitching the scrolling 15x10 viewports."""
import ctypes, ctypes.wintypes as wt, os, sys, time, struct, subprocess, json
sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
OUT = os.path.join(ROOT, 'dump', 'orig_maps')
os.makedirs(OUT, exist_ok=True)
EXE = os.path.join(ROOT, 'game', 'Nec.exe')
MAP_STATE = 0x416700
CROP = (24, 36, 504, 372)

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
    pids = []
    for line in subprocess.check_output(['tasklist', '/FI', 'IMAGENAME eq Nec.exe', '/FO', 'CSV', '/NH'], text=True).splitlines():
        p = [x.strip('"') for x in line.split('","')]
        if p and p[0].lower() == 'nec.exe':
            pids.append(int(p[1]))
    # Some leftover instances can be inaccessible; use the first openable one.
    for pid in pids:
        h = dm.k32.OpenProcess(0x1F0FFF, False, pid)
        if h:
            dm.k32.CloseHandle(h)
            return pid
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

NAMES = {}
for m in json.load(open(os.path.join(ROOT, 'assets', 'data', 'campaign_missions.json'), encoding='utf-8'))['missions']:
    NAMES[m['mission'] - 1] = m['name'].lower()

def extract(mission):
    subprocess.run(['taskkill', '/F', '/IM', 'Nec.exe'], capture_output=True)
    time.sleep(0.35)
    subprocess.Popen([EXE], cwd=os.path.dirname(EXE))
    time.sleep(4.2)
    pid = find_pid(); hwnd = find_window(pid)
    h = dm.k32.OpenProcess(0x1F0FFF, False, pid); base = dm.module_base(h)
    def u32(va): return struct.unpack('<I', dm.read(h, base, va, 4))[0]
    def u8(va): return dm.read(h, base, va, 1)[0]
    def wr(va, data):
        got = ctypes.c_size_t()
        dm.k32.WriteProcessMemory(h, ctypes.c_void_p(base + (va - 0x400000)), data, len(data), ctypes.byref(got))
    kernel32 = ctypes.WinDLL('kernel32', use_last_error=True)
    kernel32.VirtualAllocEx.restype = ctypes.c_void_p
    kernel32.VirtualAllocEx.argtypes = [wt.HANDLE, ctypes.c_void_p, ctypes.c_size_t, wt.DWORD, wt.DWORD]
    kernel32.WriteProcessMemory.argtypes = [wt.HANDLE, ctypes.c_void_p, ctypes.c_void_p, ctypes.c_size_t, ctypes.POINTER(ctypes.c_size_t)]
    empty = kernel32.VirtualAllocEx(h, None, 0x10, 0x3000, 0x04)
    if not empty:
        print('  mission %2d: VirtualAllocEx failed (WinError %d)' % (mission + 1, ctypes.get_last_error()), flush=True)
        dm.k32.CloseHandle(h)
        return None
    got = ctypes.c_size_t()
    kernel32.WriteProcessMemory(h, ctypes.c_void_p(empty), b'\xff', 1, ctypes.byref(got))
    wr(0x46b988 + mission * 4, struct.pack('<I', empty))
    def focus():
        fg = U32.GetForegroundWindow(); ft = U32.GetWindowThreadProcessId(fg, None); mt = K32.GetCurrentThreadId()
        U32.AttachThreadInput(mt, ft, True); U32.ShowWindow(hwnd, 5); U32.BringWindowToTop(hwnd); U32.SetForegroundWindow(hwnd)
        U32.AttachThreadInput(mt, ft, False)
        U32.PostMessageW(hwnd, 0x001C, 1, 0); U32.PostMessageW(hwnd, 0x0006, 1, 0); U32.PostMessageW(hwnd, 0x0007, 0, 0)
    def tap(vk, hold=0.05, gap=0.07):
        focus(); sc = U32.MapVirtualKeyW(vk, 0) & 0xFF
        key(vk, 1); U32.PostMessageW(hwnd, 0x0100, vk, 1 | (sc << 16)); U32.PostMessageW(hwnd, 0x0102, vk, 1 | (sc << 16))
        time.sleep(hold); key(vk, 0); U32.PostMessageW(hwnd, 0x0101, vk, 1 | (sc << 16) | 0xC0000000); time.sleep(gap)
    def is_map(): return u32(0x483270) == MAP_STATE
    # intro: patch mission, advance slides
    for _ in range(240):
        if is_map():
            break
        wr(0x495c52, bytes([mission & 0xF]))
        tap(0x20)
    if not is_map():
        dm.k32.CloseHandle(h)
        return None
    time.sleep(0.8)
    tap(0x20)  # action selection: make the cursor/scroll respond
    time.sleep(0.4)
    w = u8(0x495c02); hgt = u8(0x495c03)
    # Odd-q maps draw a half hex row below the last cell row; the last 4 px of
    # the viewport are the UI border, so the art ends at h*32+12.
    full = Image.new('RGB', (w * 32, hgt * 32 + 12), (0, 0, 0))
    seen = set()
    for oy in range(0, hgt, 10):
        for ox in range(0, w, 15):
            # park the cursor at the far corner of this page so the camera
            # clamps to the page origin (it centres on the cursor otherwise)
            tx = 0 if ox == 0 else w - 1
            ty = 0 if oy == 0 else hgt - 1
            cx, cy = u8(0x481a72), u8(0x481a71)
            for _ in range(120):
                cx, cy = u8(0x481a72), u8(0x481a71)
                if cx == tx and cy == ty:
                    break
                if cx < tx:
                    tap(0x27, gap=0.12)
                elif cx > tx:
                    tap(0x25, gap=0.12)
                elif cy < ty:
                    tap(0x28, gap=0.12)
                elif cy > ty:
                    tap(0x26, gap=0.12)
            time.sleep(0.55)
            aox, aoy = u8(0x495c05), u8(0x495c06)
            fb = dm.read_abs(h, u32(0x480f18), 704 * 544)
            pal = dm.read(h, base, 0x480f20, 1028)
            rgb = bytearray(768)
            for i in range(256):
                rgb[i*3] = pal[4+i*4]; rgb[i*3+1] = pal[5+i*4]; rgb[i*3+2] = pal[6+i*4]
            im = Image.frombytes('P', (704, 544), fb).transpose(Image.FLIP_TOP_BOTTOM).crop((32, 32, 672, 512))
            im.putpalette(bytes(rgb))
            if os.environ.get('SAVE_PAGES'):
                im.convert('RGB').save(os.path.join(OUT, 'page_m%02d_ox%d_oy%d.png' % (mission + 1, aox, aoy)))
            crop = im.convert('RGB').crop(CROP)
            full.paste(crop, (aox * 32, aoy * 32))
            seen.add((aox, aoy))
            print('  mission %2d viewport ox=%d oy=%d' % (mission + 1, aox, aoy), flush=True)
    name = 'full_%02d_%s.png' % (mission + 1, NAMES.get(mission, 'mission'))
    full.save(os.path.join(OUT, name))
    print('mission %2d %s -> %s %s views=%d' % (mission + 1, NAMES.get(mission, '?'), name, full.size, len(seen)), flush=True)
    dm.k32.CloseHandle(h)
    return full.size

missions = list(range(16))
if len(sys.argv) > 1:
    missions = [int(x) for x in sys.argv[1:]]
for m in missions:
    extract(m)
print('done')
