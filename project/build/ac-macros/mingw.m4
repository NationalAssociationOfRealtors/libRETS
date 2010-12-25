dnl
dnl Configure MinGW builds on Windows
dnl
AC_DEFUN([MY_TEST_MINGW], [
  AC_ARG_WITH(platform,
              AC_HELP_STRING([--with-platform=PLATFORM],
                             [Set platform (x86 or x64 - primarily for MinGW on Windows)]),
              my_platform="$withval",
	      my_platform="")

  echo \*\*\*\*\*\*\* my_platform: $my_platform
  if test "$my_platform" != "x86"; then

    if test "$my_platform" != "x64"; then
    	my_platform=""
    fi
  fi

  if test "X$my_platform" != "X"; then
      ARCH="-$my_platform"
  fi


  if test "X$MSYSTEM" != "X"; then
      LDFLAGS="$LDFLAGS -static-libgcc -Wl,-Bstatic"
  fi
])
