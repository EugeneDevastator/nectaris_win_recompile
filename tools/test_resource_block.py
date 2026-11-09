"""Call the game's resource decoder for candidate resources and dump slot 128..191.

Compares the resulting spritebank block with the captured combat states.
"""
import ctypes, ctypes.wintypes as wt, os, sys, time, struct, subprocess, json, hashlib
sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
OUT = os.path.join(ROOT, 'dump', 'resource_blocks')
os.makedirs(OUT, exist_ok=True)
TARGET_VA = 0x40cd80
ATLAS_VA = 0x484730
ATLAS_SIZE = 0x20000
RIDS = [13, 14, 27, 40, 23, 2, 3, 15, 16, 17, 21, 42, 43, 44]

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


for line in subprocess.check_output(['tasklist', '/FI', 'IMAGENAME eq Nec.exe', '/FO', 'CSV', '/NH'],
                                    text=True).splitlines():
    p = [x.strip('"') for x in line.split('","')]
    if p and p[0].lower() == 'nec.exe':
        subprocess.call(['taskkill', '/F', '/PID', p[1]], stdout=subprocess.DEVNULL)
subprocess.call([sys.executable, os.path.join(ROOT, 'tools', 'orig_play_mission.py'), '6', '--no-preview'],
                stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
time.sleep(1.0)
pid = find_pid()
if pid is None:
    raise SystemExit('could not start Nec.exe')
print('pid', pid, flush=True)
h = dm.k32.OpenProcess(0x1F0FFF, False, pid)
base = dm.module_base(h)
target = base + (TARGET_VA - 0x400000)
mem = k32.VirtualAllocEx(h, None, 0x1000, 0x3000, 0x40)
got = ctypes.c_size_t()


def call(rid):
    code = bytearray()
    for _ in range(4):
        code += b'\x68' + struct.pack('<I', 0)
    code += b'\x68' + struct.pack('<I', rid)
    code += b'\xE8' + struct.pack('<i', target - (mem + len(code) + 5))
    code += b'\x83\xC4\x14\xC3'
    k32.WriteProcessMemory(h, ctypes.c_void_p(mem), bytes(code), len(code), ctypes.byref(got))
    th = k32.CreateRemoteThread(h, None, 0, ctypes.c_void_p(mem), None, 0, None)
    k32.WaitForSingleObject(th, 5000)
    k32.CloseHandle(th)
    time.sleep(0.15)


def u32(va):
    return struct.unpack('<I', dm.read(h, base, va, 4))[0]


sb = u32(0x480f14)
print('spritebank', hex(sb), flush=True)
rows = []
for rid in RIDS:
    call(rid)
    blk = dm.read_abs(h, sb + 128 * 1024, 64 * 1024)
    at = dm.read(h, base, ATLAS_VA, ATLAS_SIZE)
    fn = os.path.join(OUT, 'res_%02d_block.bin' % rid)
    open(fn, 'wb').write(blk)
    open(os.path.join(OUT, 'res_%02d_atlas.bin' % rid), 'wb').write(at)
    hh = hashlib.md5(blk).hexdigest()[:8]
    rows.append((rid, hh, sum(1 for b in blk if b)))
    print('rid %2d md5 %s nonzero %d' % (rid, hh, rows[-1][2]), flush=True)

# compare with captured combat states
states = {}
for f in os.listdir(os.path.join(ROOT, 'dump', 'ai_turn2')):
    if f.startswith('sb_') and f.endswith('.bin'):
        d = open(os.path.join(ROOT, 'dump', 'ai_turn2', f), 'rb').read()[128 * 1024:192 * 1024]
        states.setdefault(hashlib.md5(d).hexdigest()[:8], f)
print('captured 128..191 hashes:', states, flush=True)
for rid, hh, nz in rows:
    print('rid %2d %s %s' % (rid, hh, 'MATCHES ' + states[hh] if hh in states else ''))
dm.k32.CloseHandle(h)
