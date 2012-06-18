dnl Copyright (C) 2010 Monty Taylor
dnl This file is free software; Monty Taylor
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.

AC_DEFUN([_PANDORA_SEARCH_BOOST_DATE_TIME],[
  AC_REQUIRE([AC_LIB_PREFIX])
  AC_REQUIRE([ACX_PTHREAD])

  dnl --------------------------------------------------------------------
  dnl  Check for Boost.Date_Time
  dnl --------------------------------------------------------------------

  AC_LANG_PUSH(C++)
  AC_LIB_HAVE_LINKFLAGS(boost_date_time-mt,,[
    #include <boost/date_time.hpp>
  ],[
    boost::gregorian::date weekstart(2002,2,1);
  ])
  AS_IF([test "x${ac_cv_libboost_date_time_mt}" = "xno"],[
    AC_LIB_HAVE_LINKFLAGS(boost_date_time,,[
      #include <boost/date_time.hpp>
    ],[
      boost::gregorian::date weekstart(2002,2,1);
    ])
  ])
  AC_LANG_POP()
  
  AM_CONDITIONAL(HAVE_BOOST_DATE_TIME,
    [test "x${ac_cv_libboost_date_time}" = "xyes" -o "x${ac_cv_libboost_date_time_mt}" = "xyes"])
  BOOST_LIBS="${BOOST_LIBS} ${LTLIBBOOST_DATE_TIME_MT} ${LTLIBBOOST_DATE_TIME}"
  AC_SUBST(BOOST_LIBS) 
])

AC_DEFUN([PANDORA_HAVE_BOOST_DATE_TIME],[
  PANDORA_HAVE_BOOST($1)
  _PANDORA_SEARCH_BOOST_DATE_TIME($1)
])

AC_DEFUN([PANDORA_REQUIRE_BOOST_DATE_TIME],[
  PANDORA_REQUIRE_BOOST($1)
  _PANDORA_SEARCH_BOOST_DATE_TIME($1)
  AS_IF([test "x${ac_cv_libboost_date_time}" = "xno" -a "x${ac_cv_libboost_date_time_mt}" = "xno"],
      AC_MSG_ERROR([Boost.Date_Time is required for ${PACKAGE}]))
])

