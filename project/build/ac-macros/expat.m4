dnl
dnl Configure Expat
dnl
AC_DEFUN([MY_TEST_EXPAT], [
  AC_ARG_WITH(
    expat-prefix,
    AC_HELP_STRING(
      [--with-expat-prefix=PATH],
      [find the Expat header and library in `PATH/include` and  `PATH/lib`.
       By default, checks in /usr and /usr/local.
      ]),
    expat_prefixes="$withval",
    expat_prefixes="/usr/local /usr")

  if test x"$with_expat" == "xno"; then
     AC_MSG_ERROR([Expat is required to build librets])
  fi

  for expat_prefix in $expat_prefixes
  do
    expat_h="${expat_prefix}/include/expat.h"
    AC_CHECK_FILE([$expat_h], [my_expat_h=$expat_h])
    test -n "$my_expat_h" && break
  done

  if test -z "$my_expat_h"; then
    AC_MSG_ERROR([Could not find expat.h])
  fi

  if test "$my_enable_shared_dependencies" == "yes"; then
     if test x"$withval" != "x"; then
        EXPAT_LDFLAGS="-L$withval/lib"
     fi
     AC_CHECK_LIB([expat], [XML_ParserCreate], [expat_lib="-lexpat"],
               [AC_MSG_ERROR([Can't find expat library. Please install expat.])])
  else
     for expat_prefix in $expat_prefixes
     do
       expat_lib="${expat_prefix}/lib/libexpat.a"
       AC_CHECK_FILE([$expat_lib], [my_expat_a=$expat_lib])
       test -n "$my_expat_a" && break
     done
     if test -z "$my_expat_a"; then
       AC_MSG_ERROR([Could not find libexpat.a])
     fi
  fi

  EXPAT_CFLAGS="-I${expat_prefix}/include"
  EXPAT_LIBS="$EXPAT_LDFLAGS ${expat_lib}"

  AC_SUBST(EXPAT_CFLAGS)
  AC_SUBST(EXPAT_LIBS)
])
