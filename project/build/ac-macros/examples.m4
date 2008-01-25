dnl
dnl Add examples option.  Requires MY_TEST_BOOST to be run first.
dnl
AC_DEFUN([MY_CHECK_EXAMPLES], [
  AC_ARG_ENABLE(
    examples,
    AC_HELP_STRING([--enable-examples],
      [build example programs (default: disabled)]),
    [test "$enableval" = "yes" && my_enable_examples=yes],
    [my_enable_examples=no])

    my_use_examples=no
    USE_EXAMPLES=0
    if test "$my_enable_examples"  = "yes"; then
      if test "x$BOOST_PROGRAM_OPTIONS" != "x"; then
        USE_EXAMPLES=1
        my_use_examples=yes
      else
        AC_MSG_WARN([*** Examples require Boost program options ***])
        AC_MSG_WARN([***  Not compiling examples ***])
      fi
    fi
    AC_SUBST(USE_EXAMPLES)
])
