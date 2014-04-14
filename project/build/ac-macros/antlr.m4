dnl
dnl Check for ANTLR
dnl
AC_DEFUN([MY_TEST_ANTLR], [
  AC_ARG_WITH(
    [antlr-prefix],
    AC_HELP_STRING(
      [--with-antlr-prefix=PATH],
      [find the Antlr headers and libraries in `PATH/include` and `PATH/lib`.
       By default, checks in $default_search_path.
      ]),
    antlr_prefxes="$withval",
      antlr_prefixes="$default_search_path")

  AC_CHECK_PROG(ANTLR_JAVA, java, java, [])
  if test x$ANTLR_JAVA = "x"; then
    AC_MSG_ERROR([antlr requires java to run])
  fi
  AC_CHECK_PROG(ANTLR, antlr, antlr, [])
  if test x$ANTLR == "x"; then
     AC_CHECK_PROG(ANTLR, runantlr, runantlr, [])
  fi
  if test x$ANTLR != "x"; then

    save_CFLAGS=$CPPFLAGS
    save_LIBS=$LIBS
    AC_LANG_PUSH(C++)
    for antlr_prefix in $antlr_prefixes
    do
        AC_MSG_CHECKING([whether linking with -lantlr in $antlr_prefix works])
        CPPFLAGS="$save_CFLAGS -I$antlr_prefix/include"
        LIBS="$save_LIBS -L$antlr_prefix/lib -lantlr"
        AC_LINK_IFELSE([
      #include <antlr/CommonAST.hpp>
      class TestAST : public ANTLR_USE_NAMESPACE(antlr)CommonAST {
      };

      int main(int, char **argv)
      {
        TestAST testAST;
        return 0;
      }
        ], [
          AC_MSG_RESULT(yes)
          ANTLR_CFLAGS="-I$antlr_prefix/include"
          ANTLR_LIBS="-L$antlr_prefix/lib -lantlr"
          break
        ], [
          AC_MSG_RESULT(no)
          dnl AC_MSG_ERROR([Could not link with -lantlr])
        ]
        )
    done

    AC_LANG_POP()
    CPPFLAGS=$save_CFLAGS
    LIBS=$save_LIBS
  fi

  AC_SUBST(ANTLR)
  AC_SUBST(ANTLR_CFLAGS)
  AC_SUBST(ANTLR_LIBS)
])
