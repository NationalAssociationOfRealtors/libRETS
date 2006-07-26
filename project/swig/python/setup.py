import distutils
from distutils.core import setup, Extension
import commands
import re

librets_config = "../../../librets-config-inplace"

librets_version = commands.getoutput(librets_config + " --version").rstrip()
librets_cflags = commands.getoutput(librets_config + " --cflags").split()
librets_libs = commands.getoutput(librets_config + " --libs").split()

setup(name = "librets: A RETS client library",
  version = librets_version,
  py_modules=['librets'],
  ext_modules = [Extension("_librets", ["librets_wrap.cpp"],
    extra_compile_args = librets_cflags,
    extra_link_args = librets_libs
    )
    ])

