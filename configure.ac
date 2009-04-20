dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.52)
AC_INIT([libRETS], [1.0], [librets@crt.realtors.org])

dnl AC_LANG([C++])
AC_CONFIG_SRCDIR([project/librets/include/librets.h])
AC_CONFIG_AUX_DIR([project/build])
AC_CONFIG_HEADERS([project/librets/src/config.h])


dnl Checks for programs
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_RANLIB
AC_CHECK_PROG(my_have_perl, perl, yes, no)
if test x"$my_have_perl" != x"yes"; then
    AC_MSG_ERROR([perl is required to build librets])
fi

VERSION=`perl -ane 'print $1 if /^VERSION\s+\=\s+(\S+)/' ${ac_top_builddir}project/build/version.mk`
AC_MSG_CHECKING([librets version])
AC_MSG_RESULT($VERSION)
AC_SUBST(VERSION)
HEX_VERSION=`perl -ane 'print $1 if /^HEX_VERSION\s+\=\s+(\S+)/' ${ac_top_builddir}project/build/version.mk`
AC_SUBST(HEX_VERSION)

AC_ARG_ENABLE(maintainer-documentation,
              AC_HELP_STRING([--enable-maintainer-documentation],
                             [Enables documentation used by libRETS maintainers.]),
              test "X$enableval" != "Xno" && my_enable_maintainer_documentation=yes,
              my_enable_maintainer_documentation=no)

AC_CHECK_PROG(my_have_doxygen, doxygen, yes, no)
if test x"$my_have_doxygen" = "xyes"; then
  HAVE_DOXYGEN=1
else
  HAVE_DOXYGEN=0
fi
AC_SUBST(HAVE_DOXYGEN)

if test x"$my_enable_maintainer_documentation" = "xyes"; then
  ENABLED_SECTIONS=MAINTAINER
else
  ENABLED_SECTIONS=
fi
AC_SUBST(ENABLED_SECTIONS)

MY_PROG_CCACHE
AC_CACHE_SAVE

AC_ARG_ENABLE(shared_dependencies,
              AC_HELP_STRING([--enable-shared_dependencies],
                             [Uses shared libraries for dependencies instead of their static counterparts.  Do not enable unless you know what you're doing.]),
              test "X$enableval" != "Xno" && my_enable_shared_dependencies=yes,
              my_enable_shared_dependencies=no)

AC_ARG_ENABLE(dotnet,
              AC_HELP_STRING([--disable-dotnet],
                             [If swig is installed, do not build the .NET bindings.]),
              test "X$enableval" != "Xyes" && my_use_mcs=no,
              my_use_mcs=yes)
		
AC_ARG_ENABLE(java,
              AC_HELP_STRING([--disable-java],
                             [If swig is installed, do not build the java  bindings.]),
              test "X$enableval" != "Xyes" && my_use_java=no,
              my_use_java=yes)
		
AC_ARG_ENABLE(perl,
              AC_HELP_STRING([--disable-perl],
                             [If swig is installed, do not build the Perl bindings.]),
              test "X$enableval" != "Xyes" && my_use_perl=no,
              my_use_perl=yes)
		
AC_ARG_ENABLE(php,
              AC_HELP_STRING([--disable-php],
                             [If swig is installed, do not build the PHP bindings.]),
              test "X$enableval" != "Xyes" && my_use_php=no,
              my_use_php=yes)
		
AC_ARG_ENABLE(python,
              AC_HELP_STRING([--disable-python],
                             [If swig is installed, do not build the python bindings.]),
              test "X$enableval" != "Xyes" && my_use_python=no,
              my_use_python=yes)
		
AC_ARG_ENABLE(ruby,
              AC_HELP_STRING([--disable-ruby],
                             [If swig is installed, do not build the ruby  bindings.]),
              test "X$enableval" != "Xyes" && my_use_ruby=no,
              my_use_ruby=yes)
		
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

MY_CHECK_SQL_COMPILER

MY_TEST_CPPUNIT

MY_CHECK_EXAMPLES

MY_TEST_SWIG

AC_ARG_ENABLE(debug,
              AC_HELP_STRING([--enable-debug],
                             [enable debugging support (default: disabled)]),
               test "X$enableval" != "Xno" && my_enable_debug=yes,
               my_enable_debug=no)

my_compile_type="Normal"
if test -n "$GCC"; then
  if test "$my_enable_debug" = "yes"; then
    CFLAGS="-g"
    LDFLAGS="$LDFLAGS -g"
    my_compile_type="Debug"
  fi
fi

LIBRETS_CFLAGS="$BOOST_CFLAGS $ANTLR_CFLAGS"
LIBRETS_LDFLAGS="$BOOST_LIBS $BOOST_FILESYSTEM $BOOST_SYSTEM $CURL_LIBS $EXPAT_LIBS $ANTLR_LIBS"

AC_ARG_ENABLE(PIC,
              AC_HELP_STRING([--disable-PIC],
                             [Disable the use of -fPIC when compiling (default: enabled).]),
               test "X$enableval" != "Xyes" && my_use_pic=no,
               my_use_pic=yes)

if test "$my_use_pic" = "yes"; then
   CFLAGS="$CFLAGS -fPIC"
   LIBRETS_CFLAGS="$LIBRETS_CFLAGS -fPIC"
fi

AC_SUBST(LIBRETS_CFLAGS)
AC_SUBST(LIBRETS_LDFLAGS)

AC_CONFIG_FILES([Makefile project/build/Doxyfile])
AC_CONFIG_FILES([librets-config], [chmod +x librets-config])
AC_CONFIG_FILES([librets-config-inplace], [chmod +x librets-config-inplace])

AC_OUTPUT

echo
echo Option summary:
echo
echo Use ccache .................: $my_use_ccache
echo Use dependency checking ....: $my_use_depends
echo Use -fPIC...................: $my_use_pic
echo Use shared dependencies.....: $my_enable_shared_dependencies
echo Compile type................: $my_compile_type
echo Compile examples............: $my_use_examples
echo Compile SQL compiler........: $my_use_sql_compiler
echo Compile SWIG bindings.......: $my_use_swig_bindings
  echo "  With CSharp...............: $my_have_mcs"
  echo "  With Java.................: $my_have_java"
  echo "  With PERL.................: $my_have_perl"
  echo "  With PHP..................: $my_have_php"
  echo "  With Python...............: $my_have_python"
  echo "  With Ruby.................: $my_have_ruby"
echo Enable Maintainer Docs......: $my_enable_maintainer_documentation


echo
