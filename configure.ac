#
# Libeatmydata
#
# Copyright (C) 2009-2010 Monty Taylor, Eric Day
# All rights reserved.
#
# Use and distribution licensed under the BSD license.  See
# the COPYING file in this directory for full text.
#


AC_INIT([libeatmydata],[56], [http://www.flamingspork.com/projects/libeatmydata])
AC_CONFIG_SRCDIR([libeatmydata/libeatmydata.c])
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_MACRO_DIR([m4])

PANDORA_CANONICAL_TARGET(less-warnings)

# Increment if interfaces have been added, removed, or changed.
LIBEATMYDATA_VERSION_CURRENT=2

# Increment if source code has changed, set to zero if current is incremented.
LIBEATMYDATA_VERSION_REVISION=0

# Increment if interfaces have been added, set to zero if interfaces have been
# removed or changed.
LIBEATMYDATA_VERSION_AGE=1

AC_SUBST(LIBEATMYDATA_VERSION_CURRENT)
AC_SUBST(LIBEATMYDATA_VERSION_REVISION)
AC_SUBST(LIBEATMYDATA_VERSION_AGE)

AC_DEFINE_UNQUOTED([LIBEATMYDATA_VERSION_CURRENT],
                   [$LIBEATMYDATA_VERSION_CURRENT],
                   [Current portion of liblibeatmydata version])
AC_DEFINE_UNQUOTED([LIBEATMYDATA_VERSION_REVISION],
                   [$LIBEATMYDATA_VERSION_REVISION],
                   [Revision portion of liblibeatmydata version])
AC_DEFINE_UNQUOTED([LIBEATMYDATA_VERSION_AGE],
                   [$LIBEATMYDATA_VERSION_AGE],
                   [Age portion of liblibeatmydata version])

LIBEATMYDATA_VERSION=$LIBEATMYDATA_VERSION_CURRENT:$LIBEATMYDATA_VERSION_REVISION:$LIBEATMYDATA_VERSION_AGE

AC_SUBST(LIBEATMYDATA_VERSION)

PANDORA_REQUIRE_LIBDL
AC_CHECK_SIZEOF(mode_t)
AC_CHECK_SIZEOF(int)

AC_CONFIG_FILES(Makefile docs/header.html)

AC_OUTPUT

echo "---"
echo "Configuration summary for $PACKAGE_NAME version $VERSION"
echo ""
echo "   * Installation prefix:       $prefix"
echo "   * System type:               $host_vendor-$host_os"
echo "   * Host CPU:                  $host_cpu"
echo "   * C Compiler:                $CC_VERSION"
echo "   * Assertions enabled:        $ac_cv_assert"
echo "   * Debug enabled:             $with_debug"
echo "   * Profiling enabled:         $ac_profiling"
echo "   * Coverage enabled:          $ac_coverage"
echo "   * Warnings as errors:        $ac_cv_warnings_as_errors"
echo ""
