from PIL import Image
img = Image.open(r'ref\battle\step_attack.png').convert('RGB')
w, h = img.size
px = img.load()
pts = []
for y in range(h):
    for x in range(w):
        r, g, b = px[x, y]
        if b > 170 and g > 110 and r < 130 and b > r + 60:
            pts.append((x, y))
print('points', len(pts))
# simple grid clustering
pts.sort()
clusters = []
for p in pts:
    placed = False
    for c in clusters:
        if abs(p[0]-c[0]) < 22 and abs(p[1]-c[1]) < 22:
            c[0] = (c[0]*c[2]+p[0])/(c[2]+1)
            c[1] = (c[1]*c[2]+p[1])/(c[2]+1)
            c[2] += 1
            placed = True
            break
    if not placed:
        clusters.append([p[0], p[1], 1])
clusters = [c for c in clusters if c[2] > 25]
clusters.sort(key=lambda c: -c[2])
for c in clusters[:20]:
    print(f'center=({c[0]:.0f},{c[1]:.0f}) size={c[2]} client=({c[0]-3:.0f},{c[1]-26:.0f})')
