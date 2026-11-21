"""Extract full per-frame entry paths from one_run captures.

Reads dump/one_run/*.json (produced by tools/capture_one_run.py) and writes
dump/one_run_paths.json with, per unit type, a sampled path [t, dx, dy] where
offsets are original queue units relative to the settled slot.

Usage:
  python tools/extract_full_paths.py [run.json ...]
"""
import glob
import json
import os
import sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
RUNS = os.path.join(ROOT, 'dump', 'one_run')


def groups(entries):
    by = {}
    for bank, x, y, pal, sprite in entries:
        if bank != 0 or (x == 0 and y == 0):
            continue
        k = (pal, sprite & ~3)
        g = by.setdefault(k, {'x': [], 'y': []})
        g['x'].append(x)   # field1 = screen Y
        g['y'].append(y)   # field0 = screen X
    return {k: (min(g['y']), min(g['x'])) for k, g in by.items() if len(g['x']) >= 4}


def main():
    files = sys.argv[1:] or sorted(glob.glob(os.path.join(RUNS, '*.json')))
    per_type = {}
    for path in files:
        data = json.load(open(path, encoding='utf-8'))
        atk, dfn = data['atk'], data['def']
        frames = data['frames']
        # build per-group series
        series = {}
        for fi, f in enumerate(frames):
            for k, (x, y) in groups(f['e']).items():
                series.setdefault(k, []).append((fi, x, y))
        for k, pts in series.items():
            if len(pts) < 10:
                continue
            # entry = from first appearance until stable for 6 frames
            end = len(pts) - 1
            for j in range(len(pts) - 1, 6, -1):
                if all(pts[j - m][1:] == pts[j][1:] for m in range(1, 6)):
                    end = j
                else:
                    break
            entry = pts[:end + 1]
            if len(entry) < 6:
                continue
            utype = atk if k[0] < 64 else dfn
            sx, sy = entry[-1][1], entry[-1][2]
            if abs(entry[0][1] - sx) < 8 and abs(entry[0][2] - sy) < 8:
                continue
            t0 = entry[0][0]
            dur = max(1, entry[-1][0] - t0)
            norm = [[round((p[0] - t0) / dur, 3), p[1] - sx, p[2] - sy] for p in entry[::max(1, len(entry) // 16)]]
            if norm[-1][0] < 0.999:
                norm.append([1.0, 0, 0])
            per_type.setdefault(utype, []).append(norm)

    out = {}
    for t, paths in per_type.items():
        # use the path with the largest start offset (most complete entry)
        best = max(paths, key=lambda p: abs(p[0][1]) + abs(p[0][2]))
        out[str(t)] = {
            'frames': len(best) - 1,
            'path': best,
        }
    with open(os.path.join(ROOT, 'dump', 'one_run_paths.json'), 'w', encoding='utf-8') as f:
        json.dump(out, f, indent=1)
    for t, v in sorted(out.items()):
        print('type', t, 'points', len(v['path']), 'start', v['path'][0],
              'mid', v['path'][len(v['path']) // 2], 'end', v['path'][-1])


if __name__ == '__main__':
    main()
