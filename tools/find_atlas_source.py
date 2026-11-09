"""Find the 16x16 4bpp source tiles of generated combat sprites inside the atlas."""
import os, sys, struct
import numpy as np

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
D = os.path.join(ROOT, 'dump', 'block_changes')
STATE = sys.argv[1] if len(sys.argv) > 1 else '04_be145e1a'
blk = open(os.path.join(D, STATE, 'block.bin'), 'rb').read()
atlas = open(os.path.join(D, STATE, 'atlas.bin'), 'rb').read()
ATLAS_VA = 0x484730


def pack16(tile, hi_first=True):
    out = bytearray()
    for y in range(16):
        for x in range(0, 16, 2):
            a = int(tile[y, x]) & 0xF
            b = int(tile[y, x + 1]) & 0xF
            out.append(((a << 4) | b) if hi_first else ((b << 4) | a))
    return bytes(out)


hits = []
for k in range(32):
    arr = np.frombuffer(blk[k * 1024:(k + 1) * 1024], dtype=np.uint8).reshape(32, 32)
    # spritebank is stored bottom-up and 2x2 expanded; try downsampled tiles
    for flip in (False, True):
        a = arr[::-1] if flip else arr
        for oy in (0, 1):
            for ox in (0, 1):
                tile = a[oy::2, ox::2][:16, :16]
                for hi in (True, False):
                    pat = pack16(tile, hi)
                    p = atlas.find(pat)
                    if p >= 0:
                        # try to find all occurrences
                        locs = []
                        q = 0
                        while True:
                            q = atlas.find(pat, q)
                            if q < 0:
                                break
                            locs.append(q)
                            q += 1
                        hits.append((k, flip, oy, ox, hi, p, locs[:4]))

print('state', STATE, 'hits', len(hits))
for h in hits[:40]:
    k, flip, oy, ox, hi, p, locs = h
    print('slot %d flip%s oy%d ox%d hi%s atlas_off %#x (p3 %.2f) locs %s' %
          (128 + k, flip, oy, ox, hi, p, p / 0x200, [hex(x) for x in locs]))
