dnl  Copyright (C) 2010 Brian Aker
dnl This file is free software; Brian Aker
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.

AC_DEFUN([PANDORA_WITH_VALGRIND],[

  AC_ARG_WITH([valgrind],
    [AS_HELP_STRING([--with-valgrind],
       [Add additional valgrind code for testing with valgrind.])],
    [with_valgrind=yes],
    [with_valgrind=no])
  AM_CONDITIONAL(HAVE_VALGRIND,[test "x${with_valgrind}" = "xyes"])
  AS_IF([test "x$with_valgrind" = "xyes"],
        AC_DEFINE([HAVE_VALGRIND],[1],
                  [Add additional valgrind code for testing with valgrind.]))
])
