AC_INIT([teamsync-client],[0.0.1])

AM_INIT_AUTOMAKE([subdir-objects])

dnl Switch to a C++ compiler, and check if it works.
AC_LANG(C++)
AC_PROG_CXX
AC_PROG_CXXCPP
AC_SUBST([CC])

### Determine which C compiler to use (we expect to find gcc).

AC_PROG_CC
AC_PROG_CPP
AC_PROG_GCC_TRADITIONAL

AC_CONFIG_FILES([src/Makefile])

AC_OUTPUT([Makefile])
