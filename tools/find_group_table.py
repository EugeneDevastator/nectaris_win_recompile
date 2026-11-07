import struct, os
ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
b = open(os.path.join(ROOT, 'game', 'Nec.exe'), 'rb').read()
BASE_VA, BASE_OFF = 0x42a000, 0x27c00
def foff(va): return BASE_OFF + (va - BASE_VA)
def db(va, n): return list(b[foff(va):foff(va)+n])

# scan .data for runs of consecutive increasing bytes in 225..255 (group ids)
start = foff(0x42a000)
end = foff(0x496000)
data = b[start:end]
runs = []
i = 0
while i < len(data) - 8:
    if 225 <= data[i] <= 255:
        j = i + 1
        while j < len(data) and data[j] == data[j-1] + 1 and j - i < 64:
            j += 1
        if j - i >= 6:
            runs.append((0x42a000 + (i - start), list(data[i:j])))
            i = j
            continue
    i += 1
print('increasing runs >=6:', len(runs))
for va, r in runs[:20]:
    print(hex(va), r[:40])

# also scan for any 24-byte table where values are in 220..260
print('--- 24-byte candidate tables near unit data ---')
for va in range(0x42b000, 0x42b500, 1):
    vals = db(va, 24)
    if all(200 <= v <= 260 for v in vals) and len(set(vals)) > 10:
        print(hex(va), vals)
