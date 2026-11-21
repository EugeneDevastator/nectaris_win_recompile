"""Extract full entry paths from the memory sprite-queue captures.

For every 4-chunk unit group, records the (X, Y) sequence from first appearance
to the settled slot and writes a normalised path: offsets from the final slot
over normalised time 0..1.  Output goes to dump/entry_paths.json and is also
printed for inspection.

Usage:
  python tools/extract_entry_paths.py [capture_dir ...]
"""
import glob
import json
import os
import sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
DEFAULT_DIRS = [
    'dump/battle_sprites_mem/frames',
    'dump/battle_sprites_mem_air/frames',
    'dump/battle_sprites_mem_vsair2/frames',
    'dump/battle_sprites_mem_t18/frames',
    'dump/battle_sprites_mem_t20/frames',
    'dump/battle_sprites_mem_t15/frames',
    'dump/battle_sprites_mem_t3/frames',
    'dump/battle_sprites_mem_t19/frames',
    'dump/battle_sprites_mem_t5/frames',
]


def groups(path, bank=0):
    qpath = os.path.join(path, 'queue.jsonl')
    frames = []
    if os.path.exists(qpath):
        with open(qpath, encoding='utf-8') as f:
            for line in f:
                try:
                    raw = json.loads(line)
                except Exception:
                    continue
                frames.append([(e[0], e[1], e[2], e[3], e[4]) for e in raw])
    else:
        for p in sorted(glob.glob(os.path.join(path, 'f*.json'))):
            try:
                entries = json.load(open(p))
            except Exception:
                continue
            frames.append([(e['bank'], e['x'], e['y'], e['pal'], e['sprite']) for e in entries])
    series = {}
    for i, entries in enumerate(frames):
        by = {}
        for bank_id, x, y, pal, sprite in entries:
            if bank_id != bank or (x == 0 and y == 0):
                continue
            key = (pal, sprite & ~3)
            g = by.setdefault(key, {'x': [], 'y': []})
            g['x'].append(y)   # screen Y
            g['y'].append(x)   # screen X
        for k, g in by.items():
            if len(g['x']) < 4:
                continue
            series.setdefault(k, []).append((i, min(g['x']), min(g['y'])))
    return series


def settle_index(pts, stable=4):
    """Index where the position stops changing for `stable` samples."""
    last = len(pts) - 1
    for j in range(last, stable, -1):
        if all(pts[j - m][1:] == pts[j][1:] for m in range(1, stable + 1)):
            last = j
        else:
            break
    return last


def main():
    if sys.argv[1:]:
        dirs = sys.argv[1:]
    else:
        dirs = [d for d in DEFAULT_DIRS if os.path.isdir(os.path.join(ROOT, d))]
        dirs += sorted(glob.glob(os.path.join(ROOT, 'dump', 'battle_matrix', 'jobs', '*')))
    out = {}
    for d in dirs:
        path = os.path.join(ROOT, d) if not os.path.isabs(d) else d
        series = groups(path)
        base = os.path.basename(path.rstrip('/\\'))
        name = os.path.basename(os.path.dirname(path.rstrip('/\\'))) if base == 'frames' else base
        entries = []
        for k, pts in sorted(series.items()):
            end_i = settle_index(pts)
            if end_i < 2:
                continue
            entry_pts = pts[:end_i + 1]
            # ignore effect sprites: their whole path is a long sweep with no
            # stable tail, or they move after settle
            sx, sy = entry_pts[-1][1], entry_pts[-1][2]
            start = entry_pts[0]
            if abs(start[1] - sx) < 6 and abs(start[2] - sy) < 6:
                continue
            dur = max(1, entry_pts[-1][0] - entry_pts[0][0])
            norm = []
            step = max(1, len(entry_pts) // 12)
            for p in entry_pts[::step]:
                t = (p[0] - entry_pts[0][0]) / dur
                norm.append([round(t, 3), p[1] - sx, p[2] - sy])
            if norm[-1][0] < 0.999:
                norm.append([1.0, 0, 0])
            entries.append({'pal': k[0], 'sprite': k[1],
                            'slot_x': sx, 'slot_y': sy,
                            'duration_samples': dur,
                            'path': norm})
        out[name] = entries
        print('==', name, len(entries), 'paths')
        for e in entries[:12]:
            pts = ' '.join('%.2f:%+d,%+d' % (p[0], p[1], p[2]) for p in e['path'])
            print('  p%-3d s%-3d slot %3d,%3d  %s' % (e['pal'], e['sprite'],
                                                      e['slot_x'], e['slot_y'], pts))
    with open(os.path.join(ROOT, 'dump', 'entry_paths.json'), 'w', encoding='utf-8') as f:
        json.dump(out, f, indent=1)
    print('wrote dump/entry_paths.json')


if __name__ == '__main__':
    main()
