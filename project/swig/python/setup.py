import distutils
from distutils.core import setup, Extension
import commands
import re
import sys

print "sys.platform: " + sys.platform
if (sys.platform != 'win32' and sys.platform != 'cygwin'):
    librets_config = "../../../librets-config-inplace"

    librets_version = commands.getoutput(librets_config + " --version").rstrip()
    librets_cflags = commands.getoutput(librets_config + " --cflags").split()
    librets_libs = commands.getoutput(librets_config + " --libs").split()
    librets_bridge = commands.getoutput(librets_config + " --basedir") +  "/project/swig/librets_bridge.cpp"
else:
    with open("setup.ini") as f:
        lines = f.readlines()
    librets_version = lines[0].rstrip()
    librets_libs = lines[1].rstrip().split()
    librets_cflags = lines[2].rstrip().split()
    librets_bridge = lines[3].rstrip()

setup(name = "librets-python",
  version = librets_version,
  py_modules=['librets'],
  ext_modules = [Extension("_librets", ["librets_wrap.cpp" , librets_bridge],
    extra_compile_args = librets_cflags,
    extra_link_args = librets_libs
    )
    ])

 
