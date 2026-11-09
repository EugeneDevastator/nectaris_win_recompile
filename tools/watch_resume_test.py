"""Check whether the debugged game actually keeps running."""
import ctypes, ctypes.wintypes as wt, os, sys, time, struct, hashlib
sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm
from watch_spritegen import DEBUG_EVENT, CREATE_PROCESS_DEBUG_EVENT, k32, find_pid

pid = find_pid()
h = dm.k32.OpenProcess(0x1F0FFF, False, pid)
base = dm.module_base(h)
fb = struct.unpack('<I', dm.read(h, base, 0x480f18, 4))[0]
print('fb', hex(fb), flush=True)


def fbhash():
    d = dm.read_abs(h, fb, 704 * 544)
    return hashlib.md5(d).hexdigest()[:10]


print('before', fbhash(), flush=True)
if not k32.DebugActiveProcess(pid):
    raise SystemExit('attach failed')
k32.DebugSetProcessKillOnExit(False)
print('attached', flush=True)
t0 = time.time()
continued = False
while time.time() - t0 < 5.0:
    ev = DEBUG_EVENT()
    if not k32.WaitForDebugEvent(ctypes.byref(ev), 300):
        break
    print('event', ev.dwDebugEventCode, 'tid', ev.dwThreadId, flush=True)
    k32.ContinueDebugEvent(ev.dwProcessId, ev.dwThreadId,
                           0x00010002 if ev.dwDebugEventCode == 1 else 0x00010002)
    continued = True
    if continued:
        break
time.sleep(1.0)
print('after1', fbhash(), flush=True)
time.sleep(1.0)
print('after2', fbhash(), flush=True)
k32.DebugActiveProcessStop(pid)
