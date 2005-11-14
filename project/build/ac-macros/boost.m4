dnl
dnl Configure Boost
dnl
AC_DEFUN([MY_TEST_BOOST], [
  AC_ARG_WITH(
    [boost-prefix],
    AC_HELP_STRING(
      [--with-boost-prefix=PATH],
      [find the Boost headers and libraries in `PATH/include` and  `PATH/lib`.
       By default, checks in /usr and /usr/local.
      ]),
    boost_prefixes="$withval",
	  boost_prefixes="/usr/local /usr")

  if test x"$with_boost" == "xno"; then
	 AC_MSG_ERROR([Boost is required to build librets])
  fi

  for boost_prefix in $boost_prefixes
  do
    boost_version_h="${boost_prefix}/include/boost/version.hpp"
    AC_CHECK_FILE([$boost_version_h], [my_boost_version_h=$boost_version_h])
    test -n "$my_boost_version_h" && break
  done

  if test -z "$my_boost_version_h"; then
    HAVE_BOOST=0
  else
    HAVE_BOOST=1
    BOOST_PREFIX=$boost_prefix
    BOOST_CFLAGS="-I${BOOST_PREFIX}/include"
    BOOST_LIBS=

		check="1_33_0"
		check_int=103300
    AC_MSG_CHECKING([for boost >= $check])
		
    ver=`perl -ane "print /\#define\s+BOOST_LIB_VERSION\s+\"(\S+)\"/" ${BOOST_PREFIX}/include/boost/version.hpp`
		int_ver=`perl -ane "print /\#define\s+BOOST_VERSION\s+(\S+)/" ${BOOST_PREFIX}/include/boost/version.hpp`
    ok=`perl -e "print (($int_ver>=$check_int) ? '1' : '0')"`
    if test x$ok != x0; then
    	my_cv_boost_vers="$ver"
      AC_MSG_RESULT([$my_cv_boost_vers])
    else
      AC_MSG_RESULT(FAILED)
      AC_MSG_ERROR([$ver is too old. Need version $check or higher.])
    fi

    my_lib="${BOOST_PREFIX}/lib/libboost_filesystem.a"
    AC_CHECK_FILE([$my_lib], [BOOST_FILESYSTEM=$my_lib])
    my_lib="${BOOST_PREFIX}/lib/libboost_program_options.a"
    AC_CHECK_FILE([$my_lib], [BOOST_PROGRAM_OPTIONS=$my_lib])
  fi

  AC_SUBST(HAVE_BOOST)
  AC_SUBST(BOOST_PREFIX)
  AC_SUBST(BOOST_CFLAGS)
  AC_SUBST(BOOST_LIBS)
  AC_SUBST(BOOST_FILESYSTEM)
  AC_SUBST(BOOST_PROGRAM_OPTIONS)
])
