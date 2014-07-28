dnl
dnl Configure Boost
dnl
AC_DEFUN([MY_TEST_BOOST], [
  AC_ARG_WITH(
    [boost-prefix],
    AC_HELP_STRING(
      [--with-boost-prefix=PATH],
      [find the Boost headers and libraries in `PATH/include` and  `PATH/lib`.
       By default, checks in $default_search_path.
      ]),
    boost_prefixes="$withval",
      boost_prefixes="$default_search_path")

  if test x"$with_boost" = "xno"; then
     AC_MSG_ERROR([Boost is required to build librets])
  fi

  boost_include_dir="include"
  for boost_prefix in $boost_prefixes
  do
    boost_include_dir="include"
    boost_version_h="${boost_prefix}/${boost_include_dir}/boost/version.hpp"
    AC_CHECK_FILE([$boost_version_h], [my_boost_version_h=$boost_version_h])
    test -n "$my_boost_version_h" && break
    for i in `ls -d $boost_prefix/include/boost-* 2>/dev/null`; do
      boost_version=`echo $i | sed "s#$boost_prefix##" | sed "s/\/include\/boost-//"`
      boost_include_dir="include/boost-${boost_version}"
      boost_version_h="${boost_prefix}/${boost_include_dir}/boost/version.hpp"
      AC_CHECK_FILE([$boost_version_h], [my_boost_version_h=$boost_version_h])
      test -n "$my_boost_version_h" && break
    done
    test -n "$my_boost_version_h" && break
  done

  if test -z "$my_boost_version_h"; then
    HAVE_BOOST=0
  else
    HAVE_BOOST=1
    BOOST_PREFIX=$boost_prefix
    BOOST_INCLUDE_DIR="${BOOST_PREFIX}/${boost_include_dir}"
    BOOST_CFLAGS="-I${BOOST_INCLUDE_DIR}"
    BOOST_LIBS=

    check="1_44"
    check_int=104400
    AC_MSG_CHECKING([for boost >= $check])
        
    ver=`grep -e "#define.*BOOST_LIB_VERSION" ${BOOST_PREFIX}/${boost_include_dir}/boost/version.hpp | cut -d \" -f 2`
    int_ver=`grep -e "#define.*BOOST_VERSION " ${BOOST_PREFIX}/${boost_include_dir}/boost/version.hpp | cut -d ' ' -f 3`
    ok=`${PERL} -e "print (($int_ver>=$check_int) ? '1' : '0')"`
    if test x$ok != x0; then
        my_cv_boost_vers="$ver"
      AC_MSG_RESULT([$my_cv_boost_vers])
    else
      AC_MSG_RESULT(FAILED)
      AC_MSG_ERROR([$ver is too old. Need version $check or higher.])
    fi

    dnl See if the library names include the version number
    for i in `ls ${boost_prefix}/lib/libboost_filesystem*${my_cv_boost_vers}.a 2>/dev/null`; do
      boost_version="-${my_cv_boost_vers}"
      break
    done

    dnl See if the library names include "-mt"
    for i in `ls ${boost_prefix}/lib/libboost_filesystem*-mt${boost_version}.a 2>/dev/null`; do
      boost_mt="-mt"
      break
    done

    dnl See if this library names include "-s" for static libraries
    for in in `ls ${boost_prefix}/lib/libboost_filesystem*${boost_mt}-s${boost_version}.a 2>/dev/null`; do
      boost_static="-s"
      break;
    done

    dnl See if the toolset is included as part of the name
    for i in `ls ${boost_prefix}/lib/libboost_filesystem*${boost_mt}${boost_static}${boost_version}.a 2>/dev/null`; do
      toolset=`echo $i | sed "s#${boost_prefix}/lib/libboost_filesystem##" | sed "s/-mt//" | sed "s/-s//" | sed "s/${boost_version}.a//"`
      test -n ${toolset} && break
      toolset=
    done

    if test "$my_enable_shared_dependencies" = "yes"; then
       # Because its easier, for now we will naively assume that
       # that if the boost headers and version are found, we can just
       # dynamicly link in the libraries.
       BOOST_LIBS="${BOOST_LIBS} -L${BOOST_PREFIX}/lib"
       BOOST_FILESYSTEM="-lboost_filesystem${toolset}${boost_mt}${boost_version}"
       BOOST_PROGRAM_OPTIONS="-lboost_program_options${toolset}${boost_mt}${boost_version}"

       # Boost added the system error stuff in 1.35, and boost_filesystem
       # is dependant on it.  We'll make the naive assumption that if the
       # header is present, the library will also be present.
       boost_system_error_h="${boost_prefix}/${boost_include_dir}/boost/system/system_error.hpp"
       AC_CHECK_FILE([$boost_system_error_h], [my_boost_system_error_h=$boost_system_error_h])
       if test "x$my_boost_system_error_h" != "x"; then
       	  BOOST_SYSTEM="-lboost_system${toolset}${boost_mt}${boost_version}"
       fi

       # If threadsafe was enabled, make sure to include the boost thread library
       if test "$my_enable_thread_safe" = "yes"; then
          BOOST_THREAD="-lboost_thread${toolset}${boost_mt}${boost_version}"
       fi

    else
       my_lib="${BOOST_PREFIX}/lib/libboost_filesystem${toolset}${boost_mt}${boost_version}.a"
       AC_CHECK_FILE([$my_lib], [BOOST_FILESYSTEM=$my_lib])
       my_lib="${BOOST_PREFIX}/lib/libboost_program_options${toolset}${boost_mt}${boost_version}.a"
       AC_CHECK_FILE([$my_lib], [BOOST_PROGRAM_OPTIONS=$my_lib])
       my_lib="${BOOST_PREFIX}/lib/libboost_system${toolset}${boost_mt}${boost_version}.a"
       AC_CHECK_FILE([$my_lib], [BOOST_SYSTEM=$my_lib])
       if test "$my_enable_thread_safe" = "yes"; then
           my_lib="${BOOST_PREFIX}/lib/libboost_thread${toolset}${boost_mt}${boost_version}.a"
           AC_CHECK_FILE([$my_lib], [BOOST_THREAD=$my_lib])
       fi
    fi
  fi

  AC_SUBST(HAVE_BOOST)
  AC_SUBST(BOOST_PREFIX)
  AC_SUBST(BOOST_INCLUDE_DIR)
  AC_SUBST(BOOST_CFLAGS)
  AC_SUBST(BOOST_LIBS)
  AC_SUBST(BOOST_FILESYSTEM)
  AC_SUBST(BOOST_SYSTEM)
  AC_SUBST(BOOST_THREAD)
  AC_SUBST(BOOST_PROGRAM_OPTIONS)
])
