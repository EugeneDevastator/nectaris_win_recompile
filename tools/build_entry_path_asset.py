"""Build the entry-path asset used by the combat renderer.

Reads dump/entry_paths.json (extracted from the live sprite queue) and writes
assets/data/combat_entry_paths.json with a normalised control path per unit
class.  Offsets are in original queue units: the blitter scales them by 2 and
the battle panel is 256 units wide, so the port maps them with
`offset_px = queue_units * (panel_size / 128)`.
"""
import json
import os

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
SRC = os.path.join(ROOT, 'dump', 'entry_paths.json')
DST = os.path.join(ROOT, 'assets', 'data', 'combat_entry_paths.json')

# capture dir -> which palette groups are which class
CLASSES = {
    'battle_sprites_mem': {'left': 'ground', 'right': 'ground'},
    'battle_sprites_mem_air': {'left': 'air', 'right': 'ground'},
    'battle_sprites_mem_vsair2': {'left': 'ground', 'right': 'air'},
}
EFFECT_PALS = {5, 69, 65}


def clean(path):
    """Reject jumping/noisy paths (palette copies, effects)."""
    for t, dx, dy in path:
        if abs(dx) > 120 or abs(dy) > 60:
            return False
    for a, b in zip(path, path[1:]):
        if abs(b[1] - a[1]) > 60 or abs(b[2] - a[2]) > 40:
            return False
    return True


def main():
    data = json.load(open(SRC, encoding='utf-8'))
    acc = {}
    for cap, entries in data.items():
        sides = CLASSES.get(cap)
        if not sides:
            continue
        for e in entries:
            if e['pal'] in EFFECT_PALS or not clean(e['path']):
                continue
            # left groups have lower sprite bases than right groups in these captures
            side = 'left' if e['sprite'] < 100 else 'right'
            cls = sides[side]
            start = e['path'][0]
            mid_i = min(len(e['path']) - 1, len(e['path']) // 2)
            mid = e['path'][mid_i]
            a = acc.setdefault(cls, {'sx': [], 'sy': [], 'mx': [], 'my': []})
            a['sx'].append(abs(start[1]))
            a['sy'].append(abs(start[2]))
            a['mx'].append(abs(mid[1]))
            a['my'].append(abs(mid[2]))

    def avg(v):
        return round(sum(v) / len(v), 1) if v else 0.0

    def p90(v):
        if not v:
            return 0.0
        s = sorted(v)
        return round(s[min(len(s) - 1, int(0.9 * (len(s) - 1)))], 1)

    classes = {}
    for cls, a in acc.items():
        classes[cls] = {
            'start': [p90(a['sx']), p90(a['sy'])],
            'mid': [p90(a['mx']), p90(a['my'])],
            'end': [0, 0],
        }
    out = {
        'note': 'Entry paths decoded from the live 0x4836f0 sprite queue. '
                'Offsets are original queue units (blitter scales by 2; panel '
                'is 256 units); x is mirrored for the right-side army.',
        'unit_scale': 128,
        'classes': classes,
    }
    with open(DST, 'w', encoding='utf-8') as f:
        json.dump(out, f, indent=1)
    print(json.dumps(out, indent=1))


if __name__ == '__main__':
    main()
