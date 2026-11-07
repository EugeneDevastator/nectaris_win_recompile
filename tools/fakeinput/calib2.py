from PIL import Image

def score(path, ox, oy, label):
    img = Image.open(path).convert('RGB')
    px = img.load()
    x0, x1 = 130, 520
    bands = [(366, 386), (414, 434)]
    total = black = 0
    per = []
    for (y0, y1) in bands:
        w = 0; b = 0; t = 0
        for y in range(oy+y0, oy+y1):
            for x in range(ox+x0, ox+x1):
                r, g, bl = px[x, y]
                t += 1
                if r < 24 and g < 24 and bl < 24: b += 1
                if r > 170 and g > 170 and bl > 170: w += 1
        per.append((w, b, t))
        total += t; black += b
    print(f"{label}: black_frac={black/total:.3f} rows={per}")

score(r'C:\OpenCode-Drive\SharedF\nectariswin\shot_t2.png', 3, 48, 'menu  ')
score(r'C:\OpenCode-Drive\SharedF\nectariswin\ctl_menu.png', 0, 0, 'mech  ')
score(r'C:\OpenCode-Drive\SharedF\nectariswin\shot_t1.png', 3, 48, 'logo  ')
score(r'C:\OpenCode-Drive\SharedF\nectariswin\shot1.png', 3, 48, 'title ')

