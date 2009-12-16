dnl
dnl Configure Universal Builds on OS X
dnl
AC_DEFUN([MY_TEST_DARWIN], [
  AC_ARG_ENABLE(universal,
              AC_HELP_STRING([--enable-universal],
                             [Enable Universal objects (on OS X only)]),
              test "X$enableval" == "Xyes" && my_enable_universal=yes,
              my_enable_universal=no)
  if test "$my_enable_universal" = "yes"; then

      save_CXXFLAGS="$CXXFLAGS"
      save_LDFLAGS="$LDFLAGS"
      AC_LANG_PUSH(C++)

      darwin_cxxflags="${BOOST_CFLAGS} ${CURL_CFLAGS} ${EXPAT_CFLAGS} ${ANTLR_CFLAGS} -arch_errors_fatal"
      darwin_ldflags="${BOOST_LIBS} ${CURL_LIBS} ${EXPAT_LIBS} ${ANTLR_LIBS} ${BOOST_PROGRAM_OPTIONS} -arch_errors_fatal"

      CXXFLAGS="-arch i386 ${darwin_cxxflags}"
      LDFLAGS="-arch i386 -arch_errors_fatal ${darwin_ldflags}"
      AC_MSG_CHECKING([whether we can build i386 objects])
      AC_TRY_LINK(
            [
                #include <stdlib.h>
                #include <antlr/CommonAST.hpp>
                #include <curl/curl.h>
                #include <expat.h>
                #include <boost/program_options.hpp>
                #include <zlib.h>
            ],
            [
                boost::program_options::variables_map vm;
                XML_ExpatVersion();
                curl_version();
                zlibVersion();
                boost::program_options::notify(vm);
                antlr::RefAST factory();
            ],
            [
                AC_MSG_RESULT(yes)
                ARCH_FLAGS="$ARCH_FLAGS -arch i386"
            ],
            [AC_MSG_RESULT(no)]
            )

      CXXFLAGS="-arch x86_64 ${darwin_cxxflags}"
      LDFLAGS="-arch x86_64 -arch_errors_fatal ${darwin_ldflags}"
      AC_MSG_CHECKING([whether we can build x86_64 objects])
      AC_TRY_LINK(
            [
                #include <stdlib.h>
                #include <antlr/CommonAST.hpp>
                #include <curl/curl.h>
                #include <expat.h>
                #include <boost/program_options.hpp>
                #include <zlib.h>
            ],
            [
                boost::program_options::variables_map vm;
                XML_ExpatVersion();
                curl_version();
                zlibVersion();
                boost::program_options::notify(vm);
                antlr::RefAST factory();
            ],
            [
                AC_MSG_RESULT(yes)
                ARCH_FLAGS="$ARCH_FLAGS -arch x86_64"
            ],
            [AC_MSG_RESULT(no)]
            )

      CXXFLAGS="-arch ppc ${darwin_cxxflags}"
      LDFLAGS="-arch ppc -arch_errors_fatal ${darwin_ldflags}"
      AC_MSG_CHECKING([whether we can build ppc objects])
      AC_TRY_LINK(
            [
                #include <stdlib.h>
                #include <antlr/CommonAST.hpp>
                #include <curl/curl.h>
                #include <expat.h>
                #include <boost/program_options.hpp>
                #include <zlib.h>
            ],
            [
                boost::program_options::variables_map vm;
                XML_ExpatVersion();
                curl_version();
                zlibVersion();
                boost::program_options::notify(vm);
                antlr::RefAST factory();
            ],
            [
                AC_MSG_RESULT(yes)
                ARCH_FLAGS="$ARCH_FLAGS -arch ppc"
            ],
            [AC_MSG_RESULT(no)]
            )


      AC_LANG_POP()
      CFLAGS="$save_CFLAGS"
      LDFLAGS="$save_LDFLAGS"

  fi
])
