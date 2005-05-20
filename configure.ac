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
AC_CHECK_PROG(my_have_doxygen, doxygen, yes, no)
MY_PROG_CCACHE
AC_CACHE_SAVE

MY_TEST_DEPENDS

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

AC_CONFIG_FILES([
  Makefile
  project/build/Doxyfile
])

AC_OUTPUT

echo
echo Option summary:
echo
echo Use ccache .................: $my_use_ccache
echo Use dependency checking ....: $my_use_depends
echo Compile type................: $my_compile_type
echo
