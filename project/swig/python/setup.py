import distutils
from distutils.core import setup, Extension
import commands
import re
import sys

if (sys.platform != 'win32'):
    librets_config = "../../../librets-config-inplace"

    librets_version = commands.getoutput(librets_config + " --version").rstrip()
    librets_cflags = commands.getoutput(librets_config + " --cflags").split()
    librets_libs = commands.getoutput(librets_config + " --libs").split()
else:
    lines = file("setup.ini").readlines()
    librets_version = lines[0].rstrip()
    librets_libs = lines[1].rstrip().split()
    librets_cflags = lines[2].rstrip().split()

setup(name = "librets-python",
  version = librets_version,
  py_modules=['librets'],
  ext_modules = [Extension("_librets", ["librets_wrap.cpp"],
    extra_compile_args = librets_cflags,
    extra_link_args = librets_libs
    )
    ])

 
