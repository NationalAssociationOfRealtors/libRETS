from os.path import join as ospj
import subprocess
import sys

from distutils.core import setup, Extension

def run_strip_decode(command):
    return subprocess.check_output(command).strip().decode()

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
