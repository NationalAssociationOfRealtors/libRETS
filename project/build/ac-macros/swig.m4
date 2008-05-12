dnl
dnl Test for swig and the swig languages
dnl
AC_DEFUN([MY_TEST_SWIG], [
  AC_CACHE_VAL(my_cv_swig_vers, [
    my_cv_swig_vers=NONE
    dnl check is the plain-text version of the required version
    check="1.3.33"

    set `perl -e "@v = split('\\.', '$check'); print \"@v\";"`
    check_major=$[1]
    check_minor=$[2]
    check_release=$[3]

    HAVE_JAVA=0
    HAVE_MCS=0
    HAVE_PHP=0
    HAVE_PYTHON=0
    HAVE_RUBY=0
    USE_SWIG_BINDINGS=
    my_have_java=no
    my_have_mcs=no
    my_have_php=no
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
        if test "$my_use_mcs" == "yes"; then
            AC_CHECK_PROG(MCS, mcs, mcs, no)
            if test "$MCS" != "no"; then
                HAVE_MCS=1
                my_have_mcs=yes
            fi
        fi

        dnl
        dnl Check to see if we can build for java
        dnl
	if test "$my_use_java" == "yes"; then
		AC_CHECK_PROG(JAVA, java, java, no)
		if test "$JAVA" != "no"; then
			AC_CHECK_PROG(JAVAC, javac, javac, no)
			if test "$JAVAC" != "no"; then
				AC_CHECK_PROG(JAR, jar, jar, no)
				if test "$JAR" != "no"; then
					HAVE_JAVA=1
					my_have_java=yes
				fi
			fi
		fi
	fi

        dnl
        dnl Check to see if we can build for php
        dnl
	if test "$my_use_php" == "yes"; then
		AC_CHECK_PROG(PHP, php, php, no)
		if test "$PHP" != "no"; then
			HAVE_PHP=1
			my_have_php=yes
		fi
	fi

        dnl
        dnl Check to see if we can build for python
        dnl
	if test "$my_use_python" == "yes"; then
		AC_CHECK_PROG(PYTHON, python, python, no)
		if test "$PYTHON" != "no"; then
			HAVE_PYTHON=1
			my_have_python=yes
		fi
	fi

        dnl
        dnl Check to see if we can build for ruby
        dnl
	if test "$my_use_ruby" == "yes"; then
		AC_CHECK_PROG(RUBY, ruby, ruby, no)
		if test "$RUBY" != "no"; then
			HAVE_RUBY=1
			my_have_ruby=yes
		fi
	fi

      else
        AC_MSG_RESULT(FAILED)
        AC_MSG_WARN([$ver is too old. Need version $check or higher.])
      fi
    fi

    AC_SUBST(USE_SWIG_BINDINGS)
    AC_SUBST(SWIG)
    AC_SUBST(HAVE_JAVA)
    AC_SUBST(JAVA)
    AC_SUBST(JAVAC)
    AC_SUBST(JAR)
    AC_SUBST(HAVE_MCS)
    AC_SUBST(MCS)
    AC_SUBST(HAVE_PHP)
    AC_SUBST(PHP)
    AC_SUBST(HAVE_PYTHON)
    AC_SUBST(PYTHON)
    AC_SUBST(HAVE_RUBY)
    AC_SUBST(RUBY)
  ])
])
