"""Decode every type 0..5 art resource with the game's own decoder.

Requires a running Nec.exe (e.g. start it with tools/orig_play_mission.py).
For each resource the game decoder FUN_0040cd80 is called in-process, then the
exact atlas slice it writes (p2 * 0x20 bytes at p3 * 0x200) is saved.

Outputs:
  dump/art_resources/res_XX_tY_pZ_nN.bin  raw 4bpp atlas slice
  dump/art_resources/res_XX_bank0.png     bank-0 preview (tiles or sprites)
  dump/art_resources/manifest.json
  dump/art_resources/atlas_full.bin       atlas after all resources
"""
import ctypes, ctypes.wintypes as wt, os, sys, time, struct, subprocess, json, math
sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
OUT = os.path.join(ROOT, 'dump', 'art_resources')
os.makedirs(OUT, exist_ok=True)
EXE = os.path.join(ROOT, 'game', 'Nec.exe')
TARGET_VA = 0x40cd80
ATLAS_VA = 0x484730
ATLAS_SIZE = 0x20000

# ---- static descriptor table ----
data = open(EXE, 'rb').read()
ib = 0x400000
secs = [(0x1000, 0x27400, 0x400, 0x27400), (0x2a000, 0x6c064, 0x27c00, 0x56200)]


def va_to_off(va):
    rva = va - ib
    for sva, vsize, rawptr, rawsize in secs:
        if sva <= rva < sva + max(vsize, rawsize):
            return rawptr + (rva - sva)
    raise ValueError(hex(va))


def u8(va):
    return data[va_to_off(va)]


def descriptor(rid):
    t = u8(0x42bb88 + rid * 4)
    p1 = u8(0x42bb89 + rid * 4)
    p2 = u8(0x42bb8a + rid * 4) or 256
    p3 = u8(0x42bb8b + rid * 4)
    count = u8(0x4324d0 + p1 * 4) or 256
    return t, p1, p2, p3, count


# ---- process / remote call ----
def find_pid():
    for line in subprocess.check_output(['tasklist', '/FI', 'IMAGENAME eq Nec.exe', '/FO', 'CSV', '/NH'], text=True).splitlines():
        p = [x.strip('"') for x in line.split('","')]
        if p and p[0].lower() == 'nec.exe':
            return int(p[1])
    return None


pid = find_pid()
if pid is None:
    raise SystemExit('Nec.exe is not running; start it with tools/orig_play_mission.py')
h = dm.k32.OpenProcess(0x1F0FFF, False, pid)
base = dm.module_base(h)
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
target = base + (TARGET_VA - 0x400000)
mem = k32.VirtualAllocEx(h, None, 0x1000, 0x3000, 0x40)
got = ctypes.c_size_t()


def call_resource(rid):
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
    time.sleep(0.12)


def rd(va, n):
    return dm.read(h, base, va, n)


pal_raw = rd(0x480f20, 4 + 256 * 4)
PAL = [(pal_raw[4 + i * 4], pal_raw[5 + i * 4], pal_raw[6 + i * 4]) for i in range(256)]


def render_tiles(raw, count, bank, scale=2, cols=16):
    # Expanded tile bank: 16x16 8bpp, stored bottom-up.
    tw = th = 16
    rows = max(1, (count + cols - 1) // cols)
    img = Image.new('RGBA', (cols * tw * scale, rows * th * scale), (0, 0, 0, 0))
    px = img.load()
    for t in range(count):
        base_t = t * 256
        if base_t + 256 > len(raw):
            break
        sx = (t % cols) * tw * scale
        sy = (t // cols) * th * scale
        for y in range(16):
            for x in range(16):
                v = raw[base_t + y * 16 + x]
                if not v:
                    continue
                r, g, b = PAL[(bank << 4) | (v & 0xF)]
                for yy in range(scale):
                    for xx in range(scale):
                        px[sx + x * scale + xx, sy + (15 - y) * scale + yy] = (r, g, b, 255)
    return img


def render_sprites(raw, count, bank, scale=2, cols=8):
    # Expanded sprite bank: 32x32 8bpp, stored bottom-up.
    tw = th = 32
    rows = max(1, (count + cols - 1) // cols)
    img = Image.new('RGBA', (cols * tw * scale, rows * th * scale), (0, 0, 0, 0))
    px = img.load()
    for t in range(count):
        base_t = t * 1024
        if base_t + 1024 > len(raw):
            break
        sx = (t % cols) * tw * scale
        sy = (t // cols) * th * scale
        for y in range(32):
            for x in range(32):
                v = raw[base_t + y * 32 + x]
                if not v:
                    continue
                r, g, b = PAL[(bank << 4) | (v & 0xF)]
                for yy in range(scale):
                    for xx in range(scale):
                        px[sx + x * scale + xx, sy + (31 - y) * scale + yy] = (r, g, b, 255)
    return img


tb_ptr = struct.unpack('<I', rd(0x481500, 4))[0]
sb_ptr = struct.unpack('<I', rd(0x480f14, 4))[0]

manifest = []
for rid in range(59):
    t, p1, p2, p3, count = descriptor(rid)
    if t > 5:
        continue
    call_resource(rid)
    start = p3 * 0x200
    nbytes = p2 * 0x20
    raw = rd(ATLAS_VA + start, nbytes)
    fn = 'res_%02d_t%d_p%03d_n%03d.bin' % (rid, t, p3, p2)
    open(os.path.join(OUT, fn), 'wb').write(raw)
    if t in (0, 1):
        units = p2
        expanded = dm.read_abs(h, tb_ptr + p3 * 0x1000, units * 256)
        efn = 'res_%02d_tiles.bin' % rid
        open(os.path.join(OUT, efn), 'wb').write(expanded)
        img = render_tiles(expanded, units, 0)
        kind = 'tile'
    else:
        units = p2 // 4
        expanded = dm.read_abs(h, sb_ptr + p3 * 0x1000, units * 1024)
        efn = 'res_%02d_sprites.bin' % rid
        open(os.path.join(OUT, efn), 'wb').write(expanded)
        img = render_sprites(expanded, units, 0)
        kind = 'sprite'
    img.save(os.path.join(OUT, 'res_%02d_bank0.png' % rid))
    manifest.append({
        'id': rid, 'type': t, 'p1': p1, 'p2': p2, 'p3': p3,
        'count': count, 'kind': kind, 'units': units,
        'atlas_offset': start, 'bytes': nbytes, 'raw': fn, 'expanded': efn,
    })
    print('resource %2d type %d %-6s units=%3d atlas=%#07x bytes=%d' %
          (rid, t, kind, units, start, nbytes))

# final full atlas + expanded banks
open(os.path.join(OUT, 'atlas_full.bin'), 'wb').write(rd(ATLAS_VA, ATLAS_SIZE))
open(os.path.join(OUT, 'tilebank.bin'), 'wb').write(dm.read_abs(h, tb_ptr, 0x80000))
open(os.path.join(OUT, 'spritebank.bin'), 'wb').write(dm.read_abs(h, sb_ptr, 0x100000))
open(os.path.join(OUT, 'palette.bin'), 'wb').write(pal_raw)
json.dump({'resources': manifest}, open(os.path.join(OUT, 'manifest.json'), 'w'), indent=1)
print('wrote', OUT)
dm.k32.CloseHandle(h)
