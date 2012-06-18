dnl Copyright (C) 2010 Monty Taylor
dnl Copyright (C) 2010 Hartmut Holzgraefe
dnl This file is free software; Monty Taylor and Hartmut Holzgraefe
dnl give unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.

AC_DEFUN([_PANDORA_SEARCH_FLEX],[

  dnl --------------------------------------------------------------------
  dnl  Check for flex
  dnl --------------------------------------------------------------------

  AC_CHECK_PROGS([LEX], ['flex'], [:])
  AS_IF([test "x$LEX" = "x:"],[
    pandora_have_flex=no
    LEX='if test -f "$@"; then echo "WARNING: no proper flex binary found, ignoring changes to $<"; exit 0; else echo "ERROR: no proper flex binary found"; exit 1; fi;'
    ],[
    pandora_have_flex=yes
    ])

  AM_CONDITIONAL(HAVE_FLEX, [test "x${pandora_have_flex}" = "xyes"])
])

AC_DEFUN([PANDORA_HAVE_FLEX],[
  AC_REQUIRE([_PANDORA_SEARCH_FLEX])
])

AC_DEFUN([PANDORA_REQUIRE_FLEX],[
  AC_REQUIRE([PANDORA_HAVE_FLEX])
  AS_IF([test "x${pandora_have_flex}" = "xno" -a "$pandora_building_from_bzr" = "yes"],
      AC_MSG_ERROR(["flex is required for ${PACKAGE} to build from a bzr branch"])
      )
])
