dnl
dnl Test for swig and the swig languages
dnl
AC_DEFUN([MY_TEST_SWIG], [
  AC_CACHE_VAL(my_cv_swig_vers, [
    my_cv_swig_vers=NONE
    dnl check is the plain-text version of the required version
    check="1.3.31"

    set `perl -e "@v = split('\\.', '$check'); print \"@v\";"`
    check_major=$[1]
    check_minor=$[2]
    check_release=$[3]

    HAVE_MCS=0
    HAVE_PYTHON=0
    HAVE_RUBY=0
    USE_SWIG_BINDINGS=
    my_have_mcs=no
    my_have_python=no
    my_have_ruby=no
    my_use_swig_bindings=no

    AC_CHECK_PROG(SWIG, swig, swig, no)
    if test "$SWIG" != "no"; then
      AC_MSG_CHECKING([for swig >= $check])

      dnl Sorry, I never professed to being a perl programmer
      ver=`$SWIG -version | tr "\n" " "`
      ver=`perl -e "@v = split(' ', '$ver');  print \"@v[[2]]\";"`
      set `perl -e "@v = split('\\.' , '$ver'); print \"@v\";"`

      ver_major=$[1]
      ver_minor=$[2]
      ver_release=$[3]

      ok=`expr \
          $ver_major \> $check_major \| \
          $ver_major \= $check_major \& \
          $ver_minor \> $check_minor \| \
          $ver_minor \= $check_minor \& \
          $ver_release \>\= $check_release `                         

      if test "$ok" == "1"; then
        my_cv_swig_vers="$ver"
        AC_MSG_RESULT([$my_cv_swig_vers])

        USE_SWIG_BINDINGS=1
        my_use_swig_bindings=yes

	dnl
	dnl Check to see if we can build for csharp
	dnl
	AC_CHECK_PROG(MCS, mcs, mcs, no)
	if test "$MCS" != "no"; then
	    HAVE_MCS=1
	    my_have_mcs=yes
	fi

	dnl
	dnl Check to see if we can build for python
	dnl
	AC_CHECK_PROG(PYTHON, python, python, no)
	if test "$PYTHON" != "no"; then
	    HAVE_PYTHON=1
	    my_have_python=yes
	fi

	dnl
	dnl Check to see if we can build for ruby
	dnl
	AC_CHECK_PROG(RUBY, ruby, ruby, no)
	if test "$RUBY" != "no"; then
	    HAVE_RUBY=1
	    my_have_ruby=yes
	fi

      else
        AC_MSG_RESULT(FAILED)
        AC_MSG_WARN([$ver is too old. Need version $check or higher.])
      fi
    fi

    AC_SUBST(USE_SWIG_BINDINGS)
    AC_SUBST(SWIG)
    AC_SUBST(HAVE_MCS)
    AC_SUBST(MCS)
    AC_SUBST(HAVE_PYTHON)
    AC_SUBST(PYTHON)
    AC_SUBST(HAVE_RUBY)
    AC_SUBST(RUBY)
  ])
])
