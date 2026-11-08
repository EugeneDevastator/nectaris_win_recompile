"""Drive the original into a battle with a fixed RNG byte, then dump the exact
combat state and the game's own intermediate damage registers.

Writes dump/combat_state.json for tools/check_combat_formula.py.

FUN_0040cb50 returns DAT_0048349f unchanged when it is non-zero, so writing
0x60 makes every roll deterministic: roll100 = (0x60*100)>>8 = 37, and
FUN_00421170 maps 37 to (150-37)/10 = 11 (1.1x damage).
"""
import ctypes, ctypes.wintypes as wt, os, sys, time, struct, subprocess, json
sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
GAME = os.path.join(ROOT, 'game')
OUT = os.path.join(ROOT, 'dump', 'combat_state.json')

# ------------------------------------------------------------------ fake input
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
i32(0, 0x4E454331); i32(4, 1); i32(12, 0); i32(16, 0); i32(28, 0)
for i in range(256):
    key(i, 0)

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
        p = wt.DWORD(); U32.GetWindowThreadProcessId(hwnd, ctypes.byref(p))
        if p.value == pid:
            b = ctypes.create_unicode_buffer(256); U32.GetWindowTextW(hwnd, b, 256)
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
if not hwnd:
    raise SystemExit('window not found')
print('pid', proc.pid, 'hwnd', hwnd, flush=True)

h = dm.k32.OpenProcess(0x1F0FFF, False, proc.pid)
base = dm.module_base(h)
dm.k32.WriteProcessMemory.argtypes = [wt.HANDLE, ctypes.c_void_p, ctypes.c_void_p,
                                      ctypes.c_size_t, ctypes.POINTER(ctypes.c_size_t)]
dm.k32.WriteProcessMemory.restype = wt.BOOL
def u8(va): return dm.read(h, base, va, 1)[0]
def u32(va): return struct.unpack('<I', dm.read(h, base, va, 4))[0]
def wr(va, data):
    got = ctypes.c_size_t()
    ok = dm.k32.WriteProcessMemory(h, ctypes.c_void_p(base + (va - 0x400000)),
                                   data, len(data), ctypes.byref(got))
    if not ok:
        raise OSError('WriteProcessMemory failed at %#x' % va)

def focus():
    fg = U32.GetForegroundWindow()
    ft = U32.GetWindowThreadProcessId(fg, None)
    mt = K32.GetCurrentThreadId()
    U32.AttachThreadInput(mt, ft, True)
    U32.ShowWindow(hwnd, 5); U32.BringWindowToTop(hwnd)
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

focus()
t0 = time.time()
toggle = False
while time.time() - t0 < 50:
    if u8(0x495c02) == 15 and u8(0x495c03) == 10:
        break
    tap(SPACE if not toggle else ENTER, gap=0.45)
    toggle = not toggle
print('map', u8(0x495c02), u8(0x495c03), flush=True)
time.sleep(3.0)

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
print('cursor', u8(0x481a72), u8(0x481a71), flush=True)

for vk in (SPACE, SPACE, RIGHT, RIGHT, SPACE, SPACE, RIGHT, DOWN):
    tap(vk, gap=0.40)
    print('  key %02x cursor %d,%d atk %d def %d mode %d' %
          (vk, u8(0x481a72), u8(0x481a71), u8(0x48327b), u8(0x483274), u8(0x481a68)),
          flush=True)
print('pre-battle atk', u8(0x48327b), 'def', u8(0x483274), flush=True)

# Force every FUN_0040c770 roll to 37 -> variance 11 (1.1x).
if os.environ.get('TRACE') == '1':
    focus()
    sc = U32.MapVirtualKeyW(SPACE, 0) & 0xFF
    key(SPACE, 1)
    U32.PostMessageW(hwnd, 0x0100, SPACE, 1 | (sc << 16))
    U32.PostMessageW(hwnd, 0x0102, SPACE, 1 | (sc << 16))
    for i in range(250):
        try:
            st = u32(0x483270)
            e52 = struct.unpack('<h', dm.read(h, base, 0x481e52, 2))[0]
            e60 = struct.unpack('<h', dm.read(h, base, 0x481e60, 2))[0]
            defv = u8(0x483274)
        except Exception as ex:
            st, e52, e60, defv = -1, -1, -1, -1
        print('tr %3d state=%08x def=%d e52=%d e60=%d rng=%02x' %
              (i, st, defv, e52, e60, u8(0x48349f)), flush=True)
        time.sleep(0.004)
    key(SPACE, 0)
    U32.PostMessageW(hwnd, 0x0101, SPACE, 1 | (sc << 16) | 0xC0000000)
    dm.k32.CloseHandle(h)
    proc.terminate()
    raise SystemExit(0)

if os.environ.get('NOFIX') != '1':
    wr(0x48349f, b'\x60')
for attempt in range(6):
    tap(SPACE, gap=0.55)
    if (u8(0x483274) & 0x3F) != 0:
        break
    print('  battle retry', attempt, u8(0x48327b), u8(0x483274), flush=True)
time.sleep(1.0)

def rd(va, n):
    return list(dm.read(h, base, va, n))

def byte(va):
    return u8(va)

state = {
    'map_w': byte(0x495c02),
    'map_h': byte(0x495c03),
    'current_player': byte(0x495c63),
    'atk_slot': byte(0x48327b),
    'def_slot': byte(0x483274) & 0x3F,
    'atk_type': byte(0x48327c),
    'mode': byte(0x481dbc),
    'fixed_rng': byte(0x48349f),
    'types': rd(0x4957a0, 56),
    'hp_rank': rd(0x4957e0, 56),
    'xs': rd(0x495ab0, 56),
    'ys': rd(0x495af0, 56),
    'occupancy': rd(0x495820, 0x580),
    'terrain': rd(0x481a80, 0x580),
    # game intermediates
    'e5f': byte(0x481e5f), 'e5e': byte(0x481e5e),
    'e4c': struct.unpack('<h', dm.read(h, base, 0x481e4c, 2))[0],
    'e50': struct.unpack('<h', dm.read(h, base, 0x481e50, 2))[0],
    'e48': struct.unpack('<h', dm.read(h, base, 0x481e48, 2))[0],
    'e4e': struct.unpack('<h', dm.read(h, base, 0x481e4e, 2))[0],
    'e55': byte(0x481e55), 'e5a': byte(0x481e5a),
    'e58': struct.unpack('<h', dm.read(h, base, 0x481e58, 2))[0],
    'e5c': struct.unpack('<h', dm.read(h, base, 0x481e5c, 2))[0],
    'e4a': byte(0x481e4a),
    'e52': struct.unpack('<h', dm.read(h, base, 0x481e52, 2))[0],
    'e60': struct.unpack('<h', dm.read(h, base, 0x481e60, 2))[0],
    'db6': byte(0x481db6), 'db2': byte(0x481db2),
    'db3': byte(0x481db3), 'db4': byte(0x481db4),
}
open(OUT, 'w', encoding='utf-8').write(json.dumps(state, indent=1))
print('wrote', OUT)
print('atk', state['atk_slot'], 'def', state['def_slot'], 'mode', state['mode'],
      'e5f/e5e', state['e5f'], state['e5e'])
print('e4c/e50', state['e4c'], state['e50'], 'e48/e4e', state['e48'], state['e4e'],
      'e55/e5a', state['e55'], state['e5a'], 'e58/e5c', state['e58'], state['e5c'])
print('e4a', state['e4a'], 'e52/e60', state['e52'], state['e60'],
      'db6/db2', state['db6'], state['db2'])

dm.k32.CloseHandle(h)
try:
    proc.terminate()
except Exception:
    pass
