dnl
dnl Add sql-compiler option.  Requires MY_TEST_ANTLR to be run first.
dnl
AC_DEFUN([MY_CHECK_SQL_COMPILER], [
  AC_ARG_ENABLE(
    sql-compiler,
    AC_HELP_STRING([--enable-sql-compiler],
      [build SQL to DMQL compiler (default: enabled)]),
    test "X$enableval" == "Xyes" && my_enable_sql_compiler=yes,
    my_enable_sql_compiler=yes)

    my_use_sql_compiler=no
    USE_SQL_COMPILER=
    if test "$my_enable_sql_compiler"  = "yes"; then
      MY_TEST_ANTLR
      if test "x$ANTLR_LIBS" != "x"; then
        USE_SQL_COMPILER=1
        my_use_sql_compiler=yes
      else
        AC_MSG_WARN([*** Optional SQL compiler requires ANTLR ***])
        AC_MSG_WARN([***  Not compiling SQL compiler ***])
      fi
    fi
    AC_SUBST(USE_SQL_COMPILER)
])
