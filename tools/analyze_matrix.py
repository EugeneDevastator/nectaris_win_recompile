"""Turn captured battle-matrix queues into per-unit motion + orientation data.

Reads dump/battle_matrix/jobs/<id>/queue.jsonl (one queue snapshot per line) and
writes motion.json next to it:

  units[] = {
    pal, base0,
    orientations: [sprite bases in first-seen order],
    samples: [ {t, x, y, base, orient}, ... ]
  }

`base` is the unit's 4-chunk sprite base (sprite & ~3); `orient` is the index of
that base in the unit's orientation list, so a position can later be mapped to
the corresponding impostor sprite (rotation/scaled variant).

Usage:
  python tools/analyze_matrix.py [job_dir ...]
"""
import glob
import json
import os
import sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
JOBS = os.path.join(ROOT, 'dump', 'battle_matrix', 'jobs')


def analyze(job_dir):
    qpath = os.path.join(job_dir, 'queue.jsonl')
    if not os.path.exists(qpath):
        return 0
    groups = {}
    frames = 0
    with open(qpath, encoding='utf-8') as f:
        for line in f:
            try:
                entries = json.loads(line)
            except Exception:
                continue
            t = frames * 0.02
            frames += 1
            by = {}
            for bank, x, y, pal, sprite in entries:
                if bank != 0:
                    continue
                key = (pal, sprite & ~3)
                g = by.setdefault(key, {'xs': [], 'ys': [], 'sprites': set()})
                g['xs'].append(x)
                g['ys'].append(y)
                g['sprites'].add(sprite)
            for key, g in by.items():
                if len(g['sprites']) < 4:
                    continue
                u = groups.setdefault(key, {
                    'pal': key[0], 'base0': key[1], 'orientations': [],
                    'samples': []})
                base = key[1]
                if base not in u['orientations']:
                    u['orientations'].append(base)
                u['samples'].append({
                    't': round(t, 3),
                    'x': min(g['xs']),
                    'y': min(g['ys']),
                    'base': base,
                    'orient': u['orientations'].index(base),
                })
    units = [u for u in groups.values() if len(u['samples']) >= 5]
    for u in units:
        u['samples'].sort(key=lambda s: s['t'])
    out = {'units': units, 'frames': frames, 'sample_seconds': 0.02}
    with open(os.path.join(job_dir, 'motion.json'), 'w', encoding='utf-8') as f:
        json.dump(out, f, indent=1)
    return len(units)


def main():
    dirs = sys.argv[1:]
    if not dirs:
        dirs = sorted(os.path.dirname(p) for p in glob.glob(os.path.join(JOBS, '*', 'queue.jsonl')))
    total = 0
    for d in dirs:
        n = analyze(d)
        if n:
            print('%s: %d units' % (os.path.basename(d.rstrip('/\\')), n))
            total += 1
    print('analyzed', total, 'jobs')


if __name__ == '__main__':
    main()
