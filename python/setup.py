# NOTE: it's python3 pontilus.py build, then python3 pontilus.py install

from setuptools.command.build_ext import build_ext
from setuptools import Extension, setup
from os import walk

class custom_build_ext(build_ext):
    def build_extensions(self):
        # Override the compiler executables. Importantly, this
        # removes the "default" compiler flags that would
        # otherwise get passed on to to the compiler, i.e.,
        # distutils.sysconfig.get_var("CFLAGS").
        self.compiler.set_executable("compiler_so", "g++")
        self.compiler.set_executable("compiler_cxx", "g++")
        self.compiler.set_executable("linker_so", "g++")
        build_ext.build_extensions(self)

files = []
for (dirpath, dirnames, filenames) in walk("../pontilus"):
    files.extend([dirpath + "/" + name for name in filenames if name.endswith(".cpp")])

files.append("./pontilus_engine.cpp")

setup(
    name="pontilus",
    ext_modules=[
        Extension(
            "pontilus", 
            sources=files,
            extra_compile_args="-DGL_GLEXT_PROTOTYPES -I../pontilus -g -fPIC".split(" "),
            extra_link_args="-shared -lGL -lopenal -lpython3.11 -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl".split(" ")
        )
    ],
    zip_safe=False,
    cmdclass={"build_ext": custom_build_ext}
)
