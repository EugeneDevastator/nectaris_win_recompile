import re
b=open(r'game\Nec.exe','rb').read()
off,size=0x27c00,0x56200
chunk=b[off:off+size]
print('=== ASCII strings in .data ===')
for m in re.finditer(rb'[\x20-\x7e]{4,}', chunk):
    print(f"{off+m.start():06x}  {m.group().decode('latin1')}")
print('\n=== cp932 strings in .data (excluding pure ascii) ===')
pat=re.compile(rb'(?:[\x20-\x7e]|[\x81-\x9f\xe0-\xfc][\x40-\xfc]){3,}')
seen=set()
for m in pat.finditer(chunk):
    raw=m.group()
    if all(0x20<=c<=0x7e for c in raw):
        continue
    try:
        s=raw.decode('cp932')
    except Exception:
        continue
    if '\ufffd' in s: continue
    if len(s)<3: continue
    key=m.start()
    if key in seen: continue
    seen.add(key)
    print(f"{off+m.start():06x}  {s}")
