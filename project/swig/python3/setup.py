import os
from os.path import join as ospj
import subprocess
import sys

from distutils.core import setup, Extension

def run_strip_decode(command):
    return subprocess.check_output(command).strip().decode()

# Work around Python bug 6863: http://bugs.python.org/issue6863
# distutils malfunctions if $CXX is overridden and contains more than
# one whitespace-delimited piece. This occurs here when ./configure
# is passed the --enable-ccache flag. A non-cached build is better
# than a failing build, so drop 'ccache ' from $CXX if appropriate.
if 'CXX' in os.environ:
    pieces = os.environ['CXX'].split()
    if len(pieces) == 2 and pieces[0] == 'ccache':
        print('*** Removing "ccache" from CXX environment variable')
        os.environ['CXX'] = pieces[1]

print("sys.platform: {}".format(sys.platform))
if sys.platform not in {'win32', 'cygwin'}:
    librets_config = "../../../librets-config-inplace"

    librets_version = run_strip_decode([librets_config, "--version"])
    librets_cflags = run_strip_decode([librets_config, "--cflags"]).split()
    librets_libs = run_strip_decode([librets_config, "--libs"]).split()
    librets_basedir = run_strip_decode([librets_config, "--basedir"])
    librets_bridge = ospj(librets_basedir, "project/swig/librets_bridge.cpp")
else:
    with open("setup.ini") as f:
        lines = f.readlines()
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
                librets_bridge,
            ],
            extra_compile_args=librets_cflags,
            extra_link_args=librets_libs,
        ),
    ],
)
