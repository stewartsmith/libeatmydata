dnl  Copyright (C) 2011 Andrew Hutchings
dnl This file is free software; Sun Microsystems, Inc.
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.

#--------------------------------------------------------------------
# Check for libaio
#--------------------------------------------------------------------


AC_DEFUN([_PANDORA_SEARCH_LIBAIO],[
  AC_REQUIRE([AC_LIB_PREFIX])

  AC_CHECK_HEADERS(libaio.h)
  AC_LIB_HAVE_LINKFLAGS(aio,,
  [
    #include <libaio.h>
  ],
  [
    int aio_cmd= IO_CMD_PREAD;
  ])

  AM_CONDITIONAL(HAVE_LIBAIO, [test "x${ac_cv_libaio}" = "xyes"])

  AS_IF([test "x${ac_cv_libaio}" = "xyes"],[
    AC_DEFINE([LINUX_NATIVE_AIO], [1], [Enabel libaio support in InnoDB])
  ])
])

AC_DEFUN([_PANDORA_HAVE_LIBAIO],[

  AC_ARG_ENABLE([libaio],
    [AS_HELP_STRING([--disable-libaio],
      [Build with libaio support @<:@default=on@:>@])],
    [ac_enable_libaio="$enableval"],
    [ac_enable_libaio="yes"])

  _PANDORA_SEARCH_LIBAIO
])


AC_DEFUN([PANDORA_HAVE_LIBAIO],[
  AC_REQUIRE([_PANDORA_HAVE_LIBAIO])
])

AC_DEFUN([_PANDORA_REQUIRE_LIBAIO],[
  ac_enable_libaio="yes"
  _PANDORA_SEARCH_LIBAIO
  AS_IF([test "x$ac_cv_header_libaio_h" = "xno"],[
    PANDORA_MSG_ERROR([Couldn't find libaio.h. On Debian this can be found in libaio-dev. On Redhat this can be found in libaio-devel.])
  ])
])

AC_DEFUN([PANDORA_REQUIRE_LIBAIO],[
  AC_REQUIRE([_PANDORA_REQUIRE_LIBAIO])
])
