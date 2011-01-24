dnl
dnl Configure MinGW builds on Windows
dnl
AC_DEFUN([MY_TEST_MINGW], [
  AC_ARG_WITH(platform,
              AC_HELP_STRING([--with-platform=PLATFORM],
                             [Set platform (x86 or x64 - primarily for MinGW on Windows)]),
              my_platform="$withval",
	      my_platform="")

  if test "$my_platform" != "x86"; then

    if test "$my_platform" != "x64"; then
    	my_platform=""
    fi
  fi

  if test "X$my_platform" != "X"; then
      ARCH="-$my_platform"
      CFLAGS=`echo ${CFLAGS} | sed -e 's/-fPIC//'`
      LIBRETS_CFLAGS=`echo ${LIBRETS_CFLAGS} | sed -e 's/-fPIC//'`
      LDFLAGS="${LDFLAGS} -static-libgcc -static-libstdc++"
  fi


  if test "X$MSYSTEM" != "X"; then
      LDFLAGS="$LDFLAGS -Wl,-Bstatic"
  fi
])
