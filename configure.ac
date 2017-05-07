AC_INIT([teamsync-client],[0.0.1])

AC_CONFIG_AUX_DIR(config)
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([subdir-objects])

### Determine which C compiler to use (we expect to find gcc).

AC_PROG_CC
AC_PROG_CPP
AC_PROG_GCC_TRADITIONAL


dnl Switch to a C++ compiler, and check if it works.
AC_LANG(C++)
AC_PROG_CXX
AC_PROG_CXXCPP
AC_SUBST([CC])


### Libraries

# AX_CHECK_CRYPTO_LIB([algo-name],[function])
AC_DEFUN([AX_CHECK_CRYPTO_LIB],[
  AC_CHECK_LIB([crypto], $2, [
    AC_DEFINE([CRYPTO_WITH_$1],[1],[Algorithm $1 in openssl crypto library])
  ])
])

# libcrypto
AC_MSG_CHECKING([whether openssl crypto is available])
AC_CHECK_HEADERS([openssl/opensslconf.h],[
  AC_CHECK_LIB([crypto], [CRYPTO_lock],[
    AC_DEFINE([HAVE_CRYPTO],[1],[OpenSSL crypto library is available])
    HAVE_CRYPTO=1
    AC_SUBST([CRYPTO_LIBS],[-lcrypto])

    # digests
    AX_CHECK_CRYPTO_LIB([RIPEMD],[RIPEMD160])
    AX_CHECK_CRYPTO_LIB([SHA],[SHA1])
    AX_CHECK_CRYPTO_LIB([SHA256],[SHA256])
  ])
])

# complain if libcrypto could not be found
AS_IF([test "x$HAVE_CRYPTO" != "x"],[
  LIBS="$LIBS -lcrypto"
],[
  AC_MSG_ERROR([cannot configure required openssl crypto library])
])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
