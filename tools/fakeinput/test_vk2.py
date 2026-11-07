import os, time, sys
sys.path.insert(0, os.path.dirname(__file__))
from control import Driver

game = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..', 'game'))
d = Driver(game)
VK_SPACE = 0x20
try:
    print("pid", d.proc.pid)
    d.fake_activate()
    time.sleep(32)
    seen = d.vkseen()
    print("VKs polled after 32s:", seen)
    d.snap(r"C:\OpenCode-Drive\SharedF\nectariswin\vk32_before.png")
    # try GetKeyState path (fake key state) for Space
    for k in range(6):
        d.key(VK_SPACE, True)
        d.wake(); time.sleep(0.5)
        d.key(VK_SPACE, False)
        d.wake(); time.sleep(0.8)
    d.snap(r"C:\OpenCode-Drive\SharedF\nectariswin\vk32_after_hold.png")
    print("VKs polled after hold:", d.vkseen())
    # try posted key path
    for k in range(6):
        d.tap(VK_SPACE)
        time.sleep(0.8)
    d.snap(r"C:\OpenCode-Drive\SharedF\nectariswin\vk32_after_post.png")
finally:
    d.close()
