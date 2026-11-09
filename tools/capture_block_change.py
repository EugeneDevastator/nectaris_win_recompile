"""Capture full memory state at the moment the combat sprite block changes.

Runs mission 6 normally, drives turns so the AI attacks, polls spritebank
slots 128..159, and on every content change dumps:
  spritebank (1 MB), atlas (128 KB), tilebank (512 KB), palette,
  framebuffer, unit arrays and the current frame.
"""
import ctypes, ctypes.wintypes as wt, os, sys, time, struct, subprocess, hashlib
sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
GAME = os.path.join(ROOT, 'game')
OUT = os.path.join(ROOT, 'dump', 'block_changes')
os.makedirs(OUT, exist_ok=True)
MAP_STATE = 0x416700

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


if not os.environ.get('NO_LAUNCH'):
    subprocess.run(['taskkill', '/F', '/IM', 'Nec.exe'], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    time.sleep(0.4)
    subprocess.call([sys.executable, os.path.join(ROOT, 'tools', 'orig_play_mission.py'), '6', '--no-preview'],
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


def tap(vk, hold=0.10, gap=0.15):
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


def dump_state(n, tag, sb, block):
    d = os.path.join(OUT, '%02d_%s' % (n, tag))
    os.makedirs(d, exist_ok=True)
    open(os.path.join(d, 'block.bin'), 'wb').write(block)
    open(os.path.join(d, 'spritebank.bin'), 'wb').write(dm.read_abs(h, sb, 0x100000))
    open(os.path.join(d, 'atlas.bin'), 'wb').write(dm.read(h, base, 0x484730, 0x20000))
    tb = u32(0x481500)
    if tb:
        open(os.path.join(d, 'tilebank.bin'), 'wb').write(dm.read_abs(h, tb, 0x80000))
    open(os.path.join(d, 'palette.bin'), 'wb').write(dm.read(h, base, 0x480f20, 4 + 256 * 4))
    for name, va, size in [('units_type', 0x4957a0, 0x38), ('units_hp', 0x4957e0, 0x38),
                           ('units_x', 0x495ab0, 0x38), ('units_y', 0x495af0, 0x38)]:
        open(os.path.join(d, name + '.bin'), 'wb').write(dm.read(h, base, va, size))
    frame_rgb().save(os.path.join(d, 'frame.png'))
    print('saved', d, tag, flush=True)


# wait for the map
t0 = time.time()
while time.time() - t0 < 40:
    wake()
    try:
        if u32(0x483270) == MAP_STATE:
            break
    except OSError:
        pass
    time.sleep(0.2)
print('map ready %.1fs' % (time.time() - t0), flush=True)
time.sleep(2.0)

pt = os.environ.get('PATCH_TYPE')
if pt is not None:
    typ = int(pt, 0)
    to = dm.read(h, base, 0x4957a0, 0x38)
    got = ctypes.c_size_t()
    n = 0
    for i, t in enumerate(to):
        if t & 0x80:
            dm.k32.WriteProcessMemory(h, ctypes.c_void_p(base + (0x4957a0 + i - 0x400000)),
                                      bytes([(t & 0x80) | (typ & 0x1F)]), 1, ctypes.byref(got))
            n += 1
    print('patched %d player units to type %d' % (n, typ), flush=True)

et = os.environ.get('PATCH_ENEMY_TYPE')
if et is not None:
    typ = int(et, 0)
    to = dm.read(h, base, 0x4957a0, 0x38)
    got = ctypes.c_size_t()
    n = 0
    for i, t in enumerate(to):
        if (t & 0x1F) != 0x1F and t and not (t & 0x80):
            dm.k32.WriteProcessMemory(h, ctypes.c_void_p(base + (0x4957a0 + i - 0x400000)),
                                      bytes([(t & 0x80) | (typ & 0x1F)]), 1, ctypes.byref(got))
            n += 1
    print('patched %d enemy units to type %d' % (n, typ), flush=True)

sb = u32(0x480f14)
print('spritebank', hex(sb), flush=True)

prev = None
changes = 0
last_skip = 0.0
last_space = 0.0
t1 = time.time()
while time.time() - t1 < 170 and changes < 12:
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
        dump_state(changes, 'base_' + hh, sb, block)
        changes += 1
    elif hh != prev:
        print('CHANGE %s -> %s state %#x' % (prev, hh, state), flush=True)
        dump_state(changes, hh, sb, block)
        changes += 1
        prev = hh
    now = time.time()
    if state == MAP_STATE:
        if now - last_skip > 6.0:
            for vk in (0x28, 0x28, 0x28, 0x20):
                tap(vk, gap=0.10)
            last_skip = now
    else:
        if now - last_space > 0.8:
            tap(0x20, gap=0.05)
            last_space = now
    time.sleep(0.02)
print('changes', changes, flush=True)
dm.k32.CloseHandle(h)
