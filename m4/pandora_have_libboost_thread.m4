dnl Copyright (C) 2010 Monty Taylor
dnl This file is free software; Monty Taylor
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.

AC_DEFUN([_PANDORA_SEARCH_BOOST_THREAD],[
  AC_REQUIRE([AC_LIB_PREFIX])
  AC_REQUIRE([ACX_PTHREAD])

  dnl --------------------------------------------------------------------
  dnl  Check for boost::thread
  dnl --------------------------------------------------------------------

  save_CFLAGS="${CFLAGS}"
  save_CXXFLAGS="${CXXFLAGS}"
  CFLAGS="${PTHREAD_CFLAGS} ${CFLAGS}"
  CXXFLAGS="${PTHREAD_CFLAGS} ${CXXFLAGS}"

  AC_LANG_PUSH(C++)
  AC_LIB_HAVE_LINKFLAGS(boost_thread-mt,,[
    #include <boost/thread.hpp>
  ],[
    boost::thread id;
  ])
  AS_IF([test "x${ac_cv_libboost_thread_mt}" = "xno"],[
    AC_LIB_HAVE_LINKFLAGS(boost_thread,,[
      #include <boost/thread.hpp>
    ],[
      boost::thread id;
    ])
  ])
  AC_LANG_POP()
  CFLAGS="${save_CFLAGS}"
  CXXFLAGS="${save_CXXFLAGS}"

  
  AM_CONDITIONAL(HAVE_BOOST_THREAD,
    [test "x${ac_cv_libboost_thread}" = "xyes" -o "x${ac_cv_libboost_thread_mt}" = "xyes"])
  BOOST_LIBS="${BOOST_LIBS} ${LTLIBBOOST_THREAD_MT} ${LTLIBBOOST_THREAD}"
  AC_SUBST(BOOST_LIBS) 
])

AC_DEFUN([PANDORA_HAVE_BOOST_THREAD],[
  PANDORA_HAVE_BOOST($1)
  _PANDORA_SEARCH_BOOST_THREAD($1)
])

AC_DEFUN([PANDORA_REQUIRE_BOOST_THREAD],[
  PANDORA_REQUIRE_BOOST($1)
  _PANDORA_SEARCH_BOOST_THREAD($1)
  AS_IF([test "x${ac_cv_libboost_thread}" = "xno" -a "x${ac_cv_libboost_thread_mt}" = "xno"],
      AC_MSG_ERROR([boost::thread is required for ${PACKAGE}]))
])

