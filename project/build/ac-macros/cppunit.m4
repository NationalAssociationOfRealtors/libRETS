dnl
dnl Test for cppunit
dnl
AC_DEFUN([MY_TEST_CPPUNIT], [
  AC_CACHE_VAL(my_cv_cppunit_vers, [
    my_cv_cppunit_vers=NONE
    dnl check is the plain-text version of the required version
    check="1.10.2"

    set `${PERL} -e "@v = split('\\.', '$check'); print \"@v\";"`
    check_major=$[1]
    check_minor=$[2]
    check_release=$[3]

    HAVE_CPPUNIT=0
    AC_CHECK_PROG(CPPUNIT_CONFIG, cppunit-config, cppunit-config, no)
    if test "$CPPUNIT_CONFIG" != "no"; then
      AC_MSG_CHECKING([for cppunit >= $check])

      ver=`$CPPUNIT_CONFIG --version`
      set `${PERL} -e "@v = split('\\.', '$ver'); print \"@v\";"`
      ver_major=$[1]
      ver_minor=$[2]
      ver_release=$[3]
      ok=`expr \
          $ver_major \> $check_major \| \
          $ver_major \= $check_major \& \
          $ver_minor \> $check_minor \| \
          $ver_minor \= $check_minor \& \
          $ver_release \= $check_release \& \
          $ver_release \>= $check_release `                         

      if test "$ok" = "1"; then
        my_cv_cppunit_vers="$ver"
        AC_MSG_RESULT([$my_cv_cppunit_vers])

        HAVE_CPPUNIT=1
        CPPUNIT_CFLAGS=`$CPPUNIT_CONFIG --cflags`
        CPPUNIT_LIBS=`$CPPUNIT_CONFIG --libs`
      else
        AC_MSG_RESULT(FAILED)
        AC_MSG_WARN([$ver is too old. Need version $check or higher.])
      fi
    fi
  ])

  AC_SUBST(HAVE_CPPUNIT)
  AC_SUBST(CPPUNIT_CFLAGS)
  AC_SUBST(CPPUNIT_LIBS)
])
