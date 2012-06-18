dnl Copyright (C) 2010 Andrew Hutchings
dnl This file is free software; Andrew Hutchings
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.

AC_DEFUN([_PANDORA_SEARCH_BOOST_IOSTREAMS],[
  AC_REQUIRE([AC_LIB_PREFIX])

  dnl --------------------------------------------------------------------
  dnl  Check for Boost.Iostreams
  dnl --------------------------------------------------------------------

  AC_LANG_PUSH(C++)
  AC_LIB_HAVE_LINKFLAGS(boost_iostreams-mt,,[
    #include <boost/iostreams/stream.hpp>
    #include <boost/iostreams/device/array.hpp>
  ],[
    const char* input= "hello world";
    boost::iostreams::stream<boost::iostreams::array_source> in(input, strlen(input));
  ])
  AS_IF([test "x${ac_cv_libboost_iostreams_mt}" = "xno"],[
    AC_LIB_HAVE_LINKFLAGS(boost_iostreams,,[
      #include <boost/iostreams/stream.hpp>
      #include <boost/iostreams/device/array.hpp>
    ],[
      const char* input= "hello world";
      boost::iostreams::stream<boost::iostreams::array_source> in(input, strlen(input));
    ])
  ])
  AC_LANG_POP()
  
  AM_CONDITIONAL(HAVE_BOOST_IOSTREAMS,
    [test "x${ac_cv_libboost_iostreams}" = "xyes" -o "x${ac_cv_libboost_iostreams_mt}" = "xyes"])
  BOOST_LIBS="${BOOST_LIBS} ${LTLIBBOOST_IOSTREAMS_MT} ${LTLIBBOOST_IOSTREAMS}"
  AC_SUBST(BOOST_LIBS) 
])

AC_DEFUN([PANDORA_HAVE_BOOST_IOSTREAMS],[
  PANDORA_HAVE_BOOST($1)
  _PANDORA_SEARCH_BOOST_IOSTREAMS($1)
])

AC_DEFUN([PANDORA_REQUIRE_BOOST_IOSTREAMS],[
  PANDORA_REQUIRE_BOOST($1)
  _PANDORA_SEARCH_BOOST_IOSTREAMS($1)
  AS_IF([test "x${ac_cv_libboost_iostreams}" = "xno" -a "x${ac_cv_libboost_iostreams_mt}" = "xno"],
      AC_MSG_ERROR([Boost.Iostreams is required for ${PACKAGE}]))
])

