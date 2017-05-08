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

## libcrypto

# AX_CHECK_CRYPTO_LIB([algo-name],[function])
AC_DEFUN([AX_CHECK_CRYPTO_LIB],[
  AC_CHECK_LIB([crypto], [$2], [
    AC_DEFINE([CRYPTO_WITH_$1], 1, [Algorithm $1 in openssl crypto library])
  ])
])

AC_MSG_CHECKING([whether openssl crypto is available])
AC_CHECK_HEADERS([openssl/opensslconf.h], [
  AC_CHECK_LIB([crypto], [CRYPTO_lock], [
    AC_DEFINE([HAVE_CRYPTO], 1, [OpenSSL crypto library is available])
    HAVE_CRYPTO=1
    AC_SUBST([CRYPTO_LIBS], [-lcrypto])

    # digests
    AX_CHECK_CRYPTO_LIB([RIPEMD], [RIPEMD160])
    AX_CHECK_CRYPTO_LIB([SHA], [SHA1])
    AX_CHECK_CRYPTO_LIB([SHA256], [SHA256])
  ])
])

# complain if libcrypto could not be found
AS_IF([test "x$HAVE_CRYPTO" != "x"], [
  LIBS="$CRYPTO_LIBS $LIBS"
],[
  AC_MSG_ERROR([cannot configure required openssl crypto library])
])


## libcurl

AC_MSG_CHECKING([whether libcurl is available])
AC_CHECK_HEADERS([curl/curl.h], [
  AC_CHECK_LIB([curl], [curl_easy_init], [
    CURL_LIBS="-lcurl"

    # this snippet has been stolen from curl's docs/libcurl/libcurl.m4
    AC_CACHE_CHECK([whether libcurl is usable], [libcurl_cv_lib_curl_usable], [
      _libcurl_save_libs="$LIBS"
      LIBS="$CURL_LIBS $LIBS"
      AC_LINK_IFELSE([
        AC_LANG_PROGRAM([[#include <curl/curl.h>]], [[
          /* Try and use a few common options to force a failure if we are
             missing symbols or can't link. */
          int x;
          curl_easy_setopt(NULL,CURLOPT_URL,NULL);
          x=CURL_ERROR_SIZE;
          x=CURLOPT_WRITEFUNCTION;
          x=CURLOPT_WRITEDATA;
          x=CURLOPT_ERRORBUFFER;
          x=CURLOPT_STDERR;
          x=CURLOPT_VERBOSE;
          if (x) {;}
        ]])
      ], libcurl_cv_lib_curl_usable=yes, libcurl_cv_lib_curl_usable=no)

      LIBS="$_libcurl_save_libs"
      unset _libcurl_save_libs
    ])

    if test "x$libcurl_cv_lib_curl_usable" = "xyes" ; then
      AC_DEFINE([HAVE_LIBCURL], 1, [cURL library is available])
      HAVE_LIBCURL=1
      AC_SUBST([CURL_LIBS], [$CURL_LIBS])
    else
      AC_MSG_ERROR([libcurl is installed but not useable])
    fi
  ])
])

# complain if libcurl could not be found
AS_IF([test "x$HAVE_LIBCURL" != "x"], [
  LIBS="$LIBS $CURL_LIBS"
], [
  AC_MSG_ERROR([cannot configure required curl library])
])


## RapidJSON

AC_MSG_CHECKING([whether RapidJSON is available])
AC_CHECK_HEADERS([rapidjson/rapidjson.h], [
  AC_DEFINE([HAVE_RAPIDJSON], 1, [RapidJSON is available])
  HAVE_RAPIDJSON=1
])

# complain if RapidJSON could not be found
AS_IF([test "x$HAVE_RAPIDJSON" = "x"], [
  AC_MSG_ERROR([cannot configure required RapidJSON library])
])


AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
