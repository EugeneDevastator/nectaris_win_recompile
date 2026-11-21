"""Draw the captured unit paths as a diagram (validation view).

Reads a capture_one_run JSON, maps queue coordinates into the battle-panel
layout (left palette < 64 at x 64..320, right palette >= 64 at x 320..576,
y 96..352) and draws each unit's path with a start marker and end ring.
"""
import json
import os
import sys

from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))


def groups(entries):
    by = {}
    for bank, x, y, pal, sprite in entries:
        if bank != 0 or (x == 0 and y == 0):
            continue
        k = (pal, sprite & ~3)
        g = by.setdefault(k, {'x': [], 'y': []})
        g['x'].append(x)   # field1 -> panel Y
        g['y'].append(y)   # field0 -> panel X
    return {k: (min(g['y']), min(g['x'])) for k, g in by.items() if len(g['x']) >= 4}


def main():
    src = sys.argv[1] if len(sys.argv) > 1 else os.path.join(ROOT, 'dump', 'one_run', 'plane_frames.json')
    out = sys.argv[2] if len(sys.argv) > 2 else os.path.join(ROOT, 'port', 'build', 'path_diagram.png')
    data = json.load(open(src, encoding='utf-8'))
    frames = data['frames']

    img = Image.new('RGB', (640, 480), (10, 12, 20))
    dr = ImageDraw.Draw(img)
    dr.rectangle([64, 96, 320, 352], outline=(70, 70, 90))
    dr.rectangle([320, 96, 576, 352], outline=(70, 70, 90))
    dr.text((70, 82), 'PLAYER panel (queue x/y)', fill=(140, 140, 160))
    dr.text((326, 82), 'ENEMY panel', fill=(140, 140, 160))

    series = {}
    for fi, f in enumerate(frames):
        for k, (x, y) in groups(f['e']).items():
            series.setdefault(k, []).append((fi, x, y))

    colors = [(80, 190, 255), (120, 255, 160), (255, 200, 80), (255, 110, 110),
              (200, 140, 255), (90, 230, 230), (255, 150, 60), (170, 255, 90)]
    n = 0
    for k, pts in sorted(series.items()):
        if len(pts) < 10:
            continue
        # entry only: stop at the settled position
        end = len(pts) - 1
        for j in range(len(pts) - 1, 6, -1):
            if all(pts[j - m][1:] == pts[j][1:] for m in range(1, 6)):
                end = j
            else:
                break
        entry = pts[:end + 1]
        if len(entry) < 8:
            continue
        col = colors[n % len(colors)]
        n += 1
        base_x = 64 if k[0] < 64 else 320
        line = [(base_x + p[1], 96 + p[2]) for p in entry]
        if len(line) > 1:
            dr.line(line, fill=col, width=2)
        dr.rectangle([line[0][0] - 3, line[0][1] - 3, line[0][0] + 3, line[0][1] + 3], outline=col)
        dr.ellipse([line[-1][0] - 4, line[-1][1] - 4, line[-1][0] + 4, line[-1][1] + 4], outline=col, width=2)
        dr.text((line[-1][0] + 6, line[-1][1] - 5), '%d/%d' % (k[0], k[1]), fill=col)
    img.save(out)
    print('tracks', n, '->', out)


if __name__ == '__main__':
    main()
