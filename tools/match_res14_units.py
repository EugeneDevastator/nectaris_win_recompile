import numpy as np, json

sb = open('dump/spritebank_m6.bin', 'rb').read()
r14 = open('dump/art_resources/res_14_sprites.bin', 'rb').read()


def m(raw, t):
    return (np.frombuffer(raw[t * 1024:(t + 1) * 1024], dtype=np.uint8).reshape(32, 32) != 0)[::-1]


def iou(a, b):
    u = np.logical_or(a, b).sum()
    return (a & b).sum() / u if u else 0


maps = {t: m(sb, 64 + t) for t in range(23)}
for g in range(6):
    votes = {}
    per = []
    for r in range(4):
        a = m(r14, 4 * g + r)
        best = max(((iou(a, mp), t) for t, mp in maps.items()))
        per.append((best[1], round(best[0], 2)))
        votes[best[1]] = votes.get(best[1], 0) + best[0]
    top = sorted(votes.items(), key=lambda kv: -kv[1])[:3]
    print('res14 group', g, 'per-rotation', per, 'votes', [(t, round(s, 2)) for t, s in top])
