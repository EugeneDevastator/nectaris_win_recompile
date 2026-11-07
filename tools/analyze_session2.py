import json
ev = [json.loads(l) for l in open(r'ref\session\input.jsonl', encoding='utf-8')]
print('--- input events t=35..75 ---')
for e in ev:
    if e.get('ev') in ('mdown', 'mup', 'kdown', 'kup') and 35 <= e.get('t', -1) <= 75:
        if e['ev'] in ('mdown', 'mup'):
            print('t=%7.3f %-5s %-6s cx=%s cy=%s' % (e['t'], e['ev'], e.get('btn'), e.get('cx'), e.get('cy')))
        else:
            print('t=%7.3f %-5s vk=%d (0x%02x)' % (e['t'], e['ev'], e.get('vk', 0), e.get('vk', 0)))
print('--- shot idx -> t around battle ---')
by_idx = {}
for e in ev:
    if e.get('ev') == 'shot':
        by_idx[e['idx']] = e['t']
for i in range(195, 240):
    if i in by_idx:
        print(i, by_idx[i])
