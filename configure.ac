dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.52)
AC_INIT
AC_CONFIG_SRCDIR(project/librets/include/librets.h)
AC_CONFIG_AUX_DIR(project/build)
AC_CONFIG_HEADERS([project/librets/src/config.h])

dnl Checks for programs
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_CHECK_PROG(my_have_perl, perl, yes, no)
if test x"$my_have_perl" != x"yes"; then
    AC_MSG_ERROR([perl is required to build librets])
fi

VERSION=`perl -ane 'print $1 if /VERSION\s+\=\s+(\S+)/' ${ac_top_builddir}project/build/version.mk`
AC_MSG_CHECKING([librets version])
AC_MSG_RESULT($VERSION)
AC_SUBST(VERSION)

AC_CHECK_PROG(my_have_doxygen, doxygen, yes, no)
if test x"$my_have_doxygen" = "xyes"; then
  HAVE_DOXGYEN=1
else
  HAVE_DOXYGEN=0
fi
AC_SUBST(HAVE_DOXYGEN)

MY_PROG_CCACHE
AC_CACHE_SAVE

MY_TEST_DEPENDS
MY_TEST_CURL
MY_TEST_EXPAT

MY_TEST_BOOST
if test "$HAVE_BOOST" != "1"; then
  AC_MSG_ERROR([Boost headers are required to build librets])
fi
if test -z "$BOOST_FILESYSTEM"; then
  AC_MSG_ERROR("Boost filesystem static library is required to build librets")
fi

MY_TEST_ANTLR

MY_TEST_CPPUNIT

MY_CHECK_EXAMPLES

LIBRETS_CFLAGS="$BOOST_CFLAGS"
LIBRETS_LDFLAGS="$BOOST_LIBS $BOOST_FILESYSTEM $CURL_LIBS $EXPAT_LIBS $ANTLR_LIBS"
AC_SUBST(LIBRETS_CFLAGS)
AC_SUBST(LIBRETS_LDFLAGS)

AC_ARG_ENABLE(debug,
              AC_HELP_STRING([--enable-debug],
                             [enable debugging support (default: disabled)]),
               test "X$enableval" != "Xno" && my_enable_debug=yes,
               my_enable_debug=no)

my_compile_type="Normal"
if test -n "$GCC"; then
  if test "$my_enable_debug" = "yes"; then
    CFLAGS="-g"
    LDFLAGS="-g"
    my_compile_type="Debug"
  fi
fi

AC_CONFIG_FILES([Makefile project/build/Doxyfile])
AC_CONFIG_FILES([librets-config], [chmod +x librets-config])
AC_CONFIG_FILES([librets-config-inplace], [chmod +x librets-config-inplace])

AC_OUTPUT

echo
echo Option summary:
echo
echo Use ccache .................: $my_use_ccache
echo Use dependency checking ....: $my_use_depends
echo Compile type................: $my_compile_type
echo Compile examples............: $my_use_examples
echo
