"""Resumable capture matrix for battle entry motion.

For every (attacker type x defender type x terrain) job the script:
  1. boots the original game to a map,
  2. patches all player/enemy unit types and the terrain under both units,
  3. triggers a real adjacent attack with the scripted key sequence,
  4. samples the live sprite queue (0x4836f0 / 0x483ef0) at ~50 Hz and writes
     dump/battle_matrix/jobs/<id>/queue.jsonl plus meta.json.

State lives in dump/battle_matrix/state.json, so an interrupted run continues
where it stopped: completed jobs are skipped, failed ones retry up to
--retries, and "running" entries are reset to pending on startup.

Usage:
  python tools/capture_battle_matrix.py [--limit N] [--retries 2] [--fresh]
                                        [--only JOB_ID]
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
import traceback

sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm  # noqa: E402

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
GAME = os.path.join(ROOT, 'game')
OUT = os.path.join(ROOT, 'dump', 'battle_matrix')
STATE = os.path.join(OUT, 'state.json')

ATTACKERS = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15, 16, 18, 19, 20]
DEF_GROUND = [6, 18]
DEF_AIR = [1]
TERRAINS = [1, 2, 3, 6, 9, 11]     # grass, forest, mountain, city, road, base

# fake-input shared memory (same layout as the other capture tools)
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

# The fake-input mapping must exist before the game starts: its winmm proxy
# opens "Local\\NecFakeInput" during init, so create and initialise it once.
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


def load_units():
    with open(os.path.join(ROOT, 'assets', 'data', 'units.json'), encoding='utf-8') as f:
        units = json.load(f)['units']
    return {u['id']: u for u in units}


def build_jobs():
    units = load_units()
    jobs = []
    for a in ATTACKERS:
        if a not in units:
            continue
        ua = units[a]
        targets = []
        if ua.get('attack_ground', 0) > 0:
            targets += DEF_GROUND
        if ua.get('attack_air', 0) > 0:
            targets += DEF_AIR
        for d in targets:
            for t in TERRAINS:
                jobs.append({
                    'id': 'a%02d_d%02d_t%02d' % (a, d, t),
                    'atk': a, 'def': d,
                    'terrain_atk': t, 'terrain_def': t,
                })
    return jobs


def load_state():
    if os.path.exists(STATE):
        with open(STATE, encoding='utf-8') as f:
            return json.load(f)
    return {}


def save_state(state):
    os.makedirs(OUT, exist_ok=True)
    tmp = STATE + '.tmp'
    with open(tmp, 'w', encoding='utf-8') as f:
        json.dump(state, f, indent=1)
    os.replace(tmp, STATE)


def kill_games():
    try:
        out = subprocess.check_output(['tasklist', '/FI', 'IMAGENAME eq Nec.exe', '/FO', 'CSV', '/NH'],
                                      text=True)
    except Exception:
        return
    for line in out.splitlines():
        parts = [x.strip('"') for x in line.split('","')]
        if parts and parts[0].lower() == 'nec.exe':
            subprocess.call(['taskkill', '/F', '/PID', parts[1]],
                            stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)


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


def run_job(job):
    kill_games()
    time.sleep(0.35)
    proc = subprocess.Popen([os.path.join(GAME, 'Nec.exe')], cwd=GAME,
                            stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    hwnd = None
    for _ in range(200):
        hwnd = find_window(proc.pid)
        if hwnd:
            break
        time.sleep(0.05)
    if not hwnd:
        proc.kill()
        raise RuntimeError('game window not found')
    h = dm.k32.OpenProcess(0x1F0FFF, False, proc.pid)
    if not h:
        proc.kill()
        raise RuntimeError('OpenProcess failed')
    base = dm.module_base(h)

    def u8(va):
        return dm.read(h, base, va, 1)[0]

    def u32(va):
        return struct.unpack('<I', dm.read(h, base, va, 4))[0]

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
        # ---- boot to the tactical map
        focus()
        t0 = time.time()
        toggle = False
        while time.time() - t0 < 50:
            if u8(0x495c02) == 15 and u8(0x495c03) == 10:
                break
            tap(SPACE if not toggle else ENTER, gap=0.40)
            toggle = not toggle
        if not (u8(0x495c02) == 15 and u8(0x495c03) == 10):
            raise RuntimeError('map did not load')
        time.sleep(2.5)

        # ---- patch unit types (all blue = attacker type, all green = defender)
        types = dm.read(h, base, 0x4957a0, 0x38)
        for slot in range(0x38):
            t = types[slot]
            if t == 0xff:
                continue
            owner = t & 0x80
            wr(0x4957a0 + slot, bytes([owner | (job['def'] if owner else job['atk'])]))
        # Full formations so the 8 front/rear unit positions are present.
        for slot in range(0x38):
            if types[slot] != 0xff:
                wr(0x4957e0 + slot, bytes([7]))

        # ---- patch terrain under the first blue and first green unit
        types = dm.read(h, base, 0x4957a0, 0x38)
        st = dm.read(h, base, 0x495ab0, 0x38)
        fl = dm.read(h, base, 0x495af0, 0x38)
        atk_slot = def_slot = -1
        for slot in range(0x38):
            t = types[slot]
            if t == 0xff:
                continue
            if (t & 0x80) == 0 and atk_slot < 0:
                atk_slot = slot
            elif (t & 0x80) != 0 and def_slot < 0:
                def_slot = slot
        if atk_slot < 0 or def_slot < 0:
            raise RuntimeError('no unit slots')
        ax, ay = st[atk_slot] & 0x1f, fl[atk_slot] & 0x1f
        dx, dy = st[def_slot] & 0x1f, fl[def_slot] & 0x1f
        wr(0x481a80 + ay * 32 + ax, bytes([job['terrain_atk'] & 0x0f]))
        wr(0x481a80 + dy * 32 + dx, bytes([job['terrain_def'] & 0x0f]))
        wr(0x481db3, bytes([job['terrain_atk'] & 0x0f]))
        wr(0x481db4, bytes([job['terrain_def'] & 0x0f]))
        wr(0x481dbc, bytes([0]))

        # ---- navigate to the two units and attack (standard mission-1 layout)
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
        for vk in [SPACE, SPACE, RIGHT, RIGHT, SPACE, SPACE, RIGHT, DOWN, SPACE]:
            tap(vk, gap=0.35)

        # ---- sample the queue until combat units appear and settle
        job_dir = os.path.join(OUT, 'jobs', job['id'])
        os.makedirs(job_dir, exist_ok=True)
        qpath = os.path.join(job_dir, 'queue.jsonl')
        t1 = time.time()
        started = False
        samples = 0

        def units_present(entries):
            cnt = {}
            for bank, x, y, pal, sprite in entries:
                if bank != 0:
                    continue
                key = (pal, sprite & ~3)
                cnt[key] = cnt.get(key, 0) + 1
            return sum(1 for v in cnt.values() if v >= 4) >= 2

        with open(qpath, 'w', encoding='utf-8') as qf:
            while time.time() - t1 < 10.0:
                raw0 = dm.read(h, base, 0x4836f0, 0x800)
                raw1 = dm.read(h, base, 0x483ef0, 0x800)
                entries = []
                for bank, raw in ((0, raw0), (1, raw1)):
                    for i in range(0x100):
                        e = raw[i * 8:(i + 1) * 8]
                        y, x = struct.unpack_from('<HH', e, 0)
                        if x == 0 and y == 0:
                            continue
                        entries.append([bank, x, y, e[4], e[5] | (e[7] << 8)])
                if not started:
                    started = units_present(entries)
                if started:
                    qf.write(json.dumps(entries) + '\n')
                    samples += 1
                if started and samples > 320:
                    break
                time.sleep(0.02)
        if not started or samples < 30:
            raise RuntimeError('battle did not run (samples=%d)' % samples)
        meta = {
            'job': job,
            'samples': samples,
            'duration': round(time.time() - t1, 2),
            'atk_slot': atk_slot, 'def_slot': def_slot,
            'cells': {'atk': [ax, ay], 'def': [dx, dy]},
            'queue': qpath,
            'finished': time.strftime('%Y-%m-%d %H:%M:%S'),
        }
        with open(os.path.join(job_dir, 'meta.json'), 'w', encoding='utf-8') as f:
            json.dump(meta, f, indent=1)
        return samples
    finally:
        try:
            dm.k32.CloseHandle(h)
        except Exception:
            pass
        try:
            proc.kill()
        except Exception:
            pass


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--limit', type=int, default=0)
    ap.add_argument('--retries', type=int, default=2)
    ap.add_argument('--fresh', action='store_true')
    ap.add_argument('--only', default='')
    args = ap.parse_args()

    os.makedirs(OUT, exist_ok=True)
    jobs = build_jobs()
    state = {} if args.fresh else load_state()
    # interrupted jobs go back to pending
    for jid, s in state.items():
        if s.get('status') == 'running':
            s['status'] = 'pending'

    todo = []
    for job in jobs:
        jid = job['id']
        if args.only and jid != args.only:
            continue
        s = state.setdefault(jid, {'status': 'pending', 'attempts': 0})
        if s['status'] == 'done':
            continue
        if s['attempts'] >= args.retries and s['status'] == 'failed':
            continue
        todo.append(job)
    print('jobs', len(jobs), 'todo', len(todo), flush=True)

    done_count = 0
    for job in todo:
        jid = job['id']
        s = state.setdefault(jid, {'status': 'pending', 'attempts': 0})
        s['attempts'] += 1
        s['status'] = 'running'
        s['started'] = time.strftime('%Y-%m-%d %H:%M:%S')
        save_state(state)
        try:
            n = run_job(job)
            s['status'] = 'done'
            s['samples'] = n
            print('[done] %s samples=%d' % (jid, n), flush=True)
        except Exception as e:
            s['status'] = 'pending' if s['attempts'] < args.retries else 'failed'
            s['error'] = '%s: %s' % (type(e).__name__, e)
            print('[fail] %s (%d/%d) %s' % (jid, s['attempts'], args.retries, s['error']), flush=True)
            traceback.print_exc()
        save_state(state)
        done_count += 1
        if args.limit and done_count >= args.limit:
            print('limit reached', flush=True)
            break
    print('done this run:', done_count, flush=True)


if __name__ == '__main__':
    main()
