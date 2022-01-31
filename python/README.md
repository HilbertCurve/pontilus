# Building Pontilus for python
The following is a tutorial on installing this python library.

## Windows
TODO

## Linux
NOTICE: this requires cpython<br>

Change directory to python directory:<br>
`$ cd (installation dir)/pontilus/python`<br>
Build and link project & python bindings file (pontilus_engine.cpp):<br>
`$ python3 setup.py build`<br>
Install shared library:<br>
`$ sudo python3 setup.py install`<br>
To play around with it, change directory out of the python dir and enjoy:<br>
`$ cd ..`<br>
`$ python3`<br>
`Python 3.10 [GCC 9.3.0] on linux`<br>
`Type "help", "copyright", "credits" or "license" for more information.`<br>
`>>> import pontilus`<br>
`>>> # enjoy`
