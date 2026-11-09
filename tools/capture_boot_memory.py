"""Capture boot/intro/title screens from the game's 8bpp framebuffer in memory."""
import ctypes, ctypes.wintypes as wt, os, sys, time, struct, subprocess
sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
GAME = os.path.join(ROOT, 'game')
OUT = os.path.join(ROOT, 'dump', 'boot_mem')
os.makedirs(OUT, exist_ok=True)

# fake input shared memory
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


def main():
    subprocess.run(['taskkill', '/F', '/IM', 'Nec.exe'], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    time.sleep(0.4)
    p = subprocess.Popen([os.path.join(GAME, 'Nec.exe')], cwd=GAME)
    hwnd = None
    for _ in range(200):
        hwnd = find_window(p.pid)
        if hwnd:
            break
        time.sleep(0.05)
    print('hwnd', hwnd, 'pid', p.pid, flush=True)
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

    h = dm.k32.OpenProcess(0x1F0FFF, False, p.pid)
    base = dm.module_base(h)

    def u8(va):
        return dm.read(h, base, va, 1)[0]

    def u32(va):
        return struct.unpack('<I', dm.read(h, base, va, 4))[0]

    def tap(vk, hold=0.12, gap=0.35):
        sc = U32.MapVirtualKeyW(vk, 0) & 0xFF
        key(vk, 1)
        U32.PostMessageW(hwnd, 0x0100, vk, 1 | (sc << 16))
        U32.PostMessageW(hwnd, 0x0102, vk, 1 | (sc << 16))
        time.sleep(hold)
        key(vk, 0)
        U32.PostMessageW(hwnd, 0x0101, vk, 1 | (sc << 16) | 0xC0000000)
        time.sleep(gap)

    def capture(idx):
        pal = dm.read(h, base, 0x480f20, 4 + 256 * 4)
        fb = dm.read_abs(h, u32(0x480f18), 704 * 544)
        rgb = bytearray(768)
        for i in range(256):
            rgb[i * 3] = pal[4 + i * 4]
            rgb[i * 3 + 1] = pal[5 + i * 4]
            rgb[i * 3 + 2] = pal[6 + i * 4]
        im = Image.frombytes('P', (704, 544), fb).transpose(Image.FLIP_TOP_BOTTOM).crop((32, 32, 672, 512))
        im.putpalette(bytes(rgb))
        im.convert('RGB').save(os.path.join(OUT, 'f_%03d.png' % idx))
        print('capture %03d state=%#x map=%d,%d' %
              (idx, u32(0x483270), u8(0x495c02), u8(0x495c03)), flush=True)

    t0 = time.time()
    idx = 0
    next_capture = 0.0
    next_space = 15.0
    while time.time() - t0 < 35.0:
        now = time.time() - t0
        if now >= next_capture:
            capture(idx)
            idx += 1
            next_capture = now + 0.25
        if now >= next_space:
            tap(0x20)
            next_space = now + 5.0
        time.sleep(0.03)
    print('done; game left running', flush=True)
    dm.k32.CloseHandle(h)


if __name__ == '__main__':
    main()
