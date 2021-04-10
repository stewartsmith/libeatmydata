#
# Libeatmydata
#
# Build infrastructure:
# Copyright (C) 2009-2010 Monty Taylor, Eric Day
# Copyright (C) 2010-2021 Stewart Smith
# All rights reserved.
#
# Use and distribution licensed under the BSD license.  See
# the COPYING file in this directory for full text.
#


AC_INIT([libeatmydata],[129], [http://www.flamingspork.com/projects/libeatmydata])
AC_CONFIG_SRCDIR([libeatmydata/libeatmydata.c])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([m4])

AC_CANONICAL_TARGET
AC_ARG_PROGRAM
AC_USE_SYSTEM_EXTENSIONS

AM_INIT_AUTOMAKE([-Wall -Werror -Wno-portability subdir-objects foreign tar-ustar])

AC_PREREQ([2.63])

LT_PREREQ([2.2])
LT_INIT
LT_LANG([C])

AC_CONFIG_HEADERS([config.h])

m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_CHECK_HEADERS_ONCE(pthread.h)

AC_CHECK_SIZEOF(mode_t)
AC_CHECK_SIZEOF(int)

AC_CHECK_TYPE(pthread_barrier_t,,,[
  #ifdef HAVE_PTHREAD_H
  #include <pthread.h>
  #endif
  ])

AM_CONDITIONAL(HAVE_PTHREAD_BARRIERS, [test "x$ac_cv_type_pthread_barrier_t" = xyes])

AC_CHECK_DECLS(fdatasync)
AC_CHECK_FUNCS(fdatasync)
AC_CHECK_DECLS(sync_file_range)
AC_CHECK_FUNCS(sync_file_range)
AC_CHECK_FUNCS(open64)

AX_TLS(:,:)

AC_CONFIG_FILES(Makefile libeatmydata.spec)

AC_OUTPUT

