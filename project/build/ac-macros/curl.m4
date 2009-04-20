dnl
dnl Test for libcurl
dnl
AC_DEFUN([MY_TEST_CURL], [
  AC_CACHE_VAL(my_cv_curl_vers, [
    my_cv_curl_vers=NONE
    dnl check is the plain-text version of the required version
    check="7.18.2"
    check_hex="071202"

    AC_MSG_CHECKING([for curl >= $check])

    if eval curl-config --version 2>/dev/null >/dev/null; then
      ver=`curl-config --version | perl -pe "s/libcurl //g"`
      hex_ver=`curl-config --vernum`
      ok=`perl -e "print (hex('$hex_ver')>=hex('$check_hex') ? '1' : '0')"`

      if test x$ok != x0; then
        my_cv_curl_vers="$ver"
        AC_MSG_RESULT([$my_cv_curl_vers])

        CURL_PREFIX=`curl-config --prefix`
        CURL_CFLAGS=`curl-config --cflags`
        CURL_LIBS=`curl-config --libs`

        if test "$my_enable_shared_dependencies" != "yes"; then
          dnl See if curl version is greater than 7.17.00. If so, change CURL_LIBS

          ok=`perl -e "print (hex('$hex_ver')>=hex('071100') ? '1' : '0')"`
          if test x$ok != x0; then
            CURL_LIBS=`curl-config --static-libs`
          fi

          curl_a="${CURL_PREFIX}/lib/libcurl.a"
          AC_CHECK_FILE($curl_a, have_curl_a=$curl_a)
          if test -z "$have_curl_a"; then
            AC_MSG_ERROR([libcurl.a required to build librets])
          fi
          CURL_LIBS=`echo $CURL_LIBS | perl -pe "s/^(-L\S+\s+)?-lcurl\s+//"`
          CURL_LIBS="${CURL_PREFIX}/lib/libcurl.a $CURL_LIBS"
        fi
      else
        AC_MSG_RESULT(FAILED)
        AC_MSG_ERROR([$ver is too old. Need version $check or higher.])
      fi
    else
      AC_MSG_RESULT(FAILED)
      AC_MSG_ERROR([curl-config was not found])
    fi
  ])

  AC_SUBST(CURL_PREFIX)
  AC_SUBST(CURL_CFLAGS)
  AC_SUBST(CURL_LIBS)
])
