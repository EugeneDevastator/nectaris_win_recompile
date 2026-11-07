import json
ev = [json.loads(l) for l in open(r'ref\session\input.jsonl', encoding='utf-8')]
print('meta:', ev[0])
print('--- all key events t<55 ---')
for e in ev:
    if e.get('ev') in ('kdown', 'kup', 'mdown', 'mup') and e.get('t', 999) < 55:
        if e['ev'] in ('mdown', 'mup'):
            print('t=%7.3f %-5s %-6s cx=%s cy=%s' % (e['t'], e['ev'], e.get('btn'), e.get('cx'), e.get('cy')))
        else:
            print('t=%7.3f %-5s vk=%3d (0x%02x)' % (e['t'], e['ev'], e.get('vk', 0), e.get('vk', 0)))
