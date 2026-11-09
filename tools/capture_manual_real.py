"""Open MANUAL using a real mouse click, capturing the 8bpp framebuffer from memory."""
import ctypes, ctypes.wintypes as wt, os, sys, time, struct, subprocess
sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm
from PIL import Image, ImageChops

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
GAME = os.path.join(ROOT, 'game')
OUT = os.path.join(ROOT, 'dump', 'manual_real')
os.makedirs(OUT, exist_ok=True)
REF = Image.open(os.path.join(ROOT, 'dump', 'boot_mem', 'f_088.png')).convert('L')
REF_OPTS = REF.crop((100, 350, 560, 435))

K32 = ctypes.WinDLL('kernel32', use_last_error=True)
K32.CreateFileMappingW.restype = wt.HANDLE
K32.CreateFileMappingW.argtypes = [wt.HANDLE, ctypes.c_void_p, wt.DWORD, wt.DWORD, wt.DWORD, wt.LPCWSTR]
K32.MapViewOfFile.restype = ctypes.c_void_p
K32.MapViewOfFile.argtypes = [wt.HANDLE, wt.DWORD, wt.DWORD, wt.DWORD, ctypes.c_size_t]
hmap = K32.CreateFileMappingW(wt.HANDLE(-1).value, None, 4, 0, 4096, "Local\\NecFakeInput")
ptr = K32.MapViewOfFile(hmap, 0xF001F, 0, 0, 4096)
ctypes.memset(ptr, 0, 4096)
def i32(off, v=None):
    if v is None: return ctypes.c_long.from_address(ptr+off).value
    ctypes.c_long.from_address(ptr+off).value = v
def key(vk,d): ctypes.c_ubyte.from_address(ptr+36+vk).value = 1 if d else 0
i32(0,0x4E454331); i32(4,0); i32(12,0); i32(16,0); i32(28,0)
for i in range(256): key(i,0)

U32 = ctypes.WinDLL('user32', use_last_error=True); U32.SetProcessDPIAware()
G32 = ctypes.WinDLL('gdi32', use_last_error=True)
U32.EnumWindows.argtypes=[ctypes.WINFUNCTYPE(wt.BOOL,wt.HWND,wt.LPARAM),wt.LPARAM]
U32.GetWindowThreadProcessId.argtypes=[wt.HWND,ctypes.POINTER(wt.DWORD)]; U32.GetWindowTextW.argtypes=[wt.HWND,wt.LPWSTR,ctypes.c_int]
U32.GetClientRect.argtypes=[wt.HWND,ctypes.POINTER(wt.RECT)]; U32.ClientToScreen.argtypes=[wt.HWND,ctypes.POINTER(wt.POINT)]
U32.PostMessageW.argtypes=[wt.HWND,wt.UINT,wt.WPARAM,wt.LPARAM]; U32.MapVirtualKeyW.argtypes=[wt.UINT,wt.UINT]
U32.GetForegroundWindow.restype=wt.HWND; U32.AttachThreadInput.argtypes=[wt.DWORD,wt.DWORD,wt.BOOL]
U32.SetForegroundWindow.argtypes=[wt.HWND]; U32.BringWindowToTop.argtypes=[wt.HWND]; U32.ShowWindow.argtypes=[wt.HWND,ctypes.c_int]
U32.SetCursorPos.argtypes=[ctypes.c_int,ctypes.c_int]
K32.GetCurrentThreadId.restype=wt.DWORD
def find_window(pid):
    found=[]
    @ctypes.WINFUNCTYPE(wt.BOOL,wt.HWND,wt.LPARAM)
    def cb(hwnd,lp):
        p=wt.DWORD(); U32.GetWindowThreadProcessId(hwnd,ctypes.byref(p))
        if p.value==pid:
            b=ctypes.create_unicode_buffer(256); U32.GetWindowTextW(hwnd,b,256)
            if 'nectaris' in b.value.lower(): found.append(hwnd)
        return True
    U32.EnumWindows(cb,0); return found[0] if found else None
subprocess.run(['taskkill','/F','/IM','Nec.exe'],stdout=subprocess.DEVNULL,stderr=subprocess.DEVNULL); time.sleep(0.4)
p=subprocess.Popen([os.path.join(GAME,'Nec.exe')],cwd=GAME)
hwnd=None
for _ in range(200):
    hwnd=find_window(p.pid)
    if hwnd: break
    time.sleep(0.05)
print('hwnd',hwnd,'pid',p.pid,flush=True)
time.sleep(0.8)
h=dm.k32.OpenProcess(0x1F0FFF,False,p.pid); base=dm.module_base(h)
def u32(va): return struct.unpack('<I',dm.read(h,base,va,4))[0]
def frame():
    pal=dm.read(h,base,0x480f20,4+256*4); fb=dm.read_abs(h,u32(0x480f18),704*544); rgb=bytearray(768)
    for i in range(256):
        rgb[i*3]=pal[4+i*4]; rgb[i*3+1]=pal[5+i*4]; rgb[i*3+2]=pal[6+i*4]
    im=Image.frombytes('P',(704,544),fb).transpose(Image.FLIP_TOP_BOTTOM).crop((32,32,672,512)); im.putpalette(bytes(rgb)); return im.convert('RGB')
def tap(vk,hold=0.14,gap=0.35):
    sc=U32.MapVirtualKeyW(vk,0)&0xFF; key(vk,1)
    U32.PostMessageW(hwnd,0x0100,vk,1|(sc<<16)); U32.PostMessageW(hwnd,0x0102,vk,1|(sc<<16)); time.sleep(hold); key(vk,0)
    U32.PostMessageW(hwnd,0x0101,vk,1|(sc<<16)|0xC0000000); time.sleep(gap)
def is_menu(im):
    g=im.convert('L').crop((100,350,560,435)); d=ImageChops.difference(g,REF_OPTS); return sum(d.get_flattened_data())/(460*85)<10
def focus():
    fg=U32.GetForegroundWindow(); ft=U32.GetWindowThreadProcessId(fg,None); mt=K32.GetCurrentThreadId()
    U32.AttachThreadInput(mt,ft,True); U32.ShowWindow(hwnd,5); U32.BringWindowToTop(hwnd); U32.SetForegroundWindow(hwnd); U32.AttachThreadInput(mt,ft,False)
    U32.PostMessageW(hwnd,0x001C,1,0); U32.PostMessageW(hwnd,0x0006,1,0); U32.PostMessageW(hwnd,0x0007,0,0)
t0=time.time(); ok=False
while time.time()-t0<50:
    im=frame()
    if is_menu(im):
        im.save(os.path.join(OUT,'menu.png')); print('menu %.1f'%(time.time()-t0),flush=True); ok=True; break
    tap(0x20)
if not ok: print('menu not found',flush=True)
r=wt.RECT(); U32.GetClientRect(hwnd,ctypes.byref(r)); cw,ch=r.right,r.bottom; scale=cw/640.0
print('client',cw,ch,'scale',scale,flush=True)
focus(); time.sleep(0.8)
# click MANUAL
for pt in [(205,405),(210,410),(205,410)]:
    p2=wt.POINT(int(pt[0]*scale),int(pt[1]*scale)); U32.ClientToScreen(hwnd,ctypes.byref(p2))
    U32.SetCursorPos(p2.x,p2.y); time.sleep(0.3)
    U32.mouse_event(0x0002,0,0,0,0); time.sleep(0.15); U32.mouse_event(0x0004,0,0,0,0); time.sleep(0.8)
    frame().save(os.path.join(OUT,'after_click_%d_%d.png'%pt))
    print('clicked',pt,flush=True)
for i in range(40):
    time.sleep(0.35); frame().save(os.path.join(OUT,'manual_%03d.png'%i))
print('done',flush=True)
dm.k32.CloseHandle(h)
