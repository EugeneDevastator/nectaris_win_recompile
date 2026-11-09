"""Analyse a page-guard hit: map the source atlas pointer to a resource slice."""
import os, sys, json, struct, glob
sys.path.insert(0, os.path.dirname(__file__))

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
D = os.path.join(ROOT, 'dump', 'art_resources')
ATLAS = 0x484730


def main():
    d = sys.argv[1] if len(sys.argv) > 1 else os.path.join(ROOT, 'dump', 'watchdog_boot')
    logs = glob.glob(os.path.join(d, '*.log'))
    for lg in logs:
        print('==', lg)
        for line in open(lg, encoding='utf-8'):
            if line.startswith('WRITE'):
                print(line.strip())
                try:
                    regs = json.loads(line.split('WRITE', 1)[1])
                except Exception:
                    continue
                ecx = regs.get('ecx', 0)
                off = ecx - ATLAS
                print('  atlas offset of ecx: %#x' % off if 0 <= off < 0x20000 else '  ecx not in atlas')
                eip = regs.get('eip', 0)
                print('  eip %#x  slot %s' % (eip, regs.get('slot')))
    # list hits
    for f in sorted(glob.glob(os.path.join(d, 'hit_*_stack.bin'))):
        print('--', os.path.basename(f))
        data = open(f, 'rb').read()
        for i in range(0, len(data), 4):
            v = struct.unpack_from('<I', data, i)[0]
            if 0x400000 <= v < 0x496000:
                print('   +%03x %08x' % (i, v))


if __name__ == '__main__':
    main()
