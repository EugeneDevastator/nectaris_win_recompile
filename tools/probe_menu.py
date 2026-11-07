"""Probe the live tactical-map command menu and capture framebuffer frames."""
import os, sys, time, struct, ctypes
sys.path.insert(0, os.path.dirname(__file__))
sys.path.insert(0, os.path.join(os.path.dirname(__file__), 'fakeinput'))
import dump_mem as dm
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
OUT = os.path.join(ROOT, 'ref', 'probe')
os.makedirs(OUT, exist_ok=True)

K32 = ctypes.WinDLL('kernel32', use_last_error=True)
K32.CreateFileMappingW.restype = ctypes.wintypes.HANDLE
K32.CreateFileMappingW.argtypes = [ctypes.wintypes.HANDLE, ctypes.c_void_p, ctypes.wintypes.DWORD,
                                   ctypes.wintypes.DWORD, ctypes.wintypes.DWORD, ctypes.wintypes.LPCWSTR]
K32.MapViewOfFile.restype = ctypes.c_void_p
K32.MapViewOfFile.argtypes = [ctypes.wintypes.HANDLE, ctypes.wintypes.DWORD, ctypes.wintypes.DWORD,
                              ctypes.wintypes.DWORD, ctypes.c_size_t]
hmap = K32.CreateFileMappingW(ctypes.wintypes.HANDLE(-1).value, None, 4, 0, 4096, "Local\\NecFakeInput")
ptr = K32.MapViewOfFile(hmap, 0xF001F, 0, 0, 4096)

def set_i32(off, v): ctypes.c_long.from_address(ptr + off).value = v
def set_key(vk, d): ctypes.c_ubyte.from_address(ptr + 36 + vk).value = 1 if d else 0
set_i32(4, 1)
for i in range(256): set_key(i, 0)
set_i32(28, 0)

# find game window/pid
import subprocess
out = subprocess.check_output(['tasklist', '/FI', 'IMAGENAME eq Nec.exe', '/FO', 'CSV', '/NH'], text=True)
pid = None
for line in out.splitlines():
    parts = [p.strip('"') for p in line.split('","')]
    if parts and parts[0].lower() == 'nec.exe':
        pid = int(parts[1])
print('pid', pid)
U32 = ctypes.WinDLL('user32', use_last_error=True)
U32.EnumWindows.argtypes = [ctypes.WINFUNCTYPE(ctypes.wintypes.BOOL, ctypes.wintypes.HWND, ctypes.wintypes.LPARAM), ctypes.wintypes.LPARAM]
U32.GetWindowThreadProcessId.argtypes = [ctypes.wintypes.HWND, ctypes.POINTER(ctypes.wintypes.DWORD)]
U32.GetWindowTextW.argtypes = [ctypes.wintypes.HWND, ctypes.wintypes.LPWSTR, ctypes.c_int]
U32.PostMessageW.argtypes = [ctypes.wintypes.HWND, ctypes.wintypes.UINT, ctypes.wintypes.WPARAM, ctypes.wintypes.LPARAM]
U32.MapVirtualKeyW.argtypes = [ctypes.wintypes.UINT, ctypes.wintypes.UINT]
found = []
@ctypes.WINFUNCTYPE(ctypes.wintypes.BOOL, ctypes.wintypes.HWND, ctypes.wintypes.LPARAM)
def cb(hwnd, lp):
    p = ctypes.wintypes.DWORD()
    U32.GetWindowThreadProcessId(hwnd, ctypes.byref(p))
    if p.value == pid:
        buf = ctypes.create_unicode_buffer(256)
        U32.GetWindowTextW(hwnd, buf, 256)
        if 'nectaris' in buf.value.lower(): found.append(hwnd)
    return True
U32.EnumWindows(cb, 0)
hwnd = found[0]
# tell the game it is active/focused without stealing real focus
U32.PostMessageW(hwnd, 0x001C, 1, 0)   # WM_ACTIVATEAPP
U32.PostMessageW(hwnd, 0x0006, 1, 0)   # WM_ACTIVATE (WA_ACTIVE)
U32.PostMessageW(hwnd, 0x0007, 0, 0)   # WM_SETFOCUS
time.sleep(0.2)

h = dm.k32.OpenProcess(0x1F0FFF, False, pid)
base = dm.module_base(h)
def u8(va): return dm.read(h, base, va, 1)[0]
def u32(va): return struct.unpack('<I', dm.read(h, base, va, 4))[0]
pal = open(os.path.join(ROOT, 'dump', 'palette_bgr.bin'), 'rb').read()
PALR = bytearray(768)
for i in range(256):
    PALR[i*3]=pal[i*4+2]; PALR[i*3+1]=pal[i*4+1]; PALR[i*3+2]=pal[i*4]

def shot(name):
    sp = u32(0x480f18)
    data = dm.read_abs(h, sp, 704*544)
    img = Image.frombuffer('P', (704,544), data, 'raw','P',0,1)
    img = img.transpose(Image.FLIP_TOP_BOTTOM).crop((32,32,672,512))
    img.putpalette(bytes(PALR))
    img.convert('RGB').save(os.path.join(OUT, name+'.png'))
    print(name, 'atk', u8(0x48327b), 'def', u8(0x483274), 'ind', u8(0x481dbc),
          '0x481a68', u8(0x481a68), '0x483276', u8(0x483276))

def tap(vk, hold=0.14):
    sc = U32.MapVirtualKeyW(vk, 0) & 0xFF
    set_key(vk, 1)
    U32.PostMessageW(hwnd, 0x0100, vk, 1 | (sc << 16))
    U32.PostMessageW(hwnd, 0x0102, vk, 1 | (sc << 16))
    time.sleep(hold)
    set_key(vk, 0)
    U32.PostMessageW(hwnd, 0x0101, vk, 1 | (sc << 16) | 0xC0000000)
    time.sleep(0.25)

def click(cx, cy):
    set_i32(20, cx); set_i32(24, cy)
    lp = ((cy & 0xFFFF) << 16) | (cx & 0xFFFF)
    U32.PostMessageW(hwnd, 0x0200, 0, lp)
    set_i32(28, 1)
    U32.PostMessageW(hwnd, 0x0201, 1, lp)
    time.sleep(0.15)
    set_i32(28, 0)
    U32.PostMessageW(hwnd, 0x0202, 0, lp)
    time.sleep(0.3)

time.sleep(0.3)
shot('00_base')
tap(0x0D)          # Enter
shot('01_enter')
tap(0x28)          # Down
shot('02_down')
tap(0x28)          # Down
shot('03_down2')
tap(0x0D)          # Enter
shot('04_enter2')
tap(0x27)          # Right
shot('05_right')
tap(0x0D)
shot('06_enter3')
tap(0x20)          # Space
shot('07_space')
dm.k32.CloseHandle(h)
print('done')
