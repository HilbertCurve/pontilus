import os

if os.name == "posix":
    os.chdir("../")

import pontilus

pontilus.init()

def bar():
    

foo = pontilus.Scene(bar)

pontilus.clean()