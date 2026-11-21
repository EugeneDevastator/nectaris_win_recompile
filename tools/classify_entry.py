"""Classify combat entry motion from a sprite-queue capture.

For every 4-chunk unit group, print the first and last position and the net
movement.  Uses the queue field order decoded from the game: field0 (= our
entry 'y') is screen X, field1 (= our 'x') is screen Y, both before the
`*2-32` blitter scaling (so ratios are what matters).
"""
import glob
import json
import os
import sys


def load_groups(path, bank=0):
    frames = sorted(glob.glob(os.path.join(path, 'f*.json')))
    series = {}
    for i, f in enumerate(frames):
        try:
            entries = json.load(open(f))
        except Exception:
            continue
        by = {}
        for e in entries:
            if e['bank'] != bank or (e['x'] == 0 and e['y'] == 0):
                continue
            key = (e['pal'], e['sprite'] & ~3)
            g = by.setdefault(key, {'x': [], 'y': []})
            g['x'].append(e['x'])   # screen Y
            g['y'].append(e['y'])   # screen X
        for k, g in by.items():
            if len(g['x']) < 4:
                continue
            series.setdefault(k, []).append((i, min(g['y']), min(g['x'])))
    return series


def classify(a, b):
    dx = b[1] - a[1]
    dy = b[2] - a[2]
    if abs(dx) < 8 and abs(dy) < 8:
        return 'static'
    if abs(dy) <= max(4, abs(dx) * 0.3):
        return 'horizontal'
    if abs(dx) <= max(4, abs(dy) * 0.3):
        return 'vertical'
    return 'diagonal'


def main():
    path = sys.argv[1]
    bank = int(sys.argv[2]) if len(sys.argv) > 2 else 0
    series = load_groups(path, bank)
    for k, pts in sorted(series.items()):
        # keep only the entry portion: first 150 samples and before movement stops
        entry = [p for p in pts if p[0] < 200]
        if len(entry) < 3:
            continue
        first = entry[0]
        last = entry[-1]
        # find the point where motion stops (stable for 5 samples)
        end = entry[-1]
        for j in range(len(entry) - 1, 4, -1):
            if all(entry[j - m][1:] == entry[j][1:] for m in range(1, 5)):
                end = entry[j]
            else:
                break
        kind = classify(first, end)
        print('p%-3d s%-3d  %s  from %4d,%4d to %4d,%4d  d=%4d,%4d'
              % (k[0], k[1], kind, first[1], first[2], end[1], end[2],
                 end[1] - first[1], end[2] - first[2]))


if __name__ == '__main__':
    main()
