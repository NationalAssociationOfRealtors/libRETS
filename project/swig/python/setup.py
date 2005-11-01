import distutils
from distutils.core import setup, Extension
import commands

librets_config = "../../../librets-config-inplace"

librets_cflags = commands.getoutput(librets_config + " --cflags").split()
librets_libs = commands.getoutput(librets_config + " --libs").split()

setup(name = "librets: A RETS client library",
  version = "1.0",
  ext_modules = [Extension("_librets", ["librets_wrap.cpp"],
    extra_compile_args = librets_cflags,
    extra_link_args = librets_libs
    )
    ])
