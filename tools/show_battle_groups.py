import json, os
ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
g = json.load(open(os.path.join(ROOT, 'dump', 'sprite_groups.json')))
for gid in sorted(g, key=int):
    gid = int(gid)
    if 226 <= gid <= 255:
        print('gid', gid, 'count', len(g[str(gid)]))
        for p in g[str(gid)]:
            print('   dx=%3d dy=%3d spr=%3d fl=%3d' % tuple(p))
