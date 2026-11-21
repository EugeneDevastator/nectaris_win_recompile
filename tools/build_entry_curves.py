"""Build the real per-slot entry curves from a headless object capture.

Object layout (live sprite-object table, capture_battle_objects.py):

    +0x0c : int32 X (integer part in the low word)
    +0x10 : int16 Y

Queue -> screen relation measured from the game:

    screenX = 2*queueX + 32   with queueX = objectX - 32
    screenY = 2*queueY - 32   with queueY = objectY - 48

so  screenX = 2*objX - 32, screenY = 2*objY - 128.

The units are parked at the panel edges when the battle scene opens; each slot
then slides in at its own time.  Only the entry window is stored (impact
effects re-use the object slots after ~7.45 s).

Usage: python tools/build_entry_curves.py [dump/object_runs/objraw.json]
"""
import json
import os
import struct
import sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
PANEL = {'left': (64, 96), 'right': (320, 96)}
SIZE = 256.0
ENTRY_LIMIT = 7.45


def parse_obj(raw):
    w = struct.unpack('<12H', raw)
    return w[6], w[8]                    # X low word, Y


def screen(x, y):
    return 2 * x - 32, 2 * y - 128


def norm(side, x, y):
    x0, y0 = PANEL[side]
    return [(x - x0) / SIZE, (y - y0) / SIZE]


def main():
    src = sys.argv[1] if len(sys.argv) > 1 else os.path.join(ROOT, 'dump', 'object_runs', 'objraw.json')
    data = json.load(open(src, encoding='utf-8'))
    frames = data['frames']
    t_origin = None

    # scene-open snapshot: units parked at the panel edges
    park = None
    for f in frames:
        if f['t'] < 4.9 or len(f['objs']) < 14:
            continue
        pts = {}
        for o in f['objs']:
            if o[0] < 2:
                continue
            x, y = parse_obj(bytes.fromhex(o[8]))
            if (x <= 40 or x >= 290) and y:
                pts[o[0]] = (x, y)
        if len(pts) >= 14:
            park = pts
            t_origin = f['t']
            print('park frame t=%.2f slots=%d' % (f['t'], len(pts)))
            break
    if not park:
        print('no parked frame')
        return

    series = {}
    for f in frames:
        if f['t'] > ENTRY_LIMIT:
            break
        if f['t'] < 4.88:
            continue
        for o in f['objs']:
            if o[0] in park:
                x, y = parse_obj(bytes.fromhex(o[8]))
                if x:
                    series.setdefault(o[0], []).append((f['t'], x, y))

    out = {'left': [], 'right': []}
    for slot, (px, py) in sorted(park.items()):
        seq = series.get(slot, [])
        if len(seq) < 40:
            continue
        side = 'left' if px < 168 else 'right'
        start = None
        for i, (t, x, y) in enumerate(seq):
            if abs(x - px) > 2:
                start = i
                break
        if start is None:
            continue
        end = None
        for i in range(start, len(seq)):
            t = seq[i][0]
            j = i
            while j + 1 < len(seq) and seq[j + 1][0] - t < 0.18:
                j += 1
            if j > i and all(abs(seq[k][1] - seq[i][1]) <= 2 for k in range(i, j + 1)):
                end = j
                break
        if end is None or end <= start:
            continue
        dur = seq[end][0] - seq[start][0]
        if dur < 0.30:
            continue

        sm = []
        last = None
        for (t, x, y) in seq[start:end + 1]:
            if last is not None and t - last < 0.02:
                continue
            last = t
            sx, sy = screen(x, y)
            sm.append([round(t - seq[start][0], 3)] + [round(v, 4) for v in norm(side, sx, sy)])

        ex, ey = seq[end][1], seq[end][2]
        sx, sy = screen(ex, ey)
        stx, sty = screen(px, py)
        out[side].append({
            'slot': [round(v, 4) for v in norm(side, sx, sy)],
            'start': [round(v, 4) for v in norm(side, stx, sty)],
            't0': round(max(0.0, seq[start][0] - t_origin), 3),
            'duration': round(dur, 3),
            'samples': sm,
        })

    for side in out:
        out[side].sort(key=lambda s: (s['slot'][0], s['slot'][1]))
        print(side, len(out[side]))
        for s in out[side]:
            print('  t0 %5.2f dur %.2f  start (%6.3f,%6.3f) slot (%6.3f,%6.3f) n=%d'
                  % (s['t0'], s['duration'], s['start'][0], s['start'][1],
                     s['slot'][0], s['slot'][1], len(s['samples'])))

    dst = os.path.join(ROOT, 'assets', 'data', 'combat_entry_curves.json')
    with open(dst, 'w', encoding='utf-8') as f:
        json.dump(out, f, indent=1)
    print('wrote', dst)


if __name__ == '__main__':
    main()
