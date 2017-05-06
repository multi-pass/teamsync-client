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

AX_LIB_CRYPTO([yes])
AX_CHECK_CRYPTO_LIB([SHA256],[SHA256])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
