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

AC_CONFIG_FILES([
  Makefile
  project/build/Doxyfile
])
AC_OUTPUT

echo
echo Option summary:
echo
echo Use ccache ................: $my_use_ccache
echo Use dependency checking ...: $my_use_depends
echo
