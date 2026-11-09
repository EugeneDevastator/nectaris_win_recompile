"""Run a unit's combat chunk script in-process and dump the produced sprites.

FUN_0041c090 reads the unit index from 0x483274 (or 0x48327b), loads the static
script via FUN_0041c200, and installs the interpreter callback 0x41c0f0.
Calling the callback processes the script and writes chunks into the spritebank.
"""
import ctypes, ctypes.wintypes as wt, os, sys, time, struct, subprocess, json, hashlib
sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
OUT = os.path.join(ROOT, 'dump', 'unit_scripts')
os.makedirs(OUT, exist_ok=True)
TYPES = [int(x) for x in sys.argv[1:]] or list(range(23))
SETUP_VA = 0x41c090
INTERP_VA = 0x41c0f0

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


subprocess.run(['taskkill', '/F', '/IM', 'Nec.exe'], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
time.sleep(0.3)
subprocess.call([sys.executable, os.path.join(ROOT, 'tools', 'orig_play_mission.py'), '1', '--no-preview'],
                stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
time.sleep(0.5)
pid = find_pid()
if pid is None:
    raise SystemExit('no game')
h = dm.k32.OpenProcess(0x1F0FFF, False, pid)
base = dm.module_base(h)
print('pid', pid, 'base', hex(base), flush=True)
mem = k32.VirtualAllocEx(h, None, 0x1000, 0x3000, 0x40)
got = ctypes.c_size_t()


def call0(va):
    target = base + (va - 0x400000)
    code = bytearray()
    code += b'\xE8' + struct.pack('<i', target - (mem + len(code) + 5))
    code += b'\xC3'
    k32.WriteProcessMemory(h, ctypes.c_void_p(mem), bytes(code), len(code), ctypes.byref(got))
    th = k32.CreateRemoteThread(h, None, 0, ctypes.c_void_p(mem), None, 0, None)
    k32.WaitForSingleObject(th, 8000)
    k32.CloseHandle(th)
    time.sleep(0.2)


def wr8(va, v):
    dm.k32.WriteProcessMemory(h, ctypes.c_void_p(base + (va - 0x400000)), bytes([v & 0xFF]), 1, ctypes.byref(got))


def u32(va):
    return struct.unpack('<I', dm.read(h, base, va, 4))[0]


def export(block, typ):
    pal = open(os.path.join(ROOT, 'dump', 'battle_sprites', 'palette.bin'), 'rb').read()
    PAL = [(pal[4 + i * 4], pal[5 + i * 4], pal[6 + i * 4]) for i in range(256)]
    for bank, tag in ((9, 'blue'), (10, 'green')):
        c = Image.new('RGBA', (64, 64), (0, 0, 0, 0))
        for j in range(4):
            t = Image.new('RGBA', (32, 32), (0, 0, 0, 0))
            px = t.load()
            for y in range(32):
                for x in range(32):
                    v = block[j * 1024 + y * 32 + x]
                    if v:
                        px[x, 31 - y] = PAL[((bank << 4) | (v & 15)) & 255] + (255,)
            c.alpha_composite(t, ((j % 2) * 32, (j // 2) * 32))
        c.save(os.path.join(OUT, 'script_unit_%02d_%s.png' % (typ, tag)))


load_base = u32(0x483274)
for t in TYPES:
    wr8(0x483274, t)
    call0(SETUP_VA)
    call0(INTERP_VA)
    sb = u32(0x480f14)
    block = dm.read_abs(h, sb + 64 * 1024, 0x20000)  # slots 64..191
    fn = os.path.join(OUT, 'unit_%02d_block.bin' % t)
    open(fn, 'wb').write(block)
    print('unit %2d block md5 %s nonzero %d' % (t, hashlib.md5(block).hexdigest()[:8],
                                                sum(1 for b in block if b)), flush=True)
    export(block, t)
print('done', flush=True)
dm.k32.CloseHandle(h)
