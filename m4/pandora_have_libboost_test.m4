dnl Copyright (C) 2011 Andrew Hutchings
dnl This file is free software; Andrew Hutchings
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.

AC_DEFUN([_PANDORA_SEARCH_BOOST_TEST],[
  AC_REQUIRE([AC_LIB_PREFIX])

  dnl --------------------------------------------------------------------
  dnl  Check for boost::test
  dnl --------------------------------------------------------------------

  AC_LANG_PUSH(C++)
  AC_LIB_HAVE_LINKFLAGS(boost_unit_test_framework-mt,,[
    #include <boost/test/unit_test.hpp>
  ],[
    boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_messages);
  ])
  AS_IF([test "x${ac_cv_libboost_unit_test_framework_mt}" = "xno"],[
    AC_LIB_HAVE_LINKFLAGS(boost_unit_test_framework,,[
      #include <boost/test/unit_test.hpp>
    ],[
      boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_messages);
    ])
  ])
  AC_LANG_POP()
  
  AM_CONDITIONAL(HAVE_BOOST_TEST,
    [test "x${ac_cv_libboost_unit_test_framework}" = "xyes" -o "x${ac_cv_libboost_unit_test_framework_mt}" = "xyes"])
  BOOST_LIBS="${BOOST_LIBS} ${LTLIBBOOST_TEST_MT} ${LTLIBBOOST_TEST}"
  AC_SUBST(BOOST_LIBS) 
])

AC_DEFUN([PANDORA_HAVE_BOOST_TEST],[
  PANDORA_HAVE_BOOST($1)
  _PANDORA_SEARCH_BOOST_TEST($1)
])

AC_DEFUN([PANDORA_REQUIRE_BOOST_TEST],[
  PANDORA_REQUIRE_BOOST($1)
  _PANDORA_SEARCH_BOOST_TEST($1)
  AS_IF([test "x${ac_cv_libboost_unit_test_framework}" = "xno" -a "x${ac_cv_libboost_unit_test_framework_mt}" = "xno"],
      PANDORA_MSG_ERROR([boost::test is required for ${PACKAGE}]))
])

