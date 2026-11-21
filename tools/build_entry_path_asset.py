"""Build the entry-path asset used by the combat renderer.

Reads dump/entry_paths.json (extracted from the live sprite queue) and writes
assets/data/combat_entry_paths.json.  Paths are keyed by unit type (0..23) when
a capture exists for that type, with per-class fallbacks.  Offsets are original
queue units: the blitter scales them by 2 and the battle panel is 256 units
wide, so the port maps them with `offset_px = units * panel_size / 128`.
"""
import json
import os
import re

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
SRC = os.path.join(ROOT, 'dump', 'entry_paths.json')
DST = os.path.join(ROOT, 'assets', 'data', 'combat_entry_paths.json')

# capture dir -> (left/attacker type, right/defender type); matrix captures
# encode type/terrain in the directory name and are parsed below.
CAPTURE_TYPES = {
    'battle_sprites_mem': (4, 6),
    'battle_sprites_mem_air': (2, 6),
    'battle_sprites_mem_vsair2': (9, 1),
    'battle_sprites_mem_t18': (18, 6),
    'battle_sprites_mem_t20': (20, 6),
    'battle_sprites_mem_t15': (15, 1),
    'battle_sprites_mem_t3': (3, 6),
    'battle_sprites_mem_t19': (19, 6),
    'battle_sprites_mem_t5': (5, 6),
}
MATRIX_RE = re.compile(r'^a(\d+)_d(\d+)_t(\d+)$')
EFFECT_PALS = {5, 69, 65}


def clean(path):
    for t, dx, dy in path:
        if abs(dx) > 120 or abs(dy) > 60:
            return False
    for a, b in zip(path, path[1:]):
        if abs(b[1] - a[1]) > 60 or abs(b[2] - a[2]) > 40:
            return False
    return True


def p90(v):
    if not v:
        return 0.0
    s = sorted(v)
    return round(s[min(len(s) - 1, int(0.9 * (len(s) - 1)))], 1)


def main():
    data = json.load(open(SRC, encoding='utf-8'))
    per_type = {}
    class_acc = {}
    per_terrain = {}
    for cap, entries in data.items():
        m = MATRIX_RE.match(cap)
        if m:
            atk_t, def_t, terr = int(m.group(1)), int(m.group(2)), int(m.group(3))
            types = (atk_t, def_t)
        else:
            types = CAPTURE_TYPES.get(cap)
            terr = 0
        if not types:
            continue
        for e in entries:
            if e['pal'] in EFFECT_PALS or not clean(e['path']):
                continue
            side = 0 if e['pal'] < 64 else 1       # 0 = left/blue, 1 = right/green
            utype = types[side]
            start = e['path'][0]
            mid = e['path'][min(len(e['path']) - 1, len(e['path']) // 2)]
            a = per_type.setdefault(utype, {'sx': [], 'sy': [], 'mx': [], 'my': []})
            a['sx'].append(abs(start[1]))
            a['sy'].append(abs(start[2]))
            a['mx'].append(abs(mid[1]))
            a['my'].append(abs(mid[2]))
            if terr:
                k = (utype, terr)
                a = per_terrain.setdefault(k, {'sx': [], 'sy': [], 'mx': [], 'my': []})
                a['sx'].append(abs(start[1]))
                a['sy'].append(abs(start[2]))
                a['mx'].append(abs(mid[1]))
                a['my'].append(abs(mid[2]))
            # class fallback buckets
            cls = 'air' if utype in (1, 2, 3, 22) else ('bike' if utype == 20 else 'ground')
            b = class_acc.setdefault(cls, {'sx': [], 'sy': [], 'mx': [], 'my': []})
            b['sx'].append(abs(start[1]))
            b['sy'].append(abs(start[2]))
            b['mx'].append(abs(mid[1]))
            b['my'].append(abs(mid[2]))

    types_out = {str(t): {'start': [p90(a['sx']), p90(a['sy'])],
                          'mid': [p90(a['mx']), p90(a['my'])],
                          'end': [0, 0]}
                 for t, a in per_type.items()}
    terrain_out = {'%d_%d' % (t, terr): {'start': [p90(a['sx']), p90(a['sy'])],
                                         'mid': [p90(a['mx']), p90(a['my'])],
                                         'end': [0, 0]}
                   for (t, terr), a in per_terrain.items()}
    classes_out = {cls: {'start': [p90(a['sx']), p90(a['sy'])],
                         'mid': [p90(a['mx']), p90(a['my'])],
                         'end': [0, 0]}
                   for cls, a in class_acc.items()}
    for cls in ('ground', 'air', 'bike'):
        classes_out.setdefault(cls, {'start': [35.0, 8.0], 'mid': [18.0, 6.0], 'end': [0, 0]})

    out = {
        'note': 'Entry paths decoded from the live 0x4836f0 sprite queue. '
                'Offsets are original queue units (blitter scales by 2; panel '
                'is 256 units); x is mirrored for the right-side army.',
        'unit_scale': 128,
        'types': types_out,
        'terrain_types': terrain_out,
        'classes': classes_out,
    }
    with open(DST, 'w', encoding='utf-8') as f:
        json.dump(out, f, indent=1)
    print(json.dumps(out, indent=1))


if __name__ == '__main__':
    main()
