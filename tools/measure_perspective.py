"""Generate the editable perspective/sprite configuration.

The measured entry curves already contain each slot's settled position.  This
tool writes assets/data/combat_perspective.json with one editable row per slot:

  sprite : which combat sprite variant to draw
           (0 = combat_alive.png, N = combat_vN.png if present in the unit
           folder, otherwise falls back to combat_alive.png)
  scale  : sprite size multiplier (1.0 = the original 64 px sprite on a
           256 px panel; default follows the depth row, front rows larger)
  slot   : settled panel position (informational; normally from the curves)
  speed  : global entry-speed multiplier

Edit the file by hand and relaunch the port; nothing else needs rebuilding.

Usage: python tools/measure_perspective.py
"""
import json
import os

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))


def main():
    src = os.path.join(ROOT, 'assets', 'data', 'combat_entry_curves.json')
    curves = json.load(open(src, encoding='utf-8'))
    out = {'speed': 1.1, 'left': [], 'right': []}
    for side in ('left', 'right'):
        for s in curves.get(side, []):
            u, v = s['slot']
            out[side].append({'slot': [round(u, 4), round(v, 4)],
                              'sprite': 0,
                              'scale': 1.0})
        out[side].sort(key=lambda t: (t['slot'][0], t['slot'][1]))
    dst = os.path.join(ROOT, 'assets', 'data', 'combat_perspective.json')
    json.dump(out, open(dst, 'w', encoding='utf-8'), indent=1)
    print('wrote', dst)
    for side in ('left', 'right'):
        print(side, [(r['sprite'], r['scale']) for r in out[side]])


if __name__ == '__main__':
    main()
