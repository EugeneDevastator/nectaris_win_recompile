import struct, os
D = os.path.join(os.path.dirname(__file__), '..', 'dump')
terr = open(os.path.join(D, 'terrain.bin'), 'rb').read()
tm = open(os.path.join(D, 'tilemap.bin'), 'rb').read()
W, H = 15, 10
print("terrain grid (%dx%d):" % (W, H))
for y in range(H):
    print(' '.join('%2d' % (terr[y*32+x] & 0xF) for x in range(W)))
print("\ntilemap 64x32 (tile:bank), rows with content:")
for y in range(32):
    row = []
    for x in range(64):
        e = struct.unpack_from('<H', tm, (y*64+x)*2)[0]
        if e == 0x90:
            row.append('  .')
        else:
            tile = e & 0x7ff
            bank = e >> 0xb
            row.append(f'{tile:3d}')
    if any(v != '  .' for v in row):
        print(f"y={y:2d} " + ' '.join(row))
