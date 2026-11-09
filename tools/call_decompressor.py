"""Call the game's LZSS decompressor to dump a resource stream byte-for-byte.

Stub: FUN_0040cd50(rid) -> [0x481f94]; FUN_00418bb0(); loop FUN_00418bf0() -> out.
"""
import ctypes, ctypes.wintypes as wt, os, sys, time, struct, subprocess
sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
OUT = os.path.join(ROOT, 'dump', 'game_streams')
os.makedirs(OUT, exist_ok=True)
RIDS = [int(x) for x in sys.argv[1:]] or [44]
N = 0x10000

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
    for line in subprocess.check_output(['tasklist', '/FI', 'IMAGENAME eq Nec.exe', '/FO', 'CSV', '/NH'],
                                        text=True).splitlines():
        p = [x.strip('"') for x in line.split('","')]
        if p and p[0].lower() == 'nec.exe':
            return int(p[1])
    return None


if find_pid() is None:
    subprocess.call([sys.executable, os.path.join(ROOT, 'tools', 'orig_play_mission.py'), '1', '--no-preview'],
                    stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    time.sleep(0.5)
pid = find_pid()
h = dm.k32.OpenProcess(0x1F0FFF, False, pid)
base = dm.module_base(h)
print('pid', pid, 'base', hex(base), flush=True)
mem = k32.VirtualAllocEx(h, None, 0x20000, 0x3000, 0x40)
outbuf = mem + 0x4000
got = ctypes.c_size_t()


def stub(rid):
    def rel(target, at):
        return struct.pack('<i', (base + (target - 0x400000)) - (at + 5))

    code = bytearray()
    # source = FUN_0040cd50(rid)
    code += b'\x68' + struct.pack('<I', rid)
    code += b'\xE8' + rel(0x40cd50, mem + len(code))
    code += b'\x83\xC4\x04'
    code += b'\xA3' + struct.pack('<I', base + (0x481f94 - 0x400000))
    # init decompressor
    code += b'\xE8' + rel(0x418bb0, mem + len(code))
    # output loop
    code += b'\xBF' + struct.pack('<I', outbuf)
    code += b'\xB9' + struct.pack('<I', N)
    loop = len(code)
    code += b'\xE8' + rel(0x418bf0, mem + len(code))
    code += b'\xAA\x49'
    code += b'\x75' + struct.pack('<b', loop - (len(code) + 2))
    code += b'\xC3'
    return bytes(code)


for rid in RIDS:
    p1 = rid
    code = stub(p1)
    k32.WriteProcessMemory(h, ctypes.c_void_p(mem), code, len(code), ctypes.byref(got))
    th = k32.CreateRemoteThread(h, None, 0, ctypes.c_void_p(mem), None, 0, None)
    k32.WaitForSingleObject(th, 15000)
    k32.CloseHandle(th)
    time.sleep(0.2)
    data = dm.read_abs(h, outbuf, N)
    fn = os.path.join(OUT, 'res_%03d.bin' % rid)
    open(fn, 'wb').write(data)
    # compare with our LZSS output
    lz = os.path.join(ROOT, 'dump', 'resources_lzss', 'res_%03d.bin' % rid)
    same = None
    if os.path.exists(lz):
        a = open(lz, 'rb').read()
        same = a[:512] == data[:512]
    print('rid %d -> %s  first512 matches lzss: %s' % (rid, fn, same), flush=True)
dm.k32.CloseHandle(h)
