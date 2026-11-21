"""Track combat unit sprite groups from the memory-dumped sprite queue.

dump/combat_full/frames/fNNN.json holds the raw 0x4836f0 sprite queue (bank 0 =
one battle panel, bank 1 = the other).  Each 64x64 battle unit is four 16x16
chunks with consecutive sprite ids, so a group is (palette, sprite & ~3) and its
origin is the min x/y of the four chunks.  Prints one line per frame with the
group origins, which is the actual on-screen motion read from memory.
"""
import glob
import json
import os
import sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
FRAMES = os.path.join(ROOT, 'dump', 'combat_full', 'frames')

def groups(bank, entries):
    by = {}
    for e in entries:
        if e['bank'] != bank:
            continue
        if e['x'] == 0 and e['y'] == 0:
            continue
        key = (e['pal'], e['sprite'] & ~3)
        g = by.setdefault(key, {'x': [], 'y': [], 'sprites': set()})
        g['x'].append(e['x'])
        g['y'].append(e['y'])
        g['sprites'].add(e['sprite'])
    out = []
    for key, g in by.items():
        if len(g['sprites']) < 4:
            continue
        out.append((key, min(g['x']), min(g['y'])))
    return sorted(out, key=lambda t: (t[0][0], t[0][1]))


def main():
    global FRAMES
    bank = int(sys.argv[1]) if len(sys.argv) > 1 else 0
    f0 = int(sys.argv[2]) if len(sys.argv) > 2 else 0
    f1 = int(sys.argv[3]) if len(sys.argv) > 3 else 163
    if len(sys.argv) > 4:
        FRAMES = sys.argv[4]
    files = sorted(glob.glob(os.path.join(FRAMES, 'f*.json')))
    for i in range(f0, min(f1, len(files))):
        try:
            entries = json.load(open(files[i]))
        except Exception:
            continue
        gs = groups(bank, entries)
        if not gs:
            print('%3d  -' % i)
            continue
        print('%3d  %s' % (i, '  '.join('p%d/s%3d %3d,%3d' % (k[0], k[1], x, y)
                                         for k, x, y in gs)))


if __name__ == '__main__':
    main()
