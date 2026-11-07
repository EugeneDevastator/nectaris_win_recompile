import struct, sys, re, os

path = r"game\Nec.exe"
b = open(path,'rb').read()

sections = [
    ('.text', 0x00000400, 0x27400),
    ('.rdata',0x00027800, 0x400),
    ('.data', 0x00027c00, 0x56200),
    ('.idata',0x0007de00, 0xe00),
    ('.rsrc', 0x0007ec00, 0xba800),
    ('.reloc',0x00139400, 0x7000),
]

def try_str(bs):
    out = []
    for enc in ('cp932','latin1'):
        try:
            s = bs.decode(enc)
        except Exception:
            continue
        printable = sum(1 for c in s if c.isprintable() or c in '\r\n\t')
        if printable == len(s):
            out.append((enc,s))
    return out

for name, off, size in sections:
    chunk = b[off:off+size]
    print(f"\n===== {name} file 0x{off:x} size 0x{size:x} =====")
    # split on non-printable ascii runs but keep high-bit (sjis) chars
    runs = re.finditer(rb'[\x20-\x7e\x81-\x9f\xe0-\xfc]{4,}', chunk)
    count = 0
    for m in runs:
        raw = m.group()
        # require at least 4 ascii-ish
        if sum(1 for c in raw if 0x20 <= c <= 0x7e) < 3:
            continue
        cand = try_str(raw)
        if cand:
            enc, s = cand[0]
            print(f"  {off+m.start():06x} [{enc}] {s}")
            count += 1
            if count > 400:
                print("  ...")
                break
