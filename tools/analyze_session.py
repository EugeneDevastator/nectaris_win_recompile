import json
ev = [json.loads(l) for l in open(r'ref\session\input.jsonl', encoding='utf-8')]
keys = [e for e in ev if e.get('ev') in ('mdown', 'mup', 'kdown', 'kup')]
print('total events', len(ev), 'input events', len(keys))
for e in keys[-110:]:
    if e['ev'] in ('mdown', 'mup'):
        print('t=%7.3f %-5s %-6s cx=%s cy=%s fg=%s' % (
            e['t'], e['ev'], e.get('btn'), e.get('cx'), e.get('cy'), e.get('fg')))
    else:
        print('t=%7.3f %-5s vk=%d (0x%02x) fg=%s' % (
            e['t'], e['ev'], e.get('vk', 0), e.get('vk', 0), e.get('fg')))
print('--- shot events near battle ---')
for e in ev:
    if e.get('ev') == 'shot' and e['t'] > 80:
        print(e)
