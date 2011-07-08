dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(ouzo_datagen, 1.0)


AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE()

AC_PROG_CXX
AC_PROG_CC

AC_CHECK_LIB(protobuf, main)
AC_CHECK_LIB(pthread, main)
AC_CHECK_LIB(dl, main)

AC_CONFIG_FILES(Makefile src/Makefile)
AC_OUTPUT

