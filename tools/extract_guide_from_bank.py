"""Extract the two guide frames from the live spritebank for all 24 unit types.

The WEAPON DATA screen draws the art from spritebank tiles:
  left/horizontal : 384,385,386,387 at (4,0),(36,0),(4,32),(36,32)
  right/diagonal  : 388,389,390,391 at the same offsets
Each tile is 32x32, stored bottom-up.  The four tiles are the per-type art that
the game decompresses into the bank when the screen opens.
"""
import ctypes, ctypes.wintypes as wt, os, sys, time, struct, subprocess, json
from PIL import Image
sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
GUIDE = os.path.join(ROOT, 'dump', 'guide')
UNITS = os.path.join(ROOT, 'assets', 'units')
GUIDE_STATE = 0x418d20
TILES_L = (384, 385, 386, 387)
TILES_R = (388, 389, 390, 391)
OFFS = ((4, 0), (36, 0), (4, 32), (36, 32))

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
dm.k32.WriteProcessMemory.argtypes = [wt.HANDLE, ctypes.c_void_p, ctypes.c_void_p, ctypes.c_size_t, ctypes.POINTER(ctypes.c_size_t)]
dm.k32.WriteProcessMemory.restype = wt.BOOL

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

pid = None
for line in subprocess.check_output(['tasklist', '/FI', 'IMAGENAME eq Nec.exe', '/FO', 'CSV', '/NH'], text=True).splitlines():
    parts = [x.strip('"') for x in line.split('","')]
    if parts and parts[0].lower() == 'nec.exe': pid = int(parts[1])
if not pid: raise SystemExit('Nec.exe not running')
hwnd = find_window(pid)
h = dm.k32.OpenProcess(0x1F0FFF, False, pid); base = dm.module_base(h)
def u8(va): return dm.read(h, base, va, 1)[0]
def u32(va): return struct.unpack('<I', dm.read(h, base, va, 4))[0]
def wr(va, data):
    got = ctypes.c_size_t()
    if not dm.k32.WriteProcessMemory(h, ctypes.c_void_p(base + (va - 0x400000)), data, len(data), ctypes.byref(got)):
        raise OSError('write %#x' % va)

def focus():
    fg = U32.GetForegroundWindow(); ft = U32.GetWindowThreadProcessId(fg, None); mt = K32.GetCurrentThreadId()
    U32.AttachThreadInput(mt, ft, True); U32.ShowWindow(hwnd, 5); U32.BringWindowToTop(hwnd); U32.SetForegroundWindow(hwnd)
    U32.AttachThreadInput(mt, ft, False)
    U32.PostMessageW(hwnd, 0x001C, 1, 0); U32.PostMessageW(hwnd, 0x0006, 1, 0); U32.PostMessageW(hwnd, 0x0007, 0, 0)
def tap(vk, hold=0.14, gap=0.35):
    focus(); sc = U32.MapVirtualKeyW(vk, 0) & 0xFF
    key(vk, 1); U32.PostMessageW(hwnd, 0x0100, vk, 1 | (sc << 16)); U32.PostMessageW(hwnd, 0x0102, vk, 1 | (sc << 16))
    time.sleep(hold); key(vk, 0); U32.PostMessageW(hwnd, 0x0101, vk, 1 | (sc << 16) | 0xC0000000); time.sleep(gap)
RIGHT, DOWN, LEFT, UP, SPACE = 0x27, 0x28, 0x25, 0x26, 0x20
def is_guide(): return u32(0x483270) == GUIDE_STATE
def close_guide():
    if is_guide(): tap(RIGHT, gap=0.35)
    tap(RIGHT, gap=0.25)
def reset_cursor():
    for _ in range(24): tap(LEFT, gap=0.06)
    for _ in range(24): tap(UP, gap=0.06)
    for _ in range(7):
        bx = u8(0x481a72)
        for _ in range(3):
            tap(RIGHT, gap=0.10)
            if u8(0x481a72) > bx: break
    for _ in range(4):
        by = u8(0x481a71)
        for _ in range(3):
            tap(DOWN, gap=0.10)
            if u8(0x481a71) > by: break

def palette():
    pal = dm.read(h, base, 0x480f20, 1028)
    return [(pal[4 + i * 4], pal[5 + i * 4], pal[6 + i * 4]) for i in range(256)]

def frame_from_bank(bank, tiles, pal):
    img = Image.new('RGBA', (64, 64), (0, 0, 0, 0)); px = img.load()
    for t, (ox, oy) in zip(tiles, OFFS):
        tile = dm.read_abs(h, bank + t * 1024, 1024)
        for y in range(32):
            for x in range(32):
                v = tile[y * 32 + x]
                if v:
                    xx = ox + x; yy = oy + (31 - y)
                    if 0 <= xx < 64 and 0 <= yy < 64:
                        px[xx, yy] = pal[(0xE0 | (v & 0xF)) & 0xFF] + (255,)
    return img

def crop_content(img):
    bbox = img.getbbox()
    return img.crop(bbox) if bbox else img

close_guide()
reset_cursor()
cell = u8(0x495820 + 4 * 32 + 7); slot = cell & 0x3F; orig = u8(0x4957a0 + slot)
print('slot', slot, 'orig', hex(orig))

summary = []
for t in range(24):
    if is_guide(): close_guide()
    cx, cy = u8(0x481a72), u8(0x481a71)
    if (cx, cy) != (7, 4): reset_cursor()
    wr(0x4957a0 + slot, bytes([(orig & 0xE0) | t]))
    tap(SPACE, gap=0.35); tap(DOWN, gap=0.30); tap(DOWN, gap=0.30); tap(SPACE, gap=0.65)
    if not is_guide():
        close_guide(); reset_cursor()
        wr(0x4957a0 + slot, bytes([(orig & 0xE0) | t]))
        tap(SPACE, gap=0.35); tap(DOWN, gap=0.30); tap(DOWN, gap=0.30); tap(SPACE, gap=0.65)
    ok = is_guide()
    time.sleep(1.2)  # let the guide finish decompressing the per-type art tiles
    bank = u32(0x480f14)
    pal = palette()
    left = crop_content(frame_from_bank(bank, TILES_L, pal))
    right = crop_content(frame_from_bank(bank, TILES_R, pal))
    folder = os.path.join(UNITS, 'unit_%02d' % t)
    os.makedirs(folder, exist_ok=True)
    left.save(os.path.join(folder, 'combat_alive.png'))
    right.save(os.path.join(folder, 'combat_diagonal.png'))
    # provenance: the eight raw 32x32 tiles
    raw = b''.join(dm.read_abs(h, bank + i * 1024, 1024) for i in list(TILES_L) + list(TILES_R))
    open(os.path.join(GUIDE, 'bank_type_%02d.bin' % t), 'wb').write(raw)
    summary.append({'id': t, 'left': left.size, 'right': right.size, 'guide': ok})
    print('type %2d guide=%s left=%s right=%s' % (t, ok, left.size, right.size), flush=True)
    if t == 6:
        open(os.path.join(GUIDE, 'spritebank_type06.bin'), 'wb').write(dm.read_abs(h, bank, 0x100000))

json.dump(summary, open(os.path.join(GUIDE, 'bank_frames.json'), 'w'), indent=2)
wr(0x4957a0 + slot, bytes([orig]))
close_guide()
dm.k32.CloseHandle(h)
print('done')
