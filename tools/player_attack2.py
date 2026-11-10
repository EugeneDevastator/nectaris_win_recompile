"""Mission 1 player attack: exit action selection, select our unit, attack."""
import ctypes, ctypes.wintypes as wt, os, sys, time, struct, subprocess
sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
OUT = os.path.join(ROOT, 'dump', 'attack2')
os.makedirs(OUT, exist_ok=True)
TYP = int(sys.argv[1]) if len(sys.argv) > 1 else 18

dm.k32.VirtualQueryEx.argtypes = [wt.HANDLE, ctypes.c_void_p, ctypes.c_void_p, ctypes.c_size_t]


class MBI(ctypes.Structure):
    _fields_ = [('BaseAddress', ctypes.c_void_p), ('AllocationBase', ctypes.c_void_p),
                ('AllocationProtect', wt.DWORD), ('PartitionId', wt.WORD),
                ('RegionSize', ctypes.c_size_t), ('State', wt.DWORD),
                ('Protect', wt.DWORD), ('Type', wt.DWORD)]


def scan_memory(h, sig, maxregions=500):
    out = []
    addr = 0x10000
    seen = 0
    while addr < 0x7fff0000 and seen < maxregions:
        mbi = MBI()
        if not dm.k32.VirtualQueryEx(h, ctypes.c_void_p(addr), ctypes.byref(mbi), ctypes.sizeof(mbi)):
            break
        base = mbi.BaseAddress or 0
        size = mbi.RegionSize
        if mbi.State == 0x1000 and size >= 0x8000 and (mbi.Protect & 0xFF) not in (0x01, 0x00):
            seen += 1
            try:
                data = dm.read_abs(h, base, min(size, 0x800000))
            except OSError:
                data = b''
            p = data.find(sig)
            if p >= 0:
                out.append((base, p, size, len(data)))
        nxt = base + size
        if nxt <= addr:
            break
        addr = nxt
    return out

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
time.sleep(0.3)
subprocess.call([sys.executable, os.path.join(ROOT, 'tools', 'orig_play_mission.py'), '1', '--no-preview'],
                stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
time.sleep(0.5)
pid = find_pid()
hwnd = find_window(pid)
h = dm.k32.OpenProcess(0x1F0FFF, False, pid)
base = dm.module_base(h)


def u32(va):
    return struct.unpack('<I', dm.read(h, base, va, 4))[0]


def rd8(va):
    return dm.read(h, base, va, 1)[0]


def wr8(va, v):
    got = ctypes.c_size_t()
    dm.k32.WriteProcessMemory(h, ctypes.c_void_p(base + (va - 0x400000)), bytes([v & 0xFF]), 1, ctypes.byref(got))


def tap(vk, hold=0.12, gap=0.25):
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


def frame(name):
    pal = dm.read(h, base, 0x480f20, 4 + 256 * 4)
    fb = dm.read_abs(h, u32(0x480f18), 704 * 544)
    rgb = bytearray(768)
    for i in range(256):
        rgb[i * 3] = pal[4 + i * 4]
        rgb[i * 3 + 1] = pal[5 + i * 4]
        rgb[i * 3 + 2] = pal[6 + i * 4]
    im = Image.frombytes('P', (704, 544), fb).transpose(Image.FLIP_TOP_BOTTOM).crop((32, 32, 672, 512))
    im.putpalette(bytes(rgb))
    im.convert('RGB').save(os.path.join(OUT, name))
    print('frame', name, 'state', hex(u32(0x483270)), 'cursor', rd8(0x481a72), rd8(0x481a71), flush=True)


t0 = time.time()
while time.time() - t0 < 30:
    wake()
    if u32(0x483270) == 0x416700:
        break
    time.sleep(0.15)
time.sleep(2.5)
frame('00_boot.png')
# exit action selection
tap(0x20, gap=0.6)
frame('01_exit_action.png')

# relocate green slot 5 (10,2) next to blue slot 2 (4,8)
gx, gy = rd8(0x495ab0 + 5) & 31, rd8(0x495af0 + 5) & 31
wr8(0x495ab0 + 5, 4); wr8(0x495af0 + 5, 7)
wr8(0x495820 + gy * 32 + gx, 0)
wr8(0x495820 + 7 * 32 + 4, 0x80 | 5)
# blue slot 2 becomes the desired attacker type
wr8(0x4957a0 + 2, TYP & 31)
frame('02_patched.png')

# move cursor to the blue attacker at (4,8)
for _ in range(40):
    x, y = rd8(0x481a72), rd8(0x481a71)
    if x == 4 and y == 8:
        break
    if x < 4:
        tap(0x27, gap=0.12)
    elif x > 4:
        tap(0x25, gap=0.12)
    elif y < 8:
        tap(0x28, gap=0.12)
    elif y > 8:
        tap(0x26, gap=0.12)
frame('03_cursor.png')

# open menu, choose attack, confirm, choose the target above
tap(0x20, gap=0.5); frame('04_menu.png')
tap(0x28, gap=0.4); frame('05_attack_sel.png')
tap(0x20, gap=0.5); frame('06_target.png')
tap(0x26, gap=0.4); frame('07_target_up.png')
tap(0x20, gap=0.6); frame('08_confirm.png')

# poll immediately for the generated combat state (base is 21722e92)
sb = u32(0x480f14)
import hashlib
BASE = '21722e92'
captured = False
for i in range(2000):
    wake()
    try:
        block = dm.read_abs(h, sb + 0x20000, 0x8000)
        state = u32(0x483270)
    except OSError:
        break
    hh = hashlib.md5(block).hexdigest()[:8]
    if hh != BASE:
        d = os.path.join(OUT, 'capture_%s' % hh)
        os.makedirs(d, exist_ok=True)
        open(os.path.join(d, 'block.bin'), 'wb').write(block)
        open(os.path.join(d, 'spritebank.bin'), 'wb').write(dm.read_abs(h, sb, 0x100000))
        open(os.path.join(d, 'atlas.bin'), 'wb').write(dm.read(h, base, 0x484730, 0x20000))
        tb = u32(0x481500)
        if tb:
            open(os.path.join(d, 'tilebank.bin'), 'wb').write(dm.read_abs(h, tb, 0x80000))
        frame('09_combat_%s.png' % hh)
        print('CAPTURED', hh, flush=True)
        # save palette now and again once units have materialised
        open(os.path.join(d, 'palette_at_change.bin'), 'wb').write(dm.read(h, base, 0x480f20, 4 + 256 * 4))
        time.sleep(2.5)
        try:
            pal2 = dm.read(h, base, 0x480f20, 4 + 256 * 4)
            open(os.path.join(OUT, 'palette_stable.bin'), 'wb').write(pal2)
            pal = pal2
            fb = dm.read_abs(h, u32(0x480f18), 704 * 544)
            rgb = bytearray(768)
            for i in range(256):
                rgb[i * 3] = pal[4 + i * 4]
                rgb[i * 3 + 1] = pal[5 + i * 4]
                rgb[i * 3 + 2] = pal[6 + i * 4]
            im = Image.frombytes('P', (704, 544), fb).transpose(Image.FLIP_TOP_BOTTOM).crop((32, 32, 672, 512))
            im.putpalette(bytes(rgb))
            im.convert('RGB').save(os.path.join(OUT, '10_stable.png'))
        except OSError:
            pass
        captured = True
        break
    time.sleep(0.01)
if not captured:
    print('no generated state captured', flush=True)
print('done', flush=True)
dm.k32.CloseHandle(h)
