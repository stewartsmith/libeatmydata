dnl  Copyright (C) 2009 Sun Microsystems, Inc.
dnl This file is free software; Sun Microsystems, Inc.
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.

AC_DEFUN([PANDORA_OPTIMIZE],[
  dnl Build optimized or debug version ?
  dnl First check for gcc and g++
  AS_IF([test "$GCC" = "yes" -a "$INTELCC" = "no"],[

    dnl The following is required for portable results of floating point 
    dnl calculations on PowerPC. The same must also be done for IA-64, but 
    dnl this options is missing in the IA-64 gcc backend.
    case "$target_cpu" in
      *ppc* | *powerpc*)
        AM_CFLAGS="-mno-fused-madd ${AM_CFLAGS}"
      ;;
    esac

    dnl Once we can use a modern autoconf, we can replace the std=gnu99 here
    dnl with using AC_CC_STD_C99 above
    CC="${CC} -std=gnu99"

    AM_CPPFLAGS="-g ${AM_CPPFLAGS}"

    DEBUG_CFLAGS="-O0"

    OPTIMIZE_CFLAGS="-O2"
  ])
  AS_IF([test "$INTELCC" = "yes"],[
    AM_CPPFLAGS="-g ${AM_CPPFLAGS}"

    DEBUG_CFLAGS="-O0"

    OPTIMIZE_CFLAGS="-xHOST -O2 -no-prec-div -static"

  ])
  AS_IF([test "$SUNCC" = "yes"],[
    dnl Once we can use a modern autoconf, we can replace the -xc99=all here
    dnl with using AC_CC_STD_C99 above
    CC="${CC} -xc99=all"

    AM_CFLAGS="-g -mt -xstrconst -Xa ${AM_CFLAGS}"


    dnl TODO: Make a test for -xO4 usability here
    OPTIMIZE_FLAGS="-xO3 -xlibmil -xdepend -xbuiltin"
    OPTIMIZE_CFLAGS="${OPTIMIZE_FLAGS}"

  ])

  AC_ARG_WITH([debug],
    [AS_HELP_STRING([--with-debug],
       [Add debug code/turns off optimizations (yes|no) @<:@default=no@:>@])],
    [with_debug=$withval],
    [with_debug=no])
  AS_IF([test "$with_debug" = "yes"],[
    # Debugging. No optimization.
    AM_CFLAGS="${AM_CFLAGS} ${DEBUG_CFLAGS} -DDEBUG"
  ],[
    # Optimized version. No debug
    AM_CFLAGS="${AM_CFLAGS} ${OPTIMIZE_CFLAGS}"
  ])
])
