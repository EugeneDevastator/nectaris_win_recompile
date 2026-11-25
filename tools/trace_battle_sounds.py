"""Replay the recorded mission-1 session and trace every sound the original queues.

Polls DAT_0042fb90 (the game's "play this SE next frame" cell) every 4 ms while
driving the same recorded input as tools/replay_session.py.  Each observed id is
the 1-based SE number, i.e. id 28 == assets/audio/se28.wav.  Writes
ref/battle_sound_trace.csv with time, id, attacker/defender, mode and task.
"""
import ctypes
import ctypes.wintypes as wt
import json
import os
import struct
import subprocess
import sys
import time

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
GAME = os.path.join(ROOT, 'game')
OUT = os.path.join(ROOT, 'ref', 'battle_sound_trace.csv')
sys.path.insert(0, os.path.join(ROOT, 'tools'))
import dump_mem as dm

UNTIL = 95.0

K32 = ctypes.WinDLL('kernel32', use_last_error=True)
K32.CreateFileMappingW.restype = wt.HANDLE
K32.CreateFileMappingW.argtypes = [wt.HANDLE, ctypes.c_void_p, wt.DWORD, wt.DWORD,
                                   wt.DWORD, wt.LPCWSTR]
K32.MapViewOfFile.restype = ctypes.c_void_p
K32.MapViewOfFile.argtypes = [wt.HANDLE, wt.DWORD, wt.DWORD, wt.DWORD, ctypes.c_size_t]
hmap = K32.CreateFileMappingW(wt.HANDLE(-1).value, None, 0x04, 0, 4096,
                              "Local\\NecFakeInput")
ptr = K32.MapViewOfFile(hmap, 0xF001F, 0, 0, 4096)
ctypes.memset(ptr, 0, 4096)

MAGIC = 0x4E454331


def set_i32(off, v):
    ctypes.c_long.from_address(ptr + off).value = v


def set_key(vk, down):
    ctypes.c_ubyte.from_address(ptr + 36 + vk).value = 1 if down else 0


set_i32(0, MAGIC)
set_i32(4, 1)

U32 = ctypes.WinDLL('user32', use_last_error=True)
U32.EnumWindows.argtypes = [ctypes.WINFUNCTYPE(wt.BOOL, wt.HWND, wt.LPARAM), wt.LPARAM]
U32.GetWindowThreadProcessId.argtypes = [wt.HWND, ctypes.POINTER(wt.DWORD)]
U32.GetWindowTextW.argtypes = [wt.HWND, wt.LPWSTR, ctypes.c_int]
U32.ClientToScreen.argtypes = [wt.HWND, ctypes.POINTER(wt.POINT)]
U32.SetForegroundWindow.argtypes = [wt.HWND]
U32.PostMessageW.argtypes = [wt.HWND, wt.UINT, wt.WPARAM, wt.LPARAM]
U32.MapVirtualKeyW.argtypes = [wt.UINT, wt.UINT]


def find_window(pid):
    found = []

    @ctypes.WINFUNCTYPE(wt.BOOL, wt.HWND, wt.LPARAM)
    def cb(hwnd, lp):
        p = wt.DWORD()
        U32.GetWindowThreadProcessId(hwnd, ctypes.byref(p))
        if p.value == pid:
            buf = ctypes.create_unicode_buffer(256)
            U32.GetWindowTextW(hwnd, buf, 256)
            if 'nectaris' in buf.value.lower():
                found.append(hwnd)
        return True

    U32.EnumWindows(cb, 0)
    return found[0] if found else None


def load_events():
    ev = []
    with open(os.path.join(ROOT, 'ref', 'session', 'input.jsonl'), encoding='utf-8') as f:
        for line in f:
            try:
                e = json.loads(line)
            except Exception:
                continue
            if e.get('ev') in ('move', 'mdown', 'mup', 'kdown', 'kup'):
                ev.append(e)
    ev.sort(key=lambda e: e['t'])
    return ev


def main():
    ev = load_events()
    print('events', len(ev), 'until', ev[-1]['t'], flush=True)
    p = subprocess.Popen([os.path.join(GAME, 'Nec.exe')], cwd=GAME)
    hwnd = None
    for _ in range(200):
        hwnd = find_window(p.pid)
        if hwnd:
            break
        time.sleep(0.05)
    if not hwnd:
        print('no window', flush=True)
        return
    time.sleep(0.5)
    U32.SetForegroundWindow(hwnd)

    h = dm.k32.OpenProcess(0x1F0FFF, False, p.pid)
    base = dm.module_base(h)

    def u8(va):
        return dm.read(h, base, va, 1)[0]

    def u32(va):
        return struct.unpack('<I', dm.read(h, base, va, 4))[0]

    def post_key(vk, down):
        sc = U32.MapVirtualKeyW(vk, 0) & 0xFF
        if down:
            U32.PostMessageW(hwnd, 0x0100, vk, 1 | (sc << 16))
            U32.PostMessageW(hwnd, 0x0102, vk, 1 | (sc << 16))
        else:
            U32.PostMessageW(hwnd, 0x0101, vk, 1 | (sc << 16) | 0xC0000000)

    def post_mouse(cx, cy, down=None):
        lp = ((cy & 0xFFFF) << 16) | (cx & 0xFFFF)
        U32.PostMessageW(hwnd, 0x0200, 0, lp)
        if down is True:
            U32.PostMessageW(hwnd, 0x0201, 1, lp)
        elif down is False:
            U32.PostMessageW(hwnd, 0x0202, 0, lp)

    buttons = 0
    t0 = time.monotonic()
    idx = 0
    last_id = 0
    rows = []
    last_report = -1
    while True:
        now = time.monotonic() - t0
        if now > UNTIL:
            break
        while idx < len(ev) and ev[idx]['t'] <= now:
            e = ev[idx]
            idx += 1
            k = e['ev']
            if k in ('move', 'mdown', 'mup'):
                cx, cy = e.get('cx', 0), e.get('cy', 0)
                if cx < 0 or cy < 0:
                    continue
                org = wt.POINT(0, 0)
                U32.ClientToScreen(hwnd, ctypes.byref(org))
                set_i32(12, org.x + cx)
                set_i32(16, org.y + cy)
                set_i32(20, cx)
                set_i32(24, cy)
                if k == 'mdown':
                    buttons |= 1
                    set_i32(28, buttons)
                    post_mouse(cx, cy, True)
                elif k == 'mup':
                    buttons &= ~1
                    set_i32(28, buttons)
                    post_mouse(cx, cy, False)
                else:
                    post_mouse(cx, cy)
            else:
                vk = e.get('vk', 0)
                set_key(vk, k == 'kdown')
                post_key(vk, k == 'kdown')
        try:
            sid = u32(0x42fb90)
        except Exception:
            sid = 0
        if sid != last_id:
            last_id = sid
            if 0 < sid < 0x40:
                atk = u8(0x48327b)
                dfn = u8(0x483274)
                ind = u8(0x481dbc)
                task = u32(0x482330)
                rows.append((now, sid, atk, dfn, ind, task))
        if int(now) != last_report:
            last_report = int(now)
            print('t=%d id=%d atk=%d def=%d ind=%d task=%x' % (
                now, sid, u8(0x48327b), u8(0x483274), u8(0x481dbc),
                u32(0x482330)), flush=True)
        time.sleep(0.004)
    with open(OUT, 'w', encoding='utf-8') as f:
        f.write('t,se_id,atk,def,mode,task\n')
        for r in rows:
            f.write('%.3f,%d,%d,%d,%d,%x\n' % r)
    print('wrote', OUT, len(rows), 'sound events', flush=True)
    subprocess.run(['taskkill', '/IM', 'Nec.exe', '/F'], capture_output=True)


if __name__ == '__main__':
    main()
