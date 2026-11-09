"""Dump every large committed memory region of the running game for inspection.

Navigates the original game into the combat screen (so battle art is loaded),
then:
  * dumps each committed region >= 128 KB to dump/banks/region_*.bin
  * renders each region as 8bpp palette images at several widths
  * writes dump/banks/index.json + contact.png
  * also dumps the known banks (spritebank, tilebank, atlas, framebuffer)

Usage: python tools/dump_large_banks.py [--no-battle]
"""
import ctypes, ctypes.wintypes as wt, os, sys, time, struct, subprocess, json, hashlib
sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
GAME = os.path.join(ROOT, 'game')
OUT = os.path.join(ROOT, 'dump', 'banks')
os.makedirs(OUT, exist_ok=True)

BATTLE = '--no-battle' not in sys.argv

K32 = ctypes.WinDLL('kernel32', use_last_error=True)
K32.CreateFileMappingW.restype = wt.HANDLE
K32.CreateFileMappingW.argtypes = [wt.HANDLE, ctypes.c_void_p, wt.DWORD, wt.DWORD, wt.DWORD, wt.LPCWSTR]
K32.MapViewOfFile.restype = ctypes.c_void_p
K32.MapViewOfFile.argtypes = [wt.HANDLE, wt.DWORD, wt.DWORD, wt.DWORD, ctypes.c_size_t]
hmap = K32.CreateFileMappingW(wt.HANDLE(-1).value, None, 4, 0, 4096, "Local\\NecFakeInput")
ptr = K32.MapViewOfFile(hmap, 0xF001F, 0, 0, 4096)
ctypes.memset(ptr, 0, 4096)


def i32(off, v=None):
    if v is None:
        return ctypes.c_long.from_address(ptr + off).value
    ctypes.c_long.from_address(ptr + off).value = v


def key(vk, d):
    ctypes.c_ubyte.from_address(ptr + 36 + vk).value = 1 if d else 0


i32(0, 0x4E454331)
i32(4, 1)
i32(12, 0)
i32(16, 0)
i32(28, 0)
for _i in range(256):
    key(_i, 0)

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


for line in subprocess.check_output(['tasklist', '/FI', 'IMAGENAME eq Nec.exe', '/FO', 'CSV', '/NH'],
                                    text=True).splitlines():
    parts = [x.strip('"') for x in line.split('","')]
    if parts and parts[0].lower() == 'nec.exe':
        subprocess.call(['taskkill', '/F', '/PID', parts[1]], stdout=subprocess.DEVNULL)

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


def u8(va):
    return dm.read(h, base, va, 1)[0]


def u32(va):
    return struct.unpack('<I', dm.read(h, base, va, 4))[0]


def focus():
    fg = U32.GetForegroundWindow()
    ft = U32.GetWindowThreadProcessId(fg, None)
    mt = K32.GetCurrentThreadId()
    U32.AttachThreadInput(mt, ft, True)
    U32.ShowWindow(hwnd, 5)
    U32.BringWindowToTop(hwnd)
    U32.SetForegroundWindow(hwnd)
    U32.AttachThreadInput(mt, ft, False)
    U32.PostMessageW(hwnd, 0x001C, 1, 0)
    U32.PostMessageW(hwnd, 0x0006, 1, 0)
    U32.PostMessageW(hwnd, 0x0007, 0, 0)


def tap(vk, hold=0.14, gap=0.30):
    focus()
    sc = U32.MapVirtualKeyW(vk, 0) & 0xFF
    key(vk, 1)
    U32.PostMessageW(hwnd, 0x0100, vk, 1 | (sc << 16))
    U32.PostMessageW(hwnd, 0x0102, vk, 1 | (sc << 16))
    time.sleep(hold)
    key(vk, 0)
    U32.PostMessageW(hwnd, 0x0101, vk, 1 | (sc << 16) | 0xC0000000)
    time.sleep(gap)


RIGHT, DOWN, LEFT, UP, SPACE, ENTER = 0x27, 0x28, 0x25, 0x26, 0x20, 0x0D

# ---- boot to map (or battle) ----
focus()
t0 = time.time()
toggle = False
while time.time() - t0 < 50:
    if u8(0x495c02) == 15 and u8(0x495c03) == 10:
        break
    tap(SPACE if not toggle else ENTER, gap=0.45)
    toggle = not toggle
print('map after %.1fs' % (time.time() - t0), flush=True)
time.sleep(3.0)

if BATTLE:
    for _ in range(22):
        tap(LEFT, gap=0.09)
    for _ in range(22):
        tap(UP, gap=0.09)
    for _ in range(7):
        bx = u8(0x481a72)
        for _ in range(3):
            tap(RIGHT, gap=0.16)
            if u8(0x481a72) > bx:
                break
    for _ in range(4):
        by = u8(0x481a71)
        for _ in range(3):
            tap(DOWN, gap=0.16)
            if u8(0x481a71) > by:
                break
    for vk in [SPACE, SPACE, RIGHT, RIGHT, SPACE, SPACE, RIGHT, DOWN, SPACE]:
        tap(vk, gap=0.40)
    time.sleep(4.0)
    print('battle reached', flush=True)

# ---- known banks ----
pal = dm.read(h, base, 0x480f20, 4 + 256 * 4)
PAL = [(pal[4 + i * 4], pal[5 + i * 4], pal[6 + i * 4]) for i in range(256)]
open(os.path.join(OUT, 'palette.bin'), 'wb').write(pal)

sb_ptr = u32(0x480f14)
tb_ptr = u32(0x481500)
fb_ptr = u32(0x480f18)
for name, addr, size in [('spritebank', sb_ptr, 0x100000), ('tilebank', tb_ptr, 0x80000),
                         ('atlas', None, 0x20000), ('framebuffer', fb_ptr, 704 * 544)]:
    try:
        if name == 'atlas':
            data = dm.read(h, base, 0x484730, size)
        elif addr:
            data = dm.read_abs(h, addr, size)
        else:
            continue
        open(os.path.join(OUT, name + '.bin'), 'wb').write(data)
        print('bank', name, hex(addr or 0x484730), len(data), flush=True)
    except OSError as exc:
        print('bank', name, 'failed', exc, flush=True)

# ---- same known banks at the module data area ----
try:
    open(os.path.join(OUT, 'game_data_480000.bin'), 'wb').write(dm.read(h, base, 0x480000, 0x16000))
except OSError as exc:
    print('data area failed', exc)

# ---- enumerate large committed regions ----
VirtualQueryEx = K32.VirtualQueryEx
VirtualQueryEx.argtypes = [wt.HANDLE, ctypes.c_void_p, ctypes.c_void_p, ctypes.c_size_t]


class MBI(ctypes.Structure):
    _fields_ = [('BaseAddress', ctypes.c_void_p), ('AllocationBase', ctypes.c_void_p),
                ('AllocationProtect', wt.DWORD), ('PartitionId', wt.WORD),
                ('RegionSize', ctypes.c_size_t), ('State', wt.DWORD),
                ('Protect', wt.DWORD), ('Type', wt.DWORD)]


regions = []
addr = 0x10000
maxaddr = 0x7fff0000
while addr < maxaddr:
    mbi = MBI()
    got = VirtualQueryEx(h, ctypes.c_void_p(addr), ctypes.byref(mbi), ctypes.sizeof(mbi))
    if not got:
        break
    if mbi.State == 0x1000 and mbi.RegionSize >= 0x20000:
        regions.append((mbi.BaseAddress or 0, mbi.RegionSize, mbi.Protect, mbi.Type))
    nxt = (mbi.BaseAddress or 0) + mbi.RegionSize
    if nxt <= addr:
        break
    addr = nxt

print('large regions', len(regions), flush=True)

index = []
for i, (rva, size, protect, typ) in enumerate(regions):
    cap = min(size, 0x800000)
    try:
        data = dm.read_abs(h, rva, cap)
    except OSError:
        continue
    if not data:
        continue
    fn = 'region_%02d_%08x_%04dK.bin' % (i, rva, size // 1024)
    open(os.path.join(OUT, fn), 'wb').write(data)
    nz = sum(1 for x in data if x)
    index.append({
        'id': i, 'address': hex(rva), 'size': size, 'dumped': len(data),
        'protect': hex(protect), 'type': hex(typ),
        'nonzero': nz, 'nonzero_frac': round(nz / max(1, len(data)), 4),
        'raw': fn,
    })
print('dumped regions', len(index), flush=True)

# ---- render region previews ----
def render_8bpp(data, width, maxh=1400):
    n = len(data)
    hgt = min(maxh, max(1, n // width))
    im = Image.new('RGB', (width, hgt), (0, 0, 0))
    px = im.load()
    for y in range(hgt):
        row = y * width
        if row >= n:
            break
        for x in range(width):
            idx = data[row + x]
            px[x, y] = PAL[idx]
    return im


thumbs = []
for e in index:
    data = open(os.path.join(OUT, e['raw']), 'rb').read()
    variants = {}
    for w in (320, 640, 1024):
        im = render_8bpp(data, w)
        vf = e['raw'].replace('.bin', '_w%d.png' % w)
        im.save(os.path.join(OUT, vf))
        variants[w] = vf
    e['previews'] = variants
    th = render_8bpp(data, 640).resize((256, 192), Image.BILINEAR)
    thumbs.append((e, th))

json.dump(index, open(os.path.join(OUT, 'index.json'), 'w'), indent=1)

cols = 6
rows = (len(thumbs) + cols - 1) // cols
sheet = Image.new('RGB', (cols * 270, rows * 230), (18, 18, 26))
d = ImageDraw.Draw(sheet)
for i, (e, th) in enumerate(thumbs):
    x, y = (i % cols) * 270 + 5, (i // cols) * 230 + 18
    sheet.paste(th, (x, y))
    d.text((x, y - 13), '%s %dK nz%.2f' % (e['address'], e['size'] // 1024, e['nonzero_frac']),
           fill=(255, 230, 100))
sheet.save(os.path.join(OUT, 'contact.png'))
print('wrote', OUT, 'regions', len(index), flush=True)
dm.k32.CloseHandle(h)
try:
    proc.terminate()
except Exception:
    pass
print('done', flush=True)
