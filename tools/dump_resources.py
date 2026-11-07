import struct, os
ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
b = open(os.path.join(ROOT, 'game', 'Nec.exe'), 'rb').read()
BASE_VA, BASE_OFF = 0x42a000, 0x27c00
def foff(va): return BASE_OFF + (va - BASE_VA)
def u8(va): return b[foff(va)]
def u32(va): return struct.unpack_from('<I', b, foff(va))[0]

print('resource table (id, type, p1, p2, p3, data_va):')
for rid in range(0, 64):
    # 24-bit offset from bytes at 0x4324d1 + rid*4
    o = u8(0x4324d1 + rid*4) | (u8(0x4324d2 + rid*4) << 8) | (u8(0x4324d3 + rid*4) << 16)
    data_va = 0x444348 + o
    t = b[foff(0x42bb88 + rid*4)]
    p1 = b[foff(0x42bb89 + rid*4)]
    p2 = b[foff(0x42bb8a + rid*4)]
    p3 = b[foff(0x42bb8b + rid*4)]
    if o or t:
        print('id %2d type %d p1 %3d p2 %3d p3 %3d off %#08x va %#x' % (
            rid, t, p1, p2, p3, o, data_va))
