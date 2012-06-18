dnl Copyright (C) 2010 Monty Taylor
dnl This file is free software;  Monty Taylor
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.

AC_DEFUN([_PANDORA_SEARCH_LIBCURL],[
  AC_REQUIRE([AC_LIB_PREFIX])

  dnl --------------------------------------------------------------------
  dnl  Check for libcurl
  dnl --------------------------------------------------------------------

  AC_ARG_ENABLE([libcurl],
    [AS_HELP_STRING([--disable-libcurl],
      [Build with libcurl support @<:@default=on@:>@])],
    [ac_enable_libcurl="$enableval"],
    [ac_enable_libcurl="yes"])

  AS_IF([test "x$ac_enable_libcurl" = "xyes"],[
    AC_LIB_HAVE_LINKFLAGS(curl,,
      [#include <curl/curl.h>],
      [
        CURL *handle;
        handle=curl_easy_init();
      ])
    ],[
      ac_cv_libcurl="no"
    ])

  AC_CACHE_CHECK([if libcurl has CURLOPT_USERNAME],
                 [pandora_cv_curl_have_username],[
     AC_COMPILE_IFELSE([
       AC_LANG_PROGRAM(
         [[
           CURL *handle;
           handle=curl_easy_init();
           rv= curl_easy_setopt(curl_handle, CURLOPT_USERNAME, "foo");
         ]])],
       [pandora_cv_curl_have_username=yes],
       [pandora_cv_curl_have_username=no])
     ])

  AM_CONDITIONAL(HAVE_LIBCURL,[test "x${ac_cv_libcurl}" = "xyes"])
  AS_IF([test "x$pandora_cv_curl_have_username" = "xyes"],
        AC_DEFINE([HAVE_CURLOPT_USERNAME],[1],
                  [Does libcurl provide the CURLOPT_USERNAME constant]))

])

AC_DEFUN([PANDORA_HAVE_LIBCURL],[
  AC_REQUIRE([_PANDORA_SEARCH_LIBCURL])
  AS_IF([test "x${ac_cv_libcurl}" = "xno"],[
    AC_MSG_WARN([libcurl development lib not found. On Debian this is found in libcurl4-gnutls-dev. On RHEL5/Fedora11 it's in curl-devel. On RHEL6/Fedora12 it's in libcurl-devel.])
  ])
])

AC_DEFUN([PANDORA_REQUIRE_LIBCURL],[
  PANDORA_HAVE_LIBCURL($1)
  AS_IF([test "x${ac_cv_libcurl}" = "xno"],[
    AC_MSG_ERROR([libcurl is required for ${PACKAGE}])
  ])
])
