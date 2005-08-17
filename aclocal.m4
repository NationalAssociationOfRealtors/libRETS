dnl
dnl Tests for ccache and modifies CC and CXX appropriately
dnl
dnl Variables:
dnl  my_use_ccache=(yes|no)
dnl
AC_DEFUN(MY_PROG_CCACHE,[
  AC_ARG_ENABLE(
    ccache,
    AC_HELP_STRING([--enable-ccache],
                   [enable ccache (default: disabled)]),
                   test "X$enableval" != "Xno" && my_enable_ccache=yes,
	                 my_enable_ccache=no)

my_use_ccache=no
if test "$my_enable_ccache" = "yes"; then
  AC_CHECK_PROG(my_have_ccache, ccache, yes, no)
  if test "$my_have_ccache" == "yes"; then
     CC="ccache $CC"
     CXX="ccache $CXX"
     my_use_ccache=yes
   fi
fi
])

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

dnl
dnl Test for libcurl
dnl
AC_DEFUN([MY_TEST_CURL], [
  AC_CACHE_VAL(my_cv_curl_vers, [
    my_cv_curl_vers=NONE
    dnl check is the plain-text version of the required version
    check="7.13.1"
    check_hex="070d01"
 
    AC_MSG_CHECKING([for curl >= $check])
 
    if eval curl-config --version 2>/dev/null >/dev/null; then
      ver=`curl-config --version | perl -pe "s/libcurl //g"`
      hex_ver=`curl-config --vernum`
      ok=`perl -e "print (hex('$hex_ver')>=hex('$check_hex') ? '1' : '0')"`

      if test x$ok != x0; then
        my_cv_curl_vers="$ver"
        AC_MSG_RESULT([$my_cv_curl_vers])

        CURL_PREFIX=`curl-config --prefix`
        curl_a="${CURL_PREFIX}/lib/libcurl.a"
        AC_CHECK_FILE($curl_a, have_curl_a=$curl_a)
        if test -z "$have_curl_a"; then
          AC_MSG_ERROR([libcurl.a required to build librets])
        fi
        CURL_CFLAGS=`curl-config --cflags`
        CURL_LIBS=`curl-config --libs`
        CURL_LIBS=`echo $CURL_LIBS | perl -pe "s/^(-L\S+\s+)?-lcurl\s+//"`
        CURL_LIBS="${CURL_PREFIX}/lib/libcurl.a $CURL_LIBS"
      else
        AC_MSG_RESULT(FAILED)
        AC_MSG_ERROR([$ver is too old. Need version $check or higher.])
      fi
    else
      AC_MSG_RESULT(FAILED)
      AC_MSG_ERROR([curl-config was not found])
    fi
    
    AC_SUBST(CURL_PREFIX)
    AC_SUBST(CURL_CFLAGS)
    AC_SUBST(CURL_LIBS)
  ])
])

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
    boost_filesystem_a="${boost_prefix}/lib/libboost_filesystem.a"
    boost_program_options_a="${boost_prefix}/lib/libboost_program_options.a"
    AC_CHECK_FILE([$boost_version_h], [my_boost_version_h=$boost_version_h])
    AC_CHECK_FILE([$boost_filesystem_a], 
                  [my_boost_filesystem_a=$boost_filesystem_a])
    AC_CHECK_FILE([$boost_program_options_a], 
                  [my_boost_program_options_a=$boost_program_options_a])
    
    test -n "$my_boost_version_h" -a -n "$my_boost_filesystem_a" && break
  done

  if test -z "$my_boost_version_h"; then
    AC_MSG_ERROR([Could not find boost/version.hpp])
  fi
  if test -z "$my_boost_filesystem_a"; then
    AC_MSG_ERROR([Could not find libboost_filesystem.a])
  fi
  if test -z "$my_boost_program_options_a"; then
    AC_MSG_ERROR([Could not find libboost_program_options.a])
  fi
  
  BOOST_CFLAGS="-I${boost_prefix}/include"
  BOOST_LIBS=
  BOOST_FILESYSTEM="$my_boost_filesystem_a"
  BOOST_PROGRAM_OPTIONS="$my_boost_program_options_a"

  AC_SUBST(BOOST_CFLAGS)
  AC_SUBST(BOOST_LIBS)
  AC_SUBST(BOOST_FILESYSTEM)
  AC_SUBST(BOOST_PROGRAM_OPTIONS)
])

dnl
dnl Check for ANTLR
dnl
AC_DEFUN([MY_TEST_ANTLR], [
  AC_CHECK_PROG(ANTLR, antlr, antlr, [])
  if test x$ANTLR == "x"; then
    AC_MSG_ERROR([antlr is required to build librets])
  fi
  
  AC_MSG_CHECKING([whether linking with -lantlr works])
  save_LIBS=$LIBS
  LIBS="$LIBS -lantlr"
  AC_LANG_PUSH(C++)
  AC_LINK_IFELSE([
#include <antlr/CommonAST.hpp>
class TestAST : public ANTLR_USE_NAMESPACE(antlr)CommonAST {
};

int main(int, char * argv[])
{
  TestAST testAST;
  return 0;
}
  ], [
    AC_MSG_RESULT(yes)
    ANTLR_LIBS=-lantlr
  ], [
    AC_MSG_RESULT(no)
    AC_MSG_ERROR([Could not link with -lantlr])
  ]
  )
  AC_LANG_POP()
  LIBS=$save_LIBS
  
  AC_SUBST(ANTLR)
  AC_SUBST(ANTLR_LIBS)
])