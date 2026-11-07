import struct
b = open(r'game\Nec.exe','rb').read()
base = 0x27c00 - 0x42a000
def d(va, n):
    o = base + va
    return list(struct.unpack('%db' % n, b[o:o+n]))
dx = d(0x42a238, 6)
dy = d(0x42a228, 12)
print('dx        =', dx)
print('dy even   =', dy[:6])
print('dy odd    =', dy[6:])
