"""Call the game's resource decoder for one or more resource ids in a live process.

Usage: python tools/call_resource.py 12 [more ids...]
The game must already be running.  The decoder is FUN_0040cd80 at VA 0x40cd80.
"""
import ctypes, ctypes.wintypes as wt, os, sys, time, struct, subprocess, hashlib
sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
OUT = os.path.join(ROOT, 'dump', 'live_art')
os.makedirs(OUT, exist_ok=True)
TARGET_VA = 0x40cd80
ATLAS_VA = 0x484730
ATLAS_SIZE = 0x20000

k32 = ctypes.WinDLL('kernel32', use_last_error=True)
k32.VirtualAllocEx.restype = ctypes.c_void_p
k32.VirtualAllocEx.argtypes = [wt.HANDLE, ctypes.c_void_p, ctypes.c_size_t, wt.DWORD, wt.DWORD]
k32.WriteProcessMemory.argtypes = [wt.HANDLE, ctypes.c_void_p, ctypes.c_void_p, ctypes.c_size_t,
                                   ctypes.POINTER(ctypes.c_size_t)]
k32.CreateRemoteThread.restype = wt.HANDLE
k32.CreateRemoteThread.argtypes = [wt.HANDLE, ctypes.c_void_p, ctypes.c_size_t, ctypes.c_void_p,
                                   ctypes.c_void_p, wt.DWORD, ctypes.c_void_p]
k32.WaitForSingleObject.argtypes = [wt.HANDLE, wt.DWORD]
k32.CloseHandle.argtypes = [wt.HANDLE]


def find_pid():
    for line in subprocess.check_output(['tasklist', '/FI', 'IMAGENAME eq Nec.exe', '/FO', 'CSV', '/NH'], text=True).splitlines():
        p = [x.strip('"') for x in line.split('","')]
        if p and p[0].lower() == 'nec.exe':
            return int(p[1])
    return None


def main():
    ids = [int(x, 0) for x in sys.argv[1:]] or [12]
    pid = find_pid()
    if pid is None:
        raise SystemExit('Nec.exe is not running')
    h = dm.k32.OpenProcess(0x1F0FFF, False, pid)
    base = dm.module_base(h)
    target = base + (TARGET_VA - 0x400000)
    mem = k32.VirtualAllocEx(h, None, 0x1000, 0x3000, 0x40)
    got = ctypes.c_size_t()

    def call(rid):
        code = bytearray()
        code += b'\x68' + struct.pack('<I', 0)
        code += b'\x68' + struct.pack('<I', 0)
        code += b'\x68' + struct.pack('<I', 0)
        code += b'\x68' + struct.pack('<I', 0)
        code += b'\x68' + struct.pack('<I', rid)
        code += b'\xE8' + struct.pack('<i', target - (mem + len(code) + 5))
        code += b'\x83\xC4\x14\xC3'
        k32.WriteProcessMemory(h, ctypes.c_void_p(mem), bytes(code), len(code), ctypes.byref(got))
        th = k32.CreateRemoteThread(h, None, 0, ctypes.c_void_p(mem), None, 0, None)
        k32.WaitForSingleObject(th, 5000)
        k32.CloseHandle(th)
        time.sleep(0.15)

    for rid in ids:
        before = dm.read(h, base, ATLAS_VA, ATLAS_SIZE)
        call(rid)
        after = dm.read(h, base, ATLAS_VA, ATLAS_SIZE)
        diff = sum(1 for a, b in zip(before, after) if a != b)
        open(os.path.join(OUT, 'atlas_after_%02d.bin' % rid), 'wb').write(after)
        print('resource %d: atlas bytes changed=%d md5=%s' %
              (rid, diff, hashlib.md5(after).hexdigest()))
    dm.k32.CloseHandle(h)


if __name__ == '__main__':
    main()
