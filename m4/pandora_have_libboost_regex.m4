dnl Copyright (C) 2010 Andrew Hutchings
dnl This file is free software; Andrew Hutchings
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.

AC_DEFUN([_PANDORA_SEARCH_BOOST_REGEX],[
  AC_REQUIRE([AC_LIB_PREFIX])
  AC_REQUIRE([ACX_PTHREAD])

  dnl --------------------------------------------------------------------
  dnl  Check for boost::regex
  dnl --------------------------------------------------------------------

  save_CFLAGS="${CFLAGS}"
  save_CXXFLAGS="${CXXFLAGS}"
  CFLAGS="${PTHREAD_CFLAGS} ${CFLAGS}"
  CXXFLAGS="${PTHREAD_CFLAGS} ${CXXFLAGS}"

  AC_LANG_PUSH(C++)
  AC_LIB_HAVE_LINKFLAGS(boost_regex-mt,,[
    #include <boost/regex.hpp>
  ],[
    boost::regex test_regex("drizzle");
  ])
  AS_IF([test "x${ac_cv_libboost_regex_mt}" = "xno"],[
    AC_LIB_HAVE_LINKFLAGS(boost_regex,,[
      #include <boost/regex.hpp>
    ],[
      boost::regex test_regex("drizzle");
    ])
  ])
  AC_LANG_POP()
  CFLAGS="${save_CFLAGS}"
  CXXFLAGS="${save_CXXFLAGS}"

  
  AM_CONDITIONAL(HAVE_BOOST_REGEX,
    [test "x${ac_cv_libboost_regex}" = "xyes" -o "x${ac_cv_libboost_regex_mt}" = "xyes"])
  BOOST_LIBS="${BOOST_LIBS} ${LTLIBBOOST_REGEX_MT} ${LTLIBBOOST_REGEX}"
  AC_SUBST(BOOST_LIBS) 
])

AC_DEFUN([PANDORA_HAVE_BOOST_REGEX],[
  PANDORA_HAVE_BOOST($1)
  _PANDORA_SEARCH_BOOST_REGEX($1)
])

AC_DEFUN([PANDORA_REQUIRE_BOOST_REGEX],[
  PANDORA_REQUIRE_BOOST($1)
  _PANDORA_SEARCH_BOOST_REGEX($1)
  AS_IF([test "x${ac_cv_libboost_regex}" = "xno" -a "x${ac_cv_libboost_regex_mt}" = "xno"],
      AC_MSG_ERROR([boost::regex is required for ${PACKAGE}]))
])

