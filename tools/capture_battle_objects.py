"""Capture one battle headless: object table + sprite queue + framebuffer.

The game runs on a private desktop, so nothing appears on screen.  Input is
injected through the Local\\NecFakeInput mapping (winmm proxy) plus window
messages, which work without a visible window.

Captured per distinct game frame:
  - the live sprite-object list at 0x482c30 (66 x 0x18 bytes), which holds the
    positions the queue builder consumes
  - both sprite-queue banks at 0x4836f0 / 0x483ef0
  - optionally the framebuffer

Usage:
  python tools/capture_battle_objects.py ATK_TYPE DEF_TYPE TAG [--seconds 8] [--frames]
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
OUT = os.path.join(ROOT, 'dump', 'object_runs')
DESKTOP = 'NecHeadless'

K32 = ctypes.WinDLL('kernel32', use_last_error=True)
K32.CreateFileMappingW.restype = wt.HANDLE
K32.CreateFileMappingW.argtypes = [wt.HANDLE, ctypes.c_void_p, wt.DWORD, wt.DWORD, wt.DWORD, wt.LPCWSTR]
K32.MapViewOfFile.restype = ctypes.c_void_p
K32.MapViewOfFile.argtypes = [wt.HANDLE, wt.DWORD, wt.DWORD, wt.DWORD, ctypes.c_size_t]
K32.GetCurrentThreadId.restype = wt.DWORD


class STARTUPINFO(ctypes.Structure):
    _fields_ = [('cb', wt.DWORD), ('lpReserved', wt.LPWSTR), ('lpDesktop', wt.LPWSTR),
                ('lpTitle', wt.LPWSTR), ('dwX', wt.DWORD), ('dwY', wt.DWORD),
                ('dwXSize', wt.DWORD), ('dwYSize', wt.DWORD), ('dwXCountChars', wt.DWORD),
                ('dwYCountChars', wt.DWORD), ('dwFillAttribute', wt.DWORD), ('dwFlags', wt.DWORD),
                ('wShowWindow', wt.WORD), ('cbReserved2', wt.WORD), ('lpReserved2', ctypes.c_void_p),
                ('hStdInput', wt.HANDLE), ('hStdOutput', wt.HANDLE), ('hStdError', wt.HANDLE)]


class PROCESS_INFORMATION(ctypes.Structure):
    _fields_ = [('hProcess', wt.HANDLE), ('hThread', wt.HANDLE),
                ('dwProcessId', wt.DWORD), ('dwThreadId', wt.DWORD)]


K32.CreateProcessW.argtypes = [wt.LPCWSTR, wt.LPWSTR, ctypes.c_void_p, ctypes.c_void_p, wt.BOOL,
                               wt.DWORD, ctypes.c_void_p, wt.LPCWSTR,
                               ctypes.POINTER(STARTUPINFO), ctypes.POINTER(PROCESS_INFORMATION)]

U32 = ctypes.WinDLL('user32', use_last_error=True)
U32.CreateDesktopW.restype = wt.HANDLE
U32.CreateDesktopW.argtypes = [wt.LPCWSTR, wt.LPCWSTR, ctypes.c_void_p, wt.DWORD, wt.DWORD,
                               ctypes.c_void_p]
U32.EnumDesktopWindows.argtypes = [wt.HANDLE, ctypes.WINFUNCTYPE(wt.BOOL, wt.HWND, wt.LPARAM),
                                   wt.LPARAM]
U32.GetWindowThreadProcessId.argtypes = [wt.HWND, ctypes.POINTER(wt.DWORD)]
U32.GetWindowTextW.argtypes = [wt.HWND, wt.LPWSTR, ctypes.c_int]
U32.PostMessageW.argtypes = [wt.HWND, wt.UINT, wt.WPARAM, wt.LPARAM]
U32.MapVirtualKeyW.argtypes = [wt.UINT, wt.UINT]
U32.ShowWindow.argtypes = [wt.HWND, ctypes.c_int]
U32.SetForegroundWindow.argtypes = [wt.HWND]
U32.GetForegroundWindow.restype = wt.HWND
U32.SetThreadDesktop.argtypes = [wt.HANDLE]

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


def find_window(hdesk, pid):
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

    U32.EnumDesktopWindows(hdesk, cb, 0)
    return found[0] if found else None


def focus_on_desktop(hdesk, hwnd):
    """Activate the window inside the hidden desktop (its own foreground)."""
    result = {}

    def worker():
        result['set'] = U32.SetThreadDesktop(hdesk)
        U32.ShowWindow(hwnd, 5)
        result['fg'] = U32.SetForegroundWindow(hwnd)
        result['fgw'] = U32.GetForegroundWindow()

    th = threading.Thread(target=worker, daemon=True)
    th.start()
    th.join(timeout=2.0)
    return result


def launch(hdesk):
    for line in subprocess.check_output(['tasklist', '/FI', 'IMAGENAME eq Nec.exe', '/FO', 'CSV', '/NH'],
                                        text=True).splitlines():
        parts = [x.strip('"') for x in line.split('","')]
        if parts and parts[0].lower() == 'nec.exe':
            subprocess.call(['taskkill', '/F', '/PID', parts[1]],
                            stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    time.sleep(0.3)
    si = STARTUPINFO()
    si.cb = ctypes.sizeof(si)
    si.lpDesktop = 'WinSta0\\' + DESKTOP
    pi = PROCESS_INFORMATION()
    ok = K32.CreateProcessW(os.path.join(GAME, 'Nec.exe'), None, None, None, False, 0,
                            None, GAME, ctypes.byref(si), ctypes.byref(pi))
    if not ok:
        raise OSError('CreateProcess failed %d' % ctypes.get_last_error())
    return pi


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('atk', type=int)
    ap.add_argument('dfn', type=int)
    ap.add_argument('tag')
    ap.add_argument('--seconds', type=float, default=7.0)
    ap.add_argument('--frames', action='store_true')
    ap.add_argument('--dumpbank', default=None, help='write the battle sprite bank here')
    args = ap.parse_args()

    hdesk = U32.CreateDesktopW(DESKTOP, None, None, 0, 0x10000000, None)
    if not hdesk:
        print('desktop create failed', ctypes.get_last_error())
        return 1
    pi = launch(hdesk)
    hwnd = None
    for _ in range(200):
        hwnd = find_window(hdesk, pi.dwProcessId)
        if hwnd:
            break
        time.sleep(0.05)
    if not hwnd:
        print('no window')
        return 1
    fr = focus_on_desktop(hdesk, hwnd)
    if not fr.get('fg'):
        print('warning: foreground set failed', fr)
    h = dm.k32.OpenProcess(0x1F0FFF, False, pi.dwProcessId)
    base = dm.module_base(h)

    def u8(va):
        return dm.read(h, base, va, 1)[0]

    def rd(va, n):
        return dm.read(h, base, va, n)

    def wr(va, data):
        got = ctypes.c_size_t()
        return dm.k32.WriteProcessMemory(h, ctypes.c_void_p(base + (va - 0x400000)),
                                         data, len(data), ctypes.byref(got))

    def tap(vk, hold=0.12, gap=0.25):
        sc = U32.MapVirtualKeyW(vk, 0) & 0xFF
        key(vk, 1)
        U32.PostMessageW(hwnd, 0x0100, vk, 1 | (sc << 16))
        U32.PostMessageW(hwnd, 0x0102, vk, 1 | (sc << 16))
        time.sleep(hold)
        key(vk, 0)
        U32.PostMessageW(hwnd, 0x0101, vk, 1 | (sc << 16) | 0xC0000000)
        time.sleep(gap)

    try:
        t0 = time.time()
        toggle = False
        reached = False
        while time.time() - t0 < 50:
            if u8(0x495c02) == 15 and u8(0x495c03) == 10:
                reached = True
                break
            tap(SPACE if not toggle else ENTER, gap=0.40)
            toggle = not toggle
        print('boot reached map in %.1fs: %s' % (time.time() - t0, reached))
        time.sleep(2.0)

        types = rd(0x4957a0, 0x38)
        for slot in range(0x38):
            t = types[slot]
            if t == 0xff:
                continue
            owner = t & 0x80
            wr(0x4957a0 + slot, bytes([owner | (args.dfn if owner else args.atk)]))

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

        os.makedirs(OUT, exist_ok=True)
        frame_dir = os.path.join(OUT, args.tag + '_frames')
        if args.frames:
            os.makedirs(frame_dir, exist_ok=True)

        samples = []
        stop = threading.Event()
        idx = [0]

        def save_frame(i):
            try:
                pal = dm.read(h, base, 0x480f20, 4 + 256 * 4)
                fb = dm.read_abs(h, struct.unpack('<I', dm.read(h, base, 0x480f18, 4))[0], 704 * 544)
                from PIL import Image
                rgb = bytearray(768)
                for k in range(256):
                    rgb[k * 3] = pal[4 + k * 4]
                    rgb[k * 3 + 1] = pal[5 + k * 4]
                    rgb[k * 3 + 2] = pal[6 + k * 4]
                im = Image.frombytes('P', (704, 544), fb).transpose(Image.FLIP_TOP_BOTTOM)
                im = im.crop((32, 32, 672, 512))
                im.putpalette(bytes(rgb))
                im.convert('RGB').save(os.path.join(frame_dir, 'f%04d.png' % i))
            except Exception:
                pass

        def sampler():
            last = None
            tstart = time.time()
            while not stop.is_set():
                try:
                    objs = dm.read(h, base, 0x482c30, 0x42 * 0x18)
                    q0 = dm.read(h, base, 0x4836f0, 0x800)
                    q1 = dm.read(h, base, 0x483ef0, 0x800)
                    bank = dm.read(h, base, 0x4836e8, 1)[0]
                    n = dm.read(h, base, 0x482044, 1)[0]
                except Exception:
                    break
                snap = (objs, q0, q1, bank, n)
                if snap != last:
                    samples.append((round(time.time() - tstart, 4), snap))
                    last = snap
                    if args.frames:
                        save_frame(idx[0])
                    idx[0] += 1
                time.sleep(0.001)

        th = threading.Thread(target=sampler, daemon=True)
        th.start()
        for vk in [SPACE, SPACE, RIGHT, RIGHT, SPACE, SPACE, RIGHT, DOWN, SPACE]:
            tap(vk, gap=0.30)
        if args.dumpbank:
            time.sleep(1.2)
            try:
                bankptr = struct.unpack('<I', dm.read(h, base, 0x480f14, 4))[0]
                bank = dm.read_abs(h, bankptr, 1024 * 900)
                with open(args.dumpbank, 'wb') as bf:
                    bf.write(bank)
                pal = dm.read(h, base, 0x480f20, 4 + 256 * 4)
                with open(args.dumpbank + '.pal', 'wb') as pf:
                    pf.write(pal)
                print('bank', hex(bankptr), '->', args.dumpbank)
            except Exception as ex:
                print('bank dump failed', ex)
        time.sleep(args.seconds)
        stop.set()
        th.join(timeout=2.0)

        frames = []
        for ts, (objs, q0, q1, bank, n) in samples:
            ol = []
            for k in range(0x42):
                e = objs[k * 0x18:(k + 1) * 0x18]
                if e[8] == 0:
                    continue
                x, y = struct.unpack_from('<HH', e, 0xc)
                ol.append([k, x, y, e[8], e[9], e[0xa], e[0xb], e[0x14], e.hex()])
            qe = []
            for b, raw in ((0, q0), (1, q1)):
                for k in range(0x100):
                    e = raw[k * 8:(k + 1) * 8]
                    a, c, pal, sp = struct.unpack_from('<HHHH', e, 0)
                    if a == 0 and c == 0:
                        continue
                    qe.append([b, a, c, pal, sp, k])
            frames.append({'t': ts, 'objs': ol, 'n': n, 'bank': bank, 'q': qe})
        path = os.path.join(OUT, args.tag + '.json')
        with open(path, 'w', encoding='utf-8') as f:
            json.dump({'atk': args.atk, 'dfn': args.dfn, 'frames': frames}, f)
        print('frames', len(frames), '->', path)
        return 0
    finally:
        try:
            dm.k32.CloseHandle(h)
        except Exception:
            pass
        try:
            subprocess.call(['taskkill', '/F', '/PID', str(pi.dwProcessId)],
                            stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        except Exception:
            pass


if __name__ == '__main__':
    sys.exit(main())
