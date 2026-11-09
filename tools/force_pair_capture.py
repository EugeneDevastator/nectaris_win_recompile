"""Force a specific combat pair by patching mission 1, attack, and capture block changes.

Default: player S-61 (slot 8) moved next to an enemy patched to EF-88 (slot 2),
then attack so the combat simulation runs S-61 vs EF-88.
"""
import ctypes, ctypes.wintypes as wt, os, sys, time, struct, subprocess, hashlib, json
sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
OUT = os.path.join(ROOT, 'dump', 'force_pair')
os.makedirs(os.path.join(OUT, 'frames'), exist_ok=True)

K32 = ctypes.WinDLL('kernel32', use_last_error=True)
K32.CreateFileMappingW.restype = wt.HANDLE
K32.CreateFileMappingW.argtypes = [wt.HANDLE, ctypes.c_void_p, wt.DWORD, wt.DWORD, wt.DWORD, wt.LPCWSTR]
K32.MapViewOfFile.restype = ctypes.c_void_p
K32.MapViewOfFile.argtypes = [wt.HANDLE, wt.DWORD, wt.DWORD, wt.DWORD, ctypes.c_size_t]
hmap = K32.CreateFileMappingW(wt.HANDLE(-1).value, None, 4, 0, 4096, "Local\\NecFakeInput")
ptr = K32.MapViewOfFile(hmap, 0xF001F, 0, 0, 4096)
ctypes.memset(ptr, 0, 4096)
ctypes.c_long.from_address(ptr + 0).value = 0x4E454331
ctypes.c_long.from_address(ptr + 4).value = 1
seq = [0]

U32 = ctypes.WinDLL('user32', use_last_error=True)
U32.EnumWindows.argtypes = [ctypes.WINFUNCTYPE(wt.BOOL, wt.HWND, wt.LPARAM), wt.LPARAM]
U32.GetWindowThreadProcessId.argtypes = [wt.HWND, ctypes.POINTER(wt.DWORD)]
U32.GetWindowTextW.argtypes = [wt.HWND, wt.LPWSTR, ctypes.c_int]
U32.PostMessageW.argtypes = [wt.HWND, wt.UINT, wt.WPARAM, wt.LPARAM]
U32.MapVirtualKeyW.argtypes = [wt.UINT, wt.UINT]


def find_pid():
    for line in subprocess.check_output(['tasklist', '/FI', 'IMAGENAME eq Nec.exe', '/FO', 'CSV', '/NH'],
                                        text=True).splitlines():
        p = [x.strip('"') for x in line.split('","')]
        if p and p[0].lower() == 'nec.exe':
            return int(p[1])
    return None


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
subprocess.call([sys.executable, os.path.join(ROOT, 'tools', 'orig_play_mission.py'), '1', '--no-preview'],
                stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
time.sleep(0.5)
pid = find_pid()
if pid is None:
    raise SystemExit('no game')
hwnd = find_window(pid)
h = dm.k32.OpenProcess(0x1F0FFF, False, pid)
base = dm.module_base(h)
print('pid', pid, 'hwnd', hwnd, flush=True)


def u32(va):
    return struct.unpack('<I', dm.read(h, base, va, 4))[0]


def rd8(va):
    return dm.read(h, base, va, 1)[0]


def wr8(va, v):
    got = ctypes.c_size_t()
    dm.k32.WriteProcessMemory(h, ctypes.c_void_p(base + (va - 0x400000)), bytes([v & 0xFF]), 1, ctypes.byref(got))


def tap(vk, hold=0.10, gap=0.18):
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


def frame_rgb():
    pal = dm.read(h, base, 0x480f20, 4 + 256 * 4)
    fbp = u32(0x480f18)
    fb = dm.read_abs(h, fbp, 704 * 544)
    rgb = bytearray(768)
    for i in range(256):
        rgb[i * 3] = pal[4 + i * 4]
        rgb[i * 3 + 1] = pal[5 + i * 4]
        rgb[i * 3 + 2] = pal[6 + i * 4]
    im = Image.frombytes('P', (704, 544), fb).transpose(Image.FLIP_TOP_BOTTOM).crop((32, 32, 672, 512))
    im.putpalette(bytes(rgb))
    return im.convert('RGB')


# wait for map
t0 = time.time()
while time.time() - t0 < 40:
    wake()
    try:
        if u32(0x483270) == 0x416700:
            break
    except OSError:
        pass
    time.sleep(0.2)
print('map %.1fs' % (time.time() - t0), flush=True)
time.sleep(2.0)
frame_rgb().save(os.path.join(OUT, 'pre.png'))

# patch: player slot 8 -> S-61 at (4,7); enemy slot 2 -> EF-88 at (4,8)
wr8(0x4957a0 + 8, 0x80 | 6)
wr8(0x495ab0 + 8, 4)
wr8(0x495af0 + 8, 7)
wr8(0x4957a0 + 2, 3)
wr8(0x495ab0 + 2, 4)
wr8(0x495af0 + 2, 8)
# cursor on the player unit
wr8(0x481a72, 4)
wr8(0x481a71, 7)
time.sleep(0.5)
frame_rgb().save(os.path.join(OUT, 'patched.png'))
print('patched; cursor', rd8(0x481a72), rd8(0x481a71), flush=True)

sb = u32(0x480f14)
prev = None
changes = 0
shots = 0

def dump_state(n, tag, block):
    d = os.path.join(OUT, 'states', '%02d_%s' % (n, tag))
    os.makedirs(d, exist_ok=True)
    open(os.path.join(d, 'block.bin'), 'wb').write(block)
    open(os.path.join(d, 'atlas.bin'), 'wb').write(dm.read(h, base, 0x484730, 0x20000))
    open(os.path.join(d, 'spritebank.bin'), 'wb').write(dm.read_abs(h, sb, 0x100000))
    open(os.path.join(d, 'palette.bin'), 'wb').write(dm.read(h, base, 0x480f20, 4 + 256 * 4))
    open(os.path.join(d, 'units_type.bin'), 'wb').write(dm.read(h, base, 0x4957a0, 0x38))
    open(os.path.join(d, 'units_x.bin'), 'wb').write(dm.read(h, base, 0x495ab0, 0x38))
    open(os.path.join(d, 'units_y.bin'), 'wb').write(dm.read(h, base, 0x495af0, 0x38))
    frame_rgb().save(os.path.join(d, 'frame.png'))
    print('saved state', d, flush=True)

# attack: space -> down -> space -> space
for i in range(6):
    wake()
    tap(0x20, gap=0.5)
    frame_rgb().save(os.path.join(OUT, 'frames', 'f%02d.png' % i))
    time.sleep(0.4)

# continue advancing and watching
t1 = time.time()
while time.time() - t1 < 60 and changes < 16:
    wake()
    try:
        block = dm.read_abs(h, sb + 0x20000, 0x8000)
        state = u32(0x483270)
    except OSError:
        print('process gone', flush=True)
        break
    hh = hashlib.md5(block).hexdigest()[:8]
    if prev is None:
        prev = hh
        dump_state(changes, 'base_' + hh, block); changes += 1
    elif hh != prev:
        print('CHANGE %s -> %s state %#x' % (prev, hh, state), flush=True)
        dump_state(changes, hh, block); changes += 1; prev = hh
    if state != 0x416700 and time.time() - t1 > 3:
        tap(0x20, gap=0.1)
    if shots % 5 == 0:
        frame_rgb().save(os.path.join(OUT, 'frames', 'g%03d.png' % shots))
    shots += 1
    time.sleep(0.02)
print('changes', changes, flush=True)
dm.k32.CloseHandle(h)
