"""Dump the running game's decoded art/palette/state out of its address space."""
import ctypes, ctypes.wintypes as wt, subprocess, time, os, struct
from ctypes import byref, POINTER

k32 = ctypes.WinDLL('kernel32', use_last_error=True)
psapi = ctypes.WinDLL('psapi', use_last_error=True)

PROCESS_VM_READ = 0x0010
PROCESS_QUERY_INFORMATION = 0x0400
LIST_MODULES_32BIT = 0x01

class MODULEINFO(ctypes.Structure):
    _fields_ = [("lpBaseOfDll", ctypes.c_void_p), ("SizeOfImage", wt.DWORD),
                ("EntryPoint", ctypes.c_void_p)]

k32.OpenProcess.restype = wt.HANDLE
k32.OpenProcess.argtypes = [wt.DWORD, wt.BOOL, wt.DWORD]
k32.ReadProcessMemory.argtypes = [wt.HANDLE, ctypes.c_void_p, ctypes.c_void_p,
                                  ctypes.c_size_t, POINTER(ctypes.c_size_t)]
k32.CloseHandle.argtypes = [wt.HANDLE]
psapi.EnumProcessModulesEx.argtypes = [wt.HANDLE, POINTER(wt.HMODULE), wt.DWORD,
                                       POINTER(wt.DWORD), wt.DWORD]
psapi.GetModuleInformation.argtypes = [wt.HANDLE, wt.HMODULE, POINTER(MODULEINFO), wt.DWORD]

def module_base(hproc):
    mods = (wt.HMODULE * 256)()
    needed = wt.DWORD()
    if not psapi.EnumProcessModulesEx(hproc, mods, ctypes.sizeof(mods),
                                      byref(needed), LIST_MODULES_32BIT):
        return None
    mi = MODULEINFO()
    if not psapi.GetModuleInformation(hproc, mods[0], byref(mi), ctypes.sizeof(mi)):
        return None
    return mi.lpBaseOfDll

def read(hproc, base, va, size):
    buf = ctypes.create_string_buffer(size)
    got = ctypes.c_size_t()
    ok = k32.ReadProcessMemory(hproc, ctypes.c_void_p(base + (va - 0x400000)),
                               buf, size, byref(got))
    if not ok:
        raise OSError(f"ReadProcessMemory failed at {va:#x}: {ctypes.get_last_error()}")
    return buf.raw[:got.value]

def read_abs(hproc, addr, size):
    buf = ctypes.create_string_buffer(size)
    got = ctypes.c_size_t()
    ok = k32.ReadProcessMemory(hproc, ctypes.c_void_p(addr), buf, size, byref(got))
    if not ok:
        raise OSError(f"ReadProcessMemory failed at {addr:#x}: {ctypes.get_last_error()}")
    return buf.raw[:got.value]

def u32(b, o): return struct.unpack_from('<I', b, o)[0]

if __name__ == '__main__':
    game = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'game'))
    out = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'dump'))
    os.makedirs(out, exist_ok=True)
    p = subprocess.Popen([os.path.join(game, 'Nec.exe')], cwd=game)
    time.sleep(6)
    h = k32.OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, False, p.pid)
    if not h:
        raise OSError('OpenProcess failed')
    base = module_base(h)
    print(f"pid={p.pid} imagebase={base:#x}")
    try:
        pal = read(h, base, 0x480f20, 4 + 256 * 4)
        atlas = read(h, base, 0x484730, 0x12000)
        screen = read(h, base, 0x480f18, 0x2000)
        units = {
            'u_typeowner': read(h, base, 0x4957a0, 0x38),
            'u_hp_rank':  read(h, base, 0x4957e0, 0x38),
            'u_x':        read(h, base, 0x495ab0, 0x38),
            'u_y':        read(h, base, 0x495af0, 0x38),
            'terrain':    read(h, base, 0x481a80, 0x580),
            'occupancy':  read(h, base, 0x495820, 0x580),
            'renderlist': read(h, base, 0x4836f0, 0x1000),
        }
        hdr = read(h, base, 0x481500, 4)
        tile_ptr = u32(hdr, 0)
        print(f"tilebank ptr = {tile_ptr:#x}")
        hdr2 = read(h, base, 0x480f14, 4)
        sprite_ptr = u32(hdr2, 0)
        print(f"spritebank ptr = {sprite_ptr:#x}")
        print(f"map dims = {read(h,base,0x495c02,2).hex()}")
        open(os.path.join(out, 'palette.bin'), 'wb').write(pal)
        open(os.path.join(out, 'atlas.bin'), 'wb').write(atlas)
        if tile_ptr:
            open(os.path.join(out, 'tilebank.bin'), 'wb').write(read_abs(h, tile_ptr, 0x80000))
        if sprite_ptr:
            open(os.path.join(out, 'spritebank.bin'), 'wb').write(read_abs(h, sprite_ptr, 0x100000))
        for k, v in units.items():
            open(os.path.join(out, k + '.bin'), 'wb').write(v)
        nz = sum(1 for x in atlas if x)
        print(f"atlas nonzero bytes: {nz}/{len(atlas)}")
        print("unit arrays (slot: type/owner, hp/rank, x, y):")
        for i in range(0x38):
            t, hr, x, y = units['u_typeowner'][i], units['u_hp_rank'][i], units['u_x'][i], units['u_y'][i]
            if t != 0 or hr != 0:
                print(f"  {i:2d}: type={t&0x1f:2d} own={0x80 if t&0x80 else 0:3d} "
                      f"hp={hr&7} rank={hr>>5} x={x&0x1f} y={y&0x1f}")
    finally:
        k32.CloseHandle(h)
        p.terminate()
