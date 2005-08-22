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
    expat_a="${expat_prefix}/lib/libexpat.a"
    AC_CHECK_FILE([$expat_h], [my_expat_h=$expat_h])
    AC_CHECK_FILE([$expat_a], [my_expat_a=$expat_a])
    test -n "$my_expat_h" -a -n "$my_expat_a" && break
  done

  if test -z "$my_expat_h"; then
    AC_MSG_ERROR([Could not find expat.h])
  fi
  if test -z "$my_expat_a"; then
    AC_MSG_ERROR([Could nto find libexpat.h])
  fi

  EXPAT_CFLAGS="-I${expat_prefix}/include"
  EXPAT_LIBS="${expat_a}"

  AC_SUBST(EXPAT_CFLAGS)
  AC_SUBST(EXPAT_LIBS)
])
