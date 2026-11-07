import os, time, sys
sys.path.insert(0, os.path.dirname(__file__))
from control import Driver, menu_metrics

game = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..', 'game'))
d = Driver(game)
VK_RETURN = 0x0D
try:
    print("pid", d.proc.pid)
    for k in range(5):
        d.key(VK_RETURN, True)
        d.wake()
        time.sleep(0.35)
        d.key(VK_RETURN, False)
        d.wake()
        time.sleep(1.2)
        img = d.snap(rf"C:\OpenCode-Drive\SharedF\nectariswin\key_{k}.png")
        bf, w = menu_metrics(img)
        patched, keycalls, getcalls, setcalls, lastvk, alive = d.diag()
        print(f"press {k}: black={bf:.2f} white={w} alive={alive:#x} patched={patched} keycalls={keycalls} getcalls={getcalls} setcalls={setcalls} lastvk={lastvk}")
finally:
    d.close()
