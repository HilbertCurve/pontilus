import os

if os.name == "posix":
    os.chdir("../")

import pontilus

pontilus.init()

def bar():
    print("e")

foo = pontilus.Scene(bar)

pontilus.clean()