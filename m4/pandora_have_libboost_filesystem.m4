dnl Copyright (C) 2010 Monty Taylor
dnl This file is free software; Monty Taylor
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.

AC_DEFUN([_PANDORA_SEARCH_BOOST_FILESYSTEM],[
  AC_REQUIRE([AC_LIB_PREFIX])

  dnl --------------------------------------------------------------------
  dnl  Check for Boost.Filesystem
  dnl --------------------------------------------------------------------

  AC_LANG_PUSH(C++)
  AC_LIB_HAVE_LINKFLAGS(boost_filesystem-mt,boost_system-mt,[
    #include <boost/filesystem.hpp>
  ],[
    boost::filesystem::path my_path("some_dir/file.txt");
  ])
  AS_IF([test "x${ac_cv_libboost_filesystem_mt}" = "xno"],[
    AC_LIB_HAVE_LINKFLAGS(boost_filesystem,boost_system,[
      #include <boost/filesystem.hpp>
    ],[
      boost::filesystem::path my_path("some_dir/file.txt");
    ])
  ])
  AC_LANG_POP()
  
  AM_CONDITIONAL(HAVE_BOOST_FILESYSTEM,
    [test "x${ac_cv_libboost_filesystem}" = "xyes" -o "x${ac_cv_libboost_filesystem_mt}" = "xyes"])
  BOOST_LIBS="${BOOST_LIBS} ${LTLIBBOOST_FILESYSTEM_MT} ${LTLIBBOOST_FILESYSTEM}"
  AC_SUBST(BOOST_LIBS) 
])

AC_DEFUN([PANDORA_HAVE_BOOST_FILESYSTEM],[
  PANDORA_HAVE_BOOST($1)
  _PANDORA_SEARCH_BOOST_FILESYSTEM($1)
])

AC_DEFUN([PANDORA_REQUIRE_BOOST_FILESYSTEM],[
  PANDORA_REQUIRE_BOOST($1)
  _PANDORA_SEARCH_BOOST_FILESYSTEM($1)
  AS_IF([test "x${ac_cv_libboost_filesystem}" = "xno" -a "x${ac_cv_libboost_filesystem_mt}" = "xno"],
      AC_MSG_ERROR([Boost.Filesystem is required for ${PACKAGE}]))
])

