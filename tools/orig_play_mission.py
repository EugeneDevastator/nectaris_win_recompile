"""Load any campaign mission in the original 1997 Nec.exe and leave it running.

Usage:
    python tools/orig_play_mission.py 3        # mission 3 (1-based)
    python tools/orig_play_mission.py 16       # final mission
    python tools/orig_play_mission.py 3 --restart
    python tools/orig_play_mission.py --list

The script attaches to a running Nec.exe (or launches one), skips the intro,
patches the mission index (DAT_00495c52/DAT_00495c6a), and remote-calls the
original mission start routine FUN_00405ad0.  It then exits without touching the
game, so the loaded mission stays playable in its own window.
"""
import ctypes, ctypes.wintypes as wt, os, sys, time, struct, subprocess
sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
GAME_DIR = os.path.join(ROOT, 'game')
EXE = os.path.join(GAME_DIR, 'Nec.exe')
OUT = os.path.join(ROOT, 'dump', 'orig_maps')
MAP_STATE = 0x416700

MISSIONS = [
    'REVOLT', 'ICARUS', 'ARARAT', 'RHEA', 'LACUS', 'NEREID', 'KERES',
    'THULE', 'ORION', 'DARWIN', 'VESTA', 'AURORA', 'GANYMEDE', 'TETHYS',
    'PHOBOS', 'NECTOR',
]


def usage():
    print(__doc__.strip())
    print('\nCampaign missions:')
    for i, name in enumerate(MISSIONS, 1):
        print(f'  {i:2d}  {name}')
    print('  17..32  original editor/extra mission slots (if present)')


# ---- fake input proxy shared memory (same layout as the other tools) ----
K32 = ctypes.WinDLL('kernel32', use_last_error=True)
K32.CreateFileMappingW.restype = wt.HANDLE
K32.CreateFileMappingW.argtypes = [wt.HANDLE, ctypes.c_void_p, wt.DWORD, wt.DWORD, wt.DWORD, wt.LPCWSTR]
K32.MapViewOfFile.restype = ctypes.c_void_p
K32.MapViewOfFile.argtypes = [wt.HANDLE, wt.DWORD, wt.DWORD, wt.DWORD, ctypes.c_size_t]
hmap = K32.CreateFileMappingW(wt.HANDLE(-1).value, None, 4, 0, 4096, "Local\\NecFakeInput")
ptr = K32.MapViewOfFile(hmap, 0xF001F, 0, 0, 4096)


def i32(off, v=None):
    if v is None:
        return ctypes.c_long.from_address(ptr + off).value
    ctypes.c_long.from_address(ptr + off).value = v


def key(vk, down):
    ctypes.c_ubyte.from_address(ptr + 36 + vk).value = 1 if down else 0


i32(0, 0x4E454331)
i32(4, 1)
i32(12, 0)
i32(16, 0)
i32(28, 0)

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

kernel32 = ctypes.WinDLL('kernel32', use_last_error=True)
kernel32.VirtualAllocEx.restype = ctypes.c_void_p
kernel32.VirtualAllocEx.argtypes = [wt.HANDLE, ctypes.c_void_p, ctypes.c_size_t, wt.DWORD, wt.DWORD]
kernel32.WriteProcessMemory.argtypes = [wt.HANDLE, ctypes.c_void_p, ctypes.c_void_p,
                                        ctypes.c_size_t, ctypes.POINTER(ctypes.c_size_t)]
kernel32.CreateRemoteThread.restype = wt.HANDLE
kernel32.CreateRemoteThread.argtypes = [wt.HANDLE, ctypes.c_void_p, ctypes.c_size_t,
                                        ctypes.c_void_p, ctypes.c_void_p, wt.DWORD, ctypes.c_void_p]
kernel32.WaitForSingleObject.argtypes = [wt.HANDLE, wt.DWORD]


def find_pid():
    try:
        out = subprocess.check_output(
            ['tasklist', '/FI', 'IMAGENAME eq Nec.exe', '/FO', 'CSV', '/NH'],
            text=True)
    except Exception:
        return None
    for line in out.splitlines():
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
            if 'nectaris' in b.value.lower() or b.value:
                found.append(hwnd)
        return True

    U32.EnumWindows(cb, 0)
    return found[0] if found else None


def ensure_game():
    pid = find_pid()
    if pid is None:
        print('launching', EXE)
        subprocess.Popen([EXE], cwd=GAME_DIR)
        for _ in range(60):
            time.sleep(0.5)
            pid = find_pid()
            if pid is not None:
                break
    if pid is None:
        raise SystemExit('could not start Nec.exe')
    print('attached to pid', pid)
    return pid


def main():
    args = [a for a in sys.argv[1:]]
    if not args or '--help' in args or '-h' in args or '--list' in args:
        usage()
        return 0

    restart = '--restart' in args
    no_preview = '--no-preview' in args
    args = [a for a in args if not a.startswith('--')]
    if not args:
        usage()
        return 1
    try:
        number = int(args[0])
    except ValueError:
        print('mission number must be an integer')
        return 1
    if number < 1 or number > 32:
        print('mission number must be 1..32')
        return 1

    if restart:
        subprocess.run(['taskkill', '/IM', 'Nec.exe', '/F'],
                       stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        time.sleep(1.0)

    pid = ensure_game()
    hwnd = find_window(pid)
    for _ in range(40):
        if hwnd:
            break
        time.sleep(0.5)
        hwnd = find_window(pid)
    if not hwnd:
        raise SystemExit('game window not found')

    h = dm.k32.OpenProcess(0x1F0FFF, False, pid)
    if not h:
        raise SystemExit('OpenProcess failed (run as administrator?)')
    base = dm.module_base(h)

    def u32(va):
        return struct.unpack('<I', dm.read(h, base, va, 4))[0]

    def u8(va):
        return dm.read(h, base, va, 1)[0]

    def wr(va, data):
        got = ctypes.c_size_t()
        dm.k32.WriteProcessMemory(h, ctypes.c_void_p(base + (va - 0x400000)),
                                  data, len(data), ctypes.byref(got))

    def rd(va, n):
        return dm.read(h, base, va, n)

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

    def tap(vk, hold=0.10, gap=0.22):
        focus()
        sc = U32.MapVirtualKeyW(vk, 0) & 0xFF
        key(vk, 1)
        U32.PostMessageW(hwnd, 0x0100, vk, 1 | (sc << 16))
        U32.PostMessageW(hwnd, 0x0102, vk, 1 | (sc << 16))
        time.sleep(hold)
        key(vk, 0)
        U32.PostMessageW(hwnd, 0x0101, vk, 1 | (sc << 16) | 0xC0000000)
        time.sleep(gap)

    def is_map():
        return u32(0x483270) == MAP_STATE

    # Skip title/intro until the tactical map is active.
    for _ in range(80):
        if is_map():
            break
        tap(0x20)
    time.sleep(0.7)
    if not is_map():
        print('warning: map state not detected, trying the mission call anyway')

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
        kernel32.WriteProcessMemory(h, ctypes.c_void_p(mem), bytes(code),
                                    len(code), ctypes.byref(got))
        th = kernel32.CreateRemoteThread(h, None, 0, ctypes.c_void_p(mem),
                                         None, 0, None)
        kernel32.WaitForSingleObject(th, 5000)
        kernel32.CloseHandle(th)
        time.sleep(0.9)

    index = number - 1
    wr(0x495c52, bytes([index & 0xF]))
    wr(0x495c6a, bytes([index >> 4]))
    wr(0x42fb74, b'\x00')
    wr(0x42a4e0, b'\x00')
    rcall_va(0x405ad0, 5)
    time.sleep(0.8)

    name = MISSIONS[index] if index < len(MISSIONS) else f'mission {number}'
    print(f'loaded mission {number}: {name}')

    if not no_preview:
        try:
            os.makedirs(OUT, exist_ok=True)
            fbp = u32(0x480f18)
            fb = dm.read_abs(h, fbp, 704 * 544)
            pal = rd(0x480f20, 1028)
            rgb = bytearray(768)
            for i in range(256):
                rgb[i * 3] = pal[4 + i * 4]
                rgb[i * 3 + 1] = pal[5 + i * 4]
                rgb[i * 3 + 2] = pal[6 + i * 4]
            im = Image.frombytes('P', (704, 544), fb).transpose(Image.FLIP_TOP_BOTTOM).crop((32, 32, 672, 512))
            im.putpalette(bytes(rgb))
            path = os.path.join(OUT, f'play_mission_{number:02d}.png')
            im.convert('RGB').save(path)
            print('preview:', path)
        except Exception as e:
            print('preview failed:', e)

    focus()
    print('the original game is running; switch to its window and play.')
    dm.k32.CloseHandle(h)
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
