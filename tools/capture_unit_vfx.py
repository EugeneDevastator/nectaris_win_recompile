"""Capture the original combat animation for one attacker/defender pair.

Reuses the forced player attack flow from player_attack2.py, then records the
sprite queue (0x4836f0 / +0x800) every ~60 ms for the duration of the combat.

Usage:
  python tools/capture_unit_vfx.py --attacker 4 --defender 6
  python tools/capture_unit_vfx.py --batch 4,9,17,18,1

Outputs under dump/unit_vfx/tXX_dYY/:
  q_NNN.json    sprite queue snapshot (bank/slot/x/y/flags/sprite)
  f_NNN.png     framebuffer (optional, every 5th frame)
  palette.bin   palette captured during the combat
  spritebank.bin sprite bank captured during the combat
"""
import argparse
import ctypes
import ctypes.wintypes as wt
import json
import os
import struct
import subprocess
import sys
import time

sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
OUT = os.path.join(ROOT, 'dump', 'unit_vfx')

K32 = ctypes.WinDLL('kernel32', use_last_error=True)
K32.CreateFileMappingW.restype = wt.HANDLE
K32.CreateFileMappingW.argtypes = [wt.HANDLE, ctypes.c_void_p, wt.DWORD, wt.DWORD, wt.DWORD, wt.LPCWSTR]
K32.MapViewOfFile.restype = ctypes.c_void_p
K32.MapViewOfFile.argtypes = [wt.HANDLE, wt.DWORD, wt.DWORD, wt.DWORD, ctypes.c_size_t]
hmap = K32.CreateFileMappingW(wt.HANDLE(-1).value, None, 4, 0, 4096, "Local\\NecFakeInput")
ptr = K32.MapViewOfFile(hmap, 0xF001F, 0, 0, 4096)
ctypes.memset(ptr, 0, 4096)
ctypes.c_long.from_address(ptr + 0).value = 0x4E454331
ctypes.c_long.from_address(ptr + 4).value = 1
seq = [0]

U32 = ctypes.WinDLL('user32', use_last_error=True)
U32.EnumWindows.argtypes = [ctypes.WINFUNCTYPE(wt.BOOL, wt.HWND, wt.LPARAM), wt.LPARAM]
U32.GetWindowThreadProcessId.argtypes = [wt.HWND, ctypes.POINTER(wt.DWORD)]
U32.GetWindowTextW.argtypes = [wt.HWND, wt.LPWSTR, ctypes.c_int]
U32.PostMessageW.argtypes = [wt.HWND, wt.UINT, wt.WPARAM, wt.LPARAM]
U32.MapVirtualKeyW.argtypes = [wt.UINT, wt.UINT]


def find_pid():
    for line in subprocess.check_output(['tasklist', '/FI', 'IMAGENAME eq Nec.exe', '/FO', 'CSV', '/NH'],
                                        text=True).splitlines():
        p = [x.strip('"') for x in line.split('","')]
        if p and p[0].lower() == 'nec.exe':
            return int(p[1])
    return None


def find_window(pid):
    found = []

    @ctypes.WINFUNCTYPE(wt.BOOL, wt.HWND, wt.LPARAM)
    def cb(hwnd, lp):
        p = wt.DWORD()
        U32.GetWindowThreadProcessId(hwnd, ctypes.byref(p))
        if p.value == pid:
            b = ctypes.create_unicode_buffer(256)
            U32.GetWindowTextW(hwnd, b, 256)
            if 'nectaris' in b.value.lower():
                found.append(hwnd)
        return True

    U32.EnumWindows(cb, 0)
    return found[0] if found else None


def u32(h, base, va):
    return struct.unpack('<I', dm.read(h, base, va, 4))[0]


def rd8(h, base, va):
    return dm.read(h, base, va, 1)[0]


def wr8(h, base, va, v):
    got = ctypes.c_size_t()
    dm.k32.WriteProcessMemory(h, ctypes.c_void_p(base + (va - 0x400000)), bytes([v & 0xFF]), 1, ctypes.byref(got))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--attacker', type=int, default=4)
    ap.add_argument('--defender', type=int, default=6)
    ap.add_argument('--batch', default='')
    ap.add_argument('--frames', type=int, default=90)
    args = ap.parse_args()

    types = [int(x) for x in args.batch.split(',')] if args.batch else [args.attacker]
    for atk in types:
        run_one(atk, args.defender, args.frames)
    return 0


def run_one(atk, dfn, frames):
    d = os.path.join(OUT, 't%02d_d%02d' % (atk, dfn))
    os.makedirs(d, exist_ok=True)
    print('=== attacker %d, defender %d ===' % (atk, dfn), flush=True)

    subprocess.run(['taskkill', '/F', '/IM', 'Nec.exe'], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    time.sleep(0.3)
    subprocess.call([sys.executable, os.path.join(ROOT, 'tools', 'orig_play_mission.py'), '1', '--no-preview'],
                    stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    time.sleep(0.5)
    pid = find_pid()
    hwnd = find_window(pid)
    h = dm.k32.OpenProcess(0x1F0FFF, False, pid)
    base = dm.module_base(h)

    def tap(vk, hold=0.12, gap=0.25):
        sc = U32.MapVirtualKeyW(vk, 0) & 0xFF
        ctypes.c_ubyte.from_address(ptr + 36 + vk).value = 1
        U32.PostMessageW(hwnd, 0x0100, vk, 1 | (sc << 16))
        U32.PostMessageW(hwnd, 0x0102, vk, 1 | (sc << 16))
        time.sleep(hold)
        ctypes.c_ubyte.from_address(ptr + 36 + vk).value = 0
        U32.PostMessageW(hwnd, 0x0101, vk, 1 | (sc << 16) | 0xC0000000)
        time.sleep(gap)

    def wake():
        seq[0] += 1
        ctypes.c_long.from_address(ptr + 8).value = seq[0]
        U32.PostMessageW(hwnd, 0x0200, 0, 0)
        U32.PostMessageW(hwnd, 0x0000, 0, 0)

    def read_queue():
        out = []
        for bank in (0, 1):
            raw = dm.read(h, base, 0x4836f0 + bank * 0x800, 0x800)
            for i in range(0x100):
                e = raw[i * 8:(i + 1) * 8]
                y, x = struct.unpack_from('<HH', e, 0)
                if x == 0 and y == 0:
                    continue
                out.append({'bank': bank, 'slot': i, 'x': x, 'y': y,
                            'flags': e[4], 'sprite': e[5] | (e[7] << 8)})
        return out

    # --- navigate to map and force a player attack (same path as player_attack2)
    t0 = time.time()
    while time.time() - t0 < 30:
        wake()
        if u32(h, base, 0x483270) == 0x416700:
            break
        time.sleep(0.15)
    time.sleep(2.5)
    tap(0x20, gap=0.6)                    # leave action selection

    gx, gy = rd8(h, base, 0x495ab0 + 5) & 31, rd8(h, base, 0x495af0 + 5) & 31
    wr8(h, base, 0x495ab0 + 5, 4)
    wr8(h, base, 0x495af0 + 5, 7)
    wr8(h, base, 0x495820 + gy * 32 + gx, 0)
    wr8(h, base, 0x495820 + 7 * 32 + 4, 0x80 | 5)
    wr8(h, base, 0x4957a0 + 2, atk & 31)
    wr8(h, base, 0x4957a0 + 5, dfn & 31)

    for _ in range(40):
        x, y = rd8(h, base, 0x481a72), rd8(h, base, 0x481a71)
        if x == 4 and y == 8:
            break
        if x < 4:
            tap(0x27, gap=0.12)
        elif x > 4:
            tap(0x25, gap=0.12)
        elif y < 8:
            tap(0x28, gap=0.12)
        elif y > 8:
            tap(0x26, gap=0.12)

    tap(0x20, gap=0.5)                    # open menu
    tap(0x28, gap=0.4)                    # attack
    tap(0x20, gap=0.5)                    # choose target
    tap(0x26, gap=0.4)                    # target above (green at 4,7)
    tap(0x20, gap=0.6)                    # confirm

    for i in range(frames):
        wake()
        try:
            q = read_queue()
            json.dump(q, open(os.path.join(d, 'q_%03d.json' % i), 'w'))
            if i % 5 == 0:
                pal = dm.read(h, base, 0x480f20, 4 + 256 * 4)
                fb = dm.read_abs(h, u32(h, base, 0x480f18), 704 * 544)
                rgb = bytearray(768)
                for k in range(256):
                    rgb[k * 3] = pal[4 + k * 4]
                    rgb[k * 3 + 1] = pal[5 + k * 4]
                    rgb[k * 3 + 2] = pal[6 + k * 4]
                im = Image.frombytes('P', (704, 544), fb).transpose(Image.FLIP_TOP_BOTTOM).crop((32, 32, 672, 512))
                im.putpalette(bytes(rgb))
                im.convert('RGB').save(os.path.join(d, 'f_%03d.png' % i))
                if not os.path.exists(os.path.join(d, 'palette.bin')):
                    open(os.path.join(d, 'palette.bin'), 'wb').write(pal)
            if q and not os.path.exists(os.path.join(d, 'spritebank.bin')):
                sb = u32(h, base, 0x480f14)
                if sb:
                    open(os.path.join(d, 'spritebank.bin'), 'wb').write(dm.read_abs(h, sb, 0x100000))
        except OSError:
            break
        time.sleep(0.06)
    print('captured frames to', d, flush=True)
    dm.k32.CloseHandle(h)


if __name__ == '__main__':
    sys.exit(main())
