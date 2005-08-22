dnl
dnl Enable dependency checking
dnl
dnl Variables:
dnl  my_use_depends=(yes|no)
dnl
AC_DEFUN(MY_TEST_DEPENDS, [
  AC_ARG_ENABLE(
    depends,
    AC_HELP_STRING([--enable-depends],
                   [enable dependency checking (default: disabled)]),
    test "X$enableval" != "Xno" && my_enable_depends=yes,
    my_enable_depends=no)

  my_use_depends=no
  USE_DEPENDS=0
  if test "$my_enable_depends" = "yes"; then
    if test "$my_have_perl" = "yes"; then
      USE_DEPENDS=1
      my_use_depends=yes
    else
      AC_MSG_ERROR([*** Dependecy checking requires Perl ***])
    fi
  fi
  AC_SUBST(USE_DEPENDS)
])
