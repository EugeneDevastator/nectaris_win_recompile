import ctypes, ctypes.wintypes as wt, os, sys, time, struct, subprocess
sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm

K32 = ctypes.WinDLL('kernel32', use_last_error=True)
K32.CreateFileMappingW.restype = wt.HANDLE
K32.CreateFileMappingW.argtypes = [wt.HANDLE, ctypes.c_void_p, wt.DWORD, wt.DWORD, wt.DWORD, wt.LPCWSTR]
K32.MapViewOfFile.restype = ctypes.c_void_p
K32.MapViewOfFile.argtypes = [wt.HANDLE, wt.DWORD, wt.DWORD, wt.DWORD, ctypes.c_size_t]
hmap = K32.CreateFileMappingW(wt.HANDLE(-1).value, None, 4, 0, 4096, "Local\\NecFakeInput")
ptr = K32.MapViewOfFile(hmap, 0xF001F, 0, 0, 4096)
def i32(off): return ctypes.c_long.from_address(ptr+off).value

U32 = ctypes.WinDLL('user32', use_last_error=True)
U32.EnumWindows.argtypes = [ctypes.WINFUNCTYPE(wt.BOOL, wt.HWND, wt.LPARAM), wt.LPARAM]
U32.EnumChildWindows.argtypes = [wt.HWND, ctypes.WINFUNCTYPE(wt.BOOL, wt.HWND, wt.LPARAM), wt.LPARAM]
U32.GetWindowThreadProcessId.argtypes = [wt.HWND, ctypes.POINTER(wt.DWORD)]
U32.GetWindowTextW.argtypes = [wt.HWND, wt.LPWSTR, ctypes.c_int]
U32.GetClassNameW.argtypes = [wt.HWND, wt.LPWSTR, ctypes.c_int]
U32.GetForegroundWindow.restype = wt.HWND
U32.GetFocus.restype = wt.HWND
U32.SetForegroundWindow.argtypes = [wt.HWND]

out = subprocess.check_output(['tasklist','/FI','IMAGENAME eq Nec.exe','/FO','CSV','/NH'], text=True)
pid=None
for line in out.splitlines():
    p=[x.strip('"') for x in line.split('","')]
    if p and p[0].lower()=='nec.exe': pid=int(p[1])
print('pid',pid)
top=[]
@ctypes.WINFUNCTYPE(wt.BOOL, wt.HWND, wt.LPARAM)
def cb(hwnd, lp):
    p=wt.DWORD(); U32.GetWindowThreadProcessId(hwnd, ctypes.byref(p))
    if p.value==pid:
        b=ctypes.create_unicode_buffer(256); U32.GetWindowTextW(hwnd,b,256)
        c=ctypes.create_unicode_buffer(256); U32.GetClassNameW(hwnd,c,256)
        top.append((hwnd,b.value,c.value))
    return True
U32.EnumWindows(cb,0)
for hwnd,t,c in top:
    print('TOP',hwnd,repr(t),c)
    kids=[]
    @ctypes.WINFUNCTYPE(wt.BOOL, wt.HWND, wt.LPARAM)
    def ccb(h2, lp):
        cc=ctypes.create_unicode_buffer(256); U32.GetClassNameW(h2,cc,256)
        bb=ctypes.create_unicode_buffer(256); U32.GetWindowTextW(h2,bb,256)
        kids.append((h2,bb.value,cc.value))
        return True
    U32.EnumChildWindows(hwnd, ccb, 0)
    for k in kids[:10]:
        print('   CHILD',k)
print('foreground',U32.GetForegroundWindow(),'focus',U32.GetFocus())
print('diag patched/keycall/getcall/setcall/lastvk/alive:',
      [i32(512+i*4) for i in range(6)])
print('enabled',i32(4),'seq',i32(8))
