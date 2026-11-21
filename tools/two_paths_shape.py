"""Compare one allied and one enemy entry path from a per-frame capture.

The sprite queue stores a row and a column per 16x16 chunk; the four chunks of
a 64x64 unit sit on a 16-unit grid.  Several units share the same sprite base,
so chunks (not sprite groups) are tracked by position: each unit keeps a fixed
row and slides along its column, which makes nearest-neighbour tracking stable.

One clean unit per side is selected automatically and the two column-vs-time
curves are plotted on shared axes.  Displacement is measured from the settled
column, so both curves start at their full travel and end at 0.

Usage:
  python tools/two_paths_shape.py dump/one_run/slotrun.json [--settle 234]
"""
import argparse
import json
import os

from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))


def sg(v):
    return v - 0x10000 if v > 0x8000 else v


def chunks_per_frame(frames):
    out = []
    for f in frames:
        cs = []
        for e in f['e']:
            if e[0] != 0 or (e[1] == 0 and e[2] == 0):
                continue
            cs.append((sg(e[1]), sg(e[2]), e[3]))
        out.append(cs)
    return out


def track(chunks, settle, row, col, radius=6, back=280):
    path = [(settle, row, col)]
    for i in range(settle - 1, max(-1, settle - back), -1):
        best, bd = None, radius
        for r, c, _p in chunks[i]:
            d = abs(row - r) + abs(col - c)
            if d < bd:
                bd, best = d, (r, c)
        if best is None:
            break
        row, col = best
        path.append((i, row, col))
    path.reverse()
    return path


def entry(path):
    """Keep the horizontal ramp, stop when the row starts to wobble."""
    if len(path) < 4:
        return []
    head = sorted(q[1] for q in path[:min(10, len(path))])
    row0 = head[len(head) // 2]
    out = []
    for q in path:
        if abs(q[1] - row0) > 2:
            break
        out.append(q)
    # trim a static tail, keeping one settled frame
    while len(out) > 2 and out[-1][2] == out[-2][2]:
        out.pop()
    if out:
        out.append((out[-1][0] + 1, out[-1][1], out[-1][2]))
    return out


def pick(chunks, settle, want_ally):
    best = None
    seen = set()
    for row, col, pal in chunks[settle]:
        if (pal < 64) != want_ally:
            continue
        key = (row // 16, col // 16)
        if key in seen:
            continue
        seen.add(key)
        p = entry(track(chunks, settle, row, col))
        if len(p) < 10:
            continue
        dx = abs(p[-1][2] - p[0][2])
        if dx < 20:
            continue
        # prefer long, single-row, near-linear ramps
        score = dx + 0.1 * len(p)
        if best is None or score > best[0]:
            best = (score, p)
    return best[1] if best else None


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('src')
    ap.add_argument('--settle', type=int, default=234)
    args = ap.parse_args()
    src = os.path.join(ROOT, args.src) if not os.path.isabs(args.src) else args.src
    data = json.load(open(src, encoding='utf-8'))
    frames = data['frames']
    chunks = chunks_per_frame(frames)
    settle = min(args.settle, len(frames) - 1)

    ally = pick(chunks, settle, True)
    enemy = pick(chunks, settle, False)
    for name, p in (('ALLY', ally), ('ENEMY', enemy)):
        if not p:
            print(name, 'not found')
            continue
        print('%s row %d  frames %d..%d  col %d -> %d  (%d frames, %d units)'
              % (name, p[0][1], p[0][0], p[-1][0], p[0][2], p[-1][2],
                 p[-1][0] - p[0][0], abs(p[-1][2] - p[0][2])))
        for q in p[::max(1, len(p) // 12)]:
            print('   f%3d  col %3d' % (q[0], q[2]))

    img = Image.new('RGB', (820, 460), (14, 16, 24))
    dr = ImageDraw.Draw(img)
    for gx in range(0, 5):
        x = 70 + gx * 150
        dr.line([x, 60, x, 380], fill=(38, 40, 54))
    for gy in range(0, 5):
        y = 380 - gy * 80
        dr.line([70, y, 760, y], fill=(38, 40, 54))
    dr.line([70, 380, 760, 380], fill=(150, 150, 170))
    dr.line([70, 60, 70, 380], fill=(150, 150, 170))
    dr.text((70, 30), 'distance still to travel (queue columns, 0 = settled)')
    dr.text((700, 392), 'frames')
    for name, p, colr in (('ALLY', ally, (90, 170, 255)),
                          ('ENEMY', enemy, (120, 255, 140))):
        if not p:
            continue
        final = p[-1][2]
        t0 = p[0][0]
        span = max(1, p[-1][0] - t0)
        pts = [(70 + 690 * (q[0] - t0) / span, 380 - 3.0 * abs(q[2] - final))
               for q in p]
        pts = [(x, min(445, y)) for x, y in pts]
        if len(pts) > 1:
            dr.line(pts, fill=colr, width=3)
        for q in pts[::max(1, len(pts) // 14)]:
            dr.ellipse([q[0] - 3, q[1] - 3, q[0] + 3, q[1] + 3], fill=colr)
        label = '%s  start col %d  travel %d  %d frames' % (
            name, p[0][2], abs(p[-1][2] - p[0][2]), len(p))
        dr.text((80, 72 if name == 'ALLY' else 90), label, fill=colr)
    out = os.path.join(ROOT, 'port', 'build', 'two_paths_shape.png')
    img.save(out)
    print('->', out)


if __name__ == '__main__':
    main()
