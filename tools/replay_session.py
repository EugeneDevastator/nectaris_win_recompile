"""Replay a recorded session against a fresh Nec.exe, then capture the battle.

Reads ref/session/input.jsonl, drives the winmm proxy's shared input state, and
also posts the matching window messages.  During t=50..68 it snapshots the game's
8bpp framebuffer so the battle is captured palette-perfect.
"""
import ctypes, ctypes.wintypes as wt, os, sys, time, json, struct
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
GAME = os.path.join(ROOT, 'game')
OUT = os.path.join(ROOT, 'ref', 'replay')
os.makedirs(OUT, exist_ok=True)
sys.path.insert(0, os.path.join(ROOT, 'tools'))
import dump_mem as dm

CAP_FROM, CAP_TO = 58.0, 88.0
UNTIL = 90.0

# ---------- shared input ----------
K32 = ctypes.WinDLL('kernel32', use_last_error=True)
K32.CreateFileMappingW.restype = wt.HANDLE
K32.CreateFileMappingW.argtypes = [wt.HANDLE, ctypes.c_void_p, wt.DWORD, wt.DWORD, wt.DWORD, wt.LPCWSTR]
K32.MapViewOfFile.restype = ctypes.c_void_p
K32.MapViewOfFile.argtypes = [wt.HANDLE, wt.DWORD, wt.DWORD, wt.DWORD, ctypes.c_size_t]
hmap = K32.CreateFileMappingW(wt.HANDLE(-1).value, None, 0x04, 0, 4096, "Local\\NecFakeInput")
ptr = K32.MapViewOfFile(hmap, 0xF001F, 0, 0, 4096)
ctypes.memset(ptr, 0, 4096)

MAGIC = 0x4E454331
def set_i32(off, v):
    ctypes.c_long.from_address(ptr + off).value = v
def get_i32(off):
    return ctypes.c_long.from_address(ptr + off).value
def set_key(vk, down):
    ctypes.c_ubyte.from_address(ptr + 36 + vk).value = 1 if down else 0

set_i32(0, MAGIC)
set_i32(4, 1)      # enabled

U32 = ctypes.WinDLL('user32', use_last_error=True)
U32.EnumWindows.argtypes = [ctypes.WINFUNCTYPE(wt.BOOL, wt.HWND, wt.LPARAM), wt.LPARAM]
U32.GetWindowThreadProcessId.argtypes = [wt.HWND, ctypes.POINTER(wt.DWORD)]
U32.GetWindowTextW.argtypes = [wt.HWND, wt.LPWSTR, ctypes.c_int]
U32.ClientToScreen.argtypes = [wt.HWND, ctypes.POINTER(wt.POINT)]
U32.SetForegroundWindow.argtypes = [wt.HWND]
U32.PostMessageW.argtypes = [wt.HWND, wt.UINT, wt.WPARAM, wt.LPARAM]
U32.MapVirtualKeyW.argtypes = [wt.UINT, wt.UINT]


def find_window(pid):
    found = []
    @ctypes.WINFUNCTYPE(wt.BOOL, wt.HWND, wt.LPARAM)
    def cb(hwnd, lp):
        p = wt.DWORD()
        U32.GetWindowThreadProcessId(hwnd, ctypes.byref(p))
        if p.value == pid:
            buf = ctypes.create_unicode_buffer(256)
            U32.GetWindowTextW(hwnd, buf, 256)
            if 'nectaris' in buf.value.lower():
                found.append(hwnd)
        return True
    U32.EnumWindows(cb, 0)
    return found[0] if found else None


def load_events():
    ev = []
    for line in open(os.path.join(ROOT, 'ref', 'session', 'input.jsonl'), encoding='utf-8'):
        try:
            e = json.loads(line)
        except Exception:
            continue
        if e.get('ev') in ('move', 'mdown', 'mup', 'kdown', 'kup'):
            ev.append(e)
    ev.sort(key=lambda e: e['t'])
    return ev


def main():
    import subprocess
    ev = load_events()
    print('replay events', len(ev), 'duration', ev[-1]['t'], flush=True)
    p = subprocess.Popen([os.path.join(GAME, 'Nec.exe')], cwd=GAME)
    hwnd = None
    for _ in range(200):
        hwnd = find_window(p.pid)
        if hwnd:
            break
        time.sleep(0.05)
    if not hwnd:
        print('no window', flush=True)
        return
    time.sleep(0.5)
    U32.SetForegroundWindow(hwnd)
    org = wt.POINT(0, 0)
    U32.ClientToScreen(hwnd, ctypes.byref(org))
    print('window', hwnd, 'origin', org.x, org.y, flush=True)

    h = dm.k32.OpenProcess(0x1F0FFF, False, p.pid)
    base = dm.module_base(h)
    def u8(va):
        return dm.read(h, base, va, 1)[0]
    def u32(va):
        return struct.unpack('<I', dm.read(h, base, va, 4))[0]

    pal = open(os.path.join(ROOT, 'dump', 'palette_bgr.bin'), 'rb').read()
    PALR = bytearray(768)
    for i in range(256):
        PALR[i*3+0] = pal[i*4+2]; PALR[i*3+1] = pal[i*4+1]; PALR[i*3+2] = pal[i*4]
    PALR = bytes(PALR)

    def fbshot(path):
        sp = u32(0x480f18)
        if not (0x10000 < sp < 0x7fffffff):
            return False
        data = dm.read_abs(h, sp, 704*544)
        img = Image.frombuffer('P', (704, 544), data, 'raw', 'P', 0, 1)
        img = img.transpose(Image.FLIP_TOP_BOTTOM).crop((32, 32, 672, 512))
        img.putpalette(PALR)
        img.convert('RGB').save(path)
        return True

    def post_key(vk, down):
        sc = U32.MapVirtualKeyW(vk, 0) & 0xFF
        if down:
            U32.PostMessageW(hwnd, 0x0100, vk, 1 | (sc << 16))
            U32.PostMessageW(hwnd, 0x0102, vk, 1 | (sc << 16))
        else:
            U32.PostMessageW(hwnd, 0x0101, vk, 1 | (sc << 16) | 0xC0000000)

    def post_mouse(cx, cy, down=None):
        lp = ((cy & 0xFFFF) << 16) | (cx & 0xFFFF)
        U32.PostMessageW(hwnd, 0x0200, 0, lp)
        if down is True:
            U32.PostMessageW(hwnd, 0x0201, 1, lp)
        elif down is False:
            U32.PostMessageW(hwnd, 0x0202, 0, lp)

    last_cx = last_cy = 0
    buttons = 0
    pressed = set()
    t0 = time.monotonic()
    idx = 0
    cap_i = 0
    last_cap = 0.0
    last_report = -1

    while True:
        now = time.monotonic() - t0
        if now > UNTIL:
            break
        while idx < len(ev) and ev[idx]['t'] <= now:
            e = ev[idx]; idx += 1
            k = e['ev']
            if k in ('move', 'mdown', 'mup'):
                cx, cy = e.get('cx', 0), e.get('cy', 0)
                if cx < 0 or cy < 0:
                    continue
                last_cx, last_cy = cx, cy
                org = wt.POINT(0, 0); U32.ClientToScreen(hwnd, ctypes.byref(org))
                set_i32(12, org.x + cx); set_i32(16, org.y + cy)
                set_i32(20, cx); set_i32(24, cy)
                if k == 'mdown':
                    buttons |= 1; set_i32(28, buttons); post_mouse(cx, cy, True)
                elif k == 'mup':
                    buttons &= ~1; set_i32(28, buttons); post_mouse(cx, cy, False)
                else:
                    post_mouse(cx, cy)
            else:
                vk = e.get('vk', 0)
                down = (k == 'kdown')
                set_key(vk, down)
                if down:
                    pressed.add(vk)
                else:
                    pressed.discard(vk)
                post_key(vk, down)
        # capture framebuffer during the expected battle window
        if CAP_FROM <= now <= CAP_TO and now - last_cap > 0.09:
            try:
                fbshot(os.path.join(OUT, f'fb_{cap_i:04d}.png'))
                cap_i += 1
            except Exception as ex:
                print('fb err', ex, flush=True)
            last_cap = now
        if int(now) != last_report:
            last_report = int(now)
            print('t=%.1f atk=%d def=%d ind=%d task=%x' % (
                now, u8(0x48327b), u8(0x483274), u8(0x481dbc), u32(0x482330)), flush=True)
        time.sleep(0.01)

    try:
        dm.k32.CloseHandle(h)
    except Exception:
        pass
    print('replay done, captured', cap_i, 'fb frames', flush=True)
    # leave the game running


if __name__ == '__main__':
    main()
