p = r'port/src/main.cpp'
lines = open(p, encoding='utf-8', errors='replace').read().splitlines()
for i, l in enumerate(lines):
    if 'InitWindow(mapW' in l:
        lines[i] = '    InitWindow(mapW + panelW, std::max(mapH, 720), "Nectaris - portable");'
open(p, 'w', encoding='utf-8').write('\n'.join(lines) + '\n')
print('fixed title')
