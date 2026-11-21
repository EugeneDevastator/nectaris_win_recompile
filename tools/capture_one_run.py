"""Capture one battle at full game-frame resolution.

The normal sampler sleeps ~20 ms and starts recording only after the attack
keys, which can miss the first, fastest part of the entry animation.  This tool
starts a tight background sampler *before* the attack keys and records every
distinct sprite-queue snapshot (each game frame), deduplicated, into one JSON
file.

Usage:
  python tools/capture_one_run.py ATK_TYPE DEF_TYPE OUT_TAG [--seconds 8]
"""
import argparse
import ctypes
import ctypes.wintypes as wt
import json
import os
import struct
import subprocess
import sys
import threading
import time

sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm  # noqa: E402

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
GAME = os.path.join(ROOT, 'game')
OUT = os.path.join(ROOT, 'dump', 'one_run')

K32 = ctypes.WinDLL('kernel32', use_last_error=True)
K32.CreateFileMappingW.restype = wt.HANDLE
K32.CreateFileMappingW.argtypes = [wt.HANDLE, ctypes.c_void_p, wt.DWORD, wt.DWORD, wt.DWORD, wt.LPCWSTR]
K32.MapViewOfFile.restype = ctypes.c_void_p
K32.MapViewOfFile.argtypes = [wt.HANDLE, wt.DWORD, wt.DWORD, wt.DWORD, ctypes.c_size_t]
U32 = ctypes.WinDLL('user32', use_last_error=True)
U32.EnumWindows.argtypes = [ctypes.WINFUNCTYPE(wt.BOOL, wt.HWND, wt.LPARAM), wt.LPARAM]
U32.GetWindowThreadProcessId.argtypes = [wt.HWND, ctypes.POINTER(wt.DWORD)]
U32.GetWindowTextW.argtypes = [wt.HWND, wt.LPWSTR, ctypes.c_int]
U32.PostMessageW.argtypes = [wt.HWND, wt.UINT, wt.WPARAM, wt.LPARAM]
U32.MapVirtualKeyW.argtypes = [wt.UINT, wt.UINT]
U32.GetForegroundWindow.restype = wt.HWND
U32.AttachThreadInput.argtypes = [wt.DWORD, wt.DWORD, wt.BOOL]
U32.SetForegroundWindow.argtypes = [wt.HWND]
U32.BringWindowToTop.argtypes = [wt.HWND]
U32.ShowWindow.argtypes = [wt.HWND, ctypes.c_int]
K32.GetCurrentThreadId.restype = wt.DWORD

RIGHT, DOWN, LEFT, UP, SPACE, ENTER = 0x27, 0x28, 0x25, 0x26, 0x20, 0x0D

hmap = K32.CreateFileMappingW(wt.HANDLE(-1).value, None, 4, 0, 4096, 'Local\\NecFakeInput')
ptr = K32.MapViewOfFile(hmap, 0xF001F, 0, 0, 4096)
ctypes.memset(ptr, 0, 4096)


def i32(off, v=None):
    if v is None:
        return ctypes.c_long.from_address(ptr + off).value
    ctypes.c_long.from_address(ptr + off).value = v


i32(0, 0x4E454331)
i32(4, 1)
i32(12, 0)
i32(16, 0)
i32(28, 0)


def key(vk, down):
    ctypes.c_ubyte.from_address(ptr + 36 + vk).value = 1 if down else 0


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


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('atk', type=int)
    ap.add_argument('dfn', type=int)
    ap.add_argument('tag')
    ap.add_argument('--seconds', type=float, default=8.0)
    args = ap.parse_args()

    for line in subprocess.check_output(['tasklist', '/FI', 'IMAGENAME eq Nec.exe', '/FO', 'CSV', '/NH'],
                                        text=True).splitlines():
        parts = [x.strip('"') for x in line.split('","')]
        if parts and parts[0].lower() == 'nec.exe':
            subprocess.call(['taskkill', '/F', '/PID', parts[1]],
                            stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    time.sleep(0.3)
    proc = subprocess.Popen([os.path.join(GAME, 'Nec.exe')], cwd=GAME,
                            stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    hwnd = None
    for _ in range(200):
        hwnd = find_window(proc.pid)
        if hwnd:
            break
        time.sleep(0.05)
    if not hwnd:
        print('no window')
        return 1
    h = dm.k32.OpenProcess(0x1F0FFF, False, proc.pid)
    base = dm.module_base(h)

    def u8(va):
        return dm.read(h, base, va, 1)[0]

    def rd(va, n):
        return dm.read(h, base, va, n)

    def wr(va, data):
        got = ctypes.c_size_t()
        return dm.k32.WriteProcessMemory(h, ctypes.c_void_p(base + (va - 0x400000)),
                                         data, len(data), ctypes.byref(got))

    def focus():
        fg = U32.GetForegroundWindow()
        ft = U32.GetWindowThreadProcessId(fg, None)
        mt = K32.GetCurrentThreadId()
        U32.AttachThreadInput(mt, ft, True)
        U32.ShowWindow(hwnd, 5)
        U32.BringWindowToTop(hwnd)
        U32.SetForegroundWindow(hwnd)
        U32.AttachThreadInput(mt, ft, False)
        U32.PostMessageW(hwnd, 0x001C, 1, 0)
        U32.PostMessageW(hwnd, 0x0006, 1, 0)
        U32.PostMessageW(hwnd, 0x0007, 0, 0)

    def tap(vk, hold=0.12, gap=0.25):
        focus()
        sc = U32.MapVirtualKeyW(vk, 0) & 0xFF
        key(vk, 1)
        U32.PostMessageW(hwnd, 0x0100, vk, 1 | (sc << 16))
        U32.PostMessageW(hwnd, 0x0102, vk, 1 | (sc << 16))
        time.sleep(hold)
        key(vk, 0)
        U32.PostMessageW(hwnd, 0x0101, vk, 1 | (sc << 16) | 0xC0000000)
        time.sleep(gap)

    try:
        focus()
        t0 = time.time()
        toggle = False
        while time.time() - t0 < 50:
            if u8(0x495c02) == 15 and u8(0x495c03) == 10:
                break
            tap(SPACE if not toggle else ENTER, gap=0.40)
            toggle = not toggle
        time.sleep(2.0)

        types = rd(0x4957a0, 0x38)
        for slot in range(0x38):
            t = types[slot]
            if t == 0xff:
                continue
            owner = t & 0x80
            wr(0x4957a0 + slot, bytes([owner | (args.dfn if owner else args.atk)]))

        # navigation to the two units
        for _ in range(22):
            tap(LEFT, gap=0.08)
        for _ in range(22):
            tap(UP, gap=0.08)
        for _ in range(7):
            bx = u8(0x481a72)
            for _ in range(3):
                tap(RIGHT, gap=0.14)
                if u8(0x481a72) > bx:
                    break
        for _ in range(4):
            by = u8(0x481a71)
            for _ in range(3):
                tap(DOWN, gap=0.14)
                if u8(0x481a71) > by:
                    break

        # start the tight sampler before the attack keys so the first animation
        # frames are not lost
        samples = []
        stop = threading.Event()

        def sampler():
            last = None
            tstart = time.time()
            while not stop.is_set():
                try:
                    q0 = dm.read(h, base, 0x4836f0, 0x800)
                    q1 = dm.read(h, base, 0x483ef0, 0x800)
                except Exception:
                    break
                if (q0, q1) != last:
                    samples.append((round(time.time() - tstart, 4), q0, q1))
                    last = (q0, q1)
                time.sleep(0.001)

        th = threading.Thread(target=sampler, daemon=True)
        th.start()
        for vk in [SPACE, SPACE, RIGHT, RIGHT, SPACE, SPACE, RIGHT, DOWN, SPACE]:
            tap(vk, gap=0.30)
        time.sleep(args.seconds)
        stop.set()
        th.join(timeout=2.0)

        os.makedirs(OUT, exist_ok=True)
        path = os.path.join(OUT, args.tag + '.json')
        frames = []
        for ts, q0, q1 in samples:
            entries = []
            for bank, raw in ((0, q0), (1, q1)):
                for i in range(0x100):
                    e = raw[i * 8:(i + 1) * 8]
                    y, x = struct.unpack_from('<HH', e, 0)
                    if x == 0 and y == 0:
                        continue
                    entries.append([bank, x, y, e[4], e[5] | (e[7] << 8)])
            frames.append({'t': ts, 'e': entries})
        with open(path, 'w', encoding='utf-8') as f:
            json.dump({'atk': args.atk, 'def': args.dfn, 'frames': frames}, f)
        print('frames', len(frames), '->', path)
        return 0
    finally:
        try:
            dm.k32.CloseHandle(h)
        except Exception:
            pass
        try:
            proc.kill()
        except Exception:
            pass


if __name__ == '__main__':
    sys.exit(main())
