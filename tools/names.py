import os, sys
print(bytes.fromhex('83478366834283628367').decode('cp932'))
print(bytes.fromhex('8386836A83628367947A9275').decode('cp932'))
root = r'game\MAPDATA'
for d in os.listdir(root):
    print(repr(d), len(d))
