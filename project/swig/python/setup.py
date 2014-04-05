# 'commands' is deprecated as of Python 2.6, but it still *works*.
# I'd be inclined to use the 'subprocess' module instead, but it was
# added in 2.4. I seriously doubt that anyone is using current
# librets with Python 2.3 or older, but we don't gain anything by
# dropping support for those versions as long as they're still
# supported by SWIG.
import commands
from os.path import join as ospj
import sys

from distutils.core import setup, Extension

print "sys.platform: " + sys.platform
if sys.platform not in ('win32', 'cygwin'):
    librets_config = "../../../librets-config-inplace"

    librets_version = commands.getoutput(librets_config + " --version").rstrip()
    librets_cflags = commands.getoutput(librets_config + " --cflags").split()
    librets_libs = commands.getoutput(librets_config + " --libs").split()
    librets_basedir = commands.getoutput(librets_config + " --basedir")
    librets_bridge = ospj(librets_basedir, "project/swig/librets_bridge.cpp")
else:
    f = open("setup.ini")
    lines = f.readlines()
    f.close()
    librets_version = lines[0].rstrip()
    librets_libs = lines[1].rstrip().split()
    librets_cflags = lines[2].rstrip().split()
    librets_bridge = lines[3].rstrip()

setup(
    name="librets-python",
    version=librets_version,
    py_modules=['librets'],
    ext_modules=[
        Extension(
            "_librets",
            [
                "librets_wrap.cpp",
                librets_bridge
            ],
            extra_compile_args=librets_cflags,
            extra_link_args=librets_libs,
        ),
    ],
)
