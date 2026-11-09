"""List or patch tactical-map unit types in the running game.

Unit state arrays (base-relative VAs):
  0x4957a0  slot: type (bits 0..4) | owner (0x80 = player)
  0x4957e0  slot: hp (bits 0..2) | rank (bits 5..)
  0x495ab0  slot: x
  0x495af0  slot: y

Usage:
  python tools/patch_unit_type.py --list
  python tools/patch_unit_type.py --slot 3 --type 6 [--owner 0x80]
"""
import ctypes, os, sys, struct, subprocess
sys.path.insert(0, os.path.dirname(__file__))
import dump_mem as dm

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
TYPES = dm.k32  # silence
import json
units = json.load(open(os.path.join(ROOT, 'assets', 'data', 'units.json'), encoding='utf-8'))['units']
names = {u['id']: u['name'] for u in units}


def find_pid():
    for line in subprocess.check_output(['tasklist', '/FI', 'IMAGENAME eq Nec.exe', '/FO', 'CSV', '/NH'],
                                        text=True).splitlines():
        p = [x.strip('"') for x in line.split('","')]
        if p and p[0].lower() == 'nec.exe':
            return int(p[1])
    return None


def main():
    pid = find_pid()
    if pid is None:
        raise SystemExit('Nec.exe not running')
    h = dm.k32.OpenProcess(0x1F0FFF, False, pid)
    base = dm.module_base(h)
    to, hp, xs, ys = (dm.read(h, base, a, 0x38) for a in (0x4957a0, 0x4957e0, 0x495ab0, 0x495af0))

    args = sys.argv[1:]
    if not args or '--list' in args:
        print('slot own type name         hp rank x  y')
        for i in range(0x38):
            t = to[i]
            if t == 0 and xs[i] == 0 and ys[i] == 0:
                continue
            typ = t & 0x1F
            own = 'P' if t & 0x80 else 'E'
            if typ >= 0x1F:
                continue
            print('%3d  %s  %3d %-12s %2d %2d %2d %2d' %
                  (i, own, typ, names.get(typ, '?'), hp[i] & 7, hp[i] >> 5, xs[i] & 31, ys[i] & 31))
        return

    def getarg(name, default=None):
        if name in args:
            return args[args.index(name) + 1]
        return default

    slot = int(getarg('--slot'), 0)
    typ = int(getarg('--type'), 0)
    owner = getarg('--owner')
    cur = to[slot]
    new = (cur & 0x80) if owner is None else (0x80 if int(owner, 0) else 0)
    new |= typ & 0x1F
    buf = bytes([new])
    got = ctypes.c_size_t()
    addr = base + (0x4957a0 + slot - 0x400000)
    ok = dm.k32.WriteProcessMemory(h, ctypes.c_void_p(addr), buf, 1, ctypes.byref(got))
    print('slot %d type %d -> %d (%s) owner %d ok=%s' %
          (slot, cur & 0x1F, typ, names.get(typ, '?'), 0x80 if new & 0x80 else 0, ok))

    for name, table, va in (('x', xs, 0x495ab0), ('y', ys, 0x495af0)):
        val = getarg('--' + name)
        if val is not None:
            b = bytes([int(val, 0) & 0x1F])
            a = base + (va + slot - 0x400000)
            dm.k32.WriteProcessMemory(h, ctypes.c_void_p(a), b, 1, ctypes.byref(got))
            print('slot %d %s -> %d' % (slot, name, int(val, 0) & 0x1F))


if __name__ == '__main__':
    main()
