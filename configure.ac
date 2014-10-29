#
# Libeatmydata
#
# Build infrastructure:
# Copyright (C) 2009-2010 Monty Taylor, Eric Day
# Copyright (C) 2010-2014 Stewart Smith
# All rights reserved.
#
# Use and distribution licensed under the BSD license.  See
# the COPYING file in this directory for full text.
#


AC_INIT([libeatmydata],[105], [http://www.flamingspork.com/projects/libeatmydata])
AC_CONFIG_SRCDIR([libeatmydata/libeatmydata.c])
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_MACRO_DIR([m4])

PANDORA_CANONICAL_TARGET(less-warnings)

PANDORA_REQUIRE_LIBDL
AC_CHECK_SIZEOF(mode_t)
AC_CHECK_SIZEOF(int)

AC_CHECK_DECLS(fdatasync)
AC_CHECK_FUNCS(fdatasync)
AC_CHECK_DECLS(sync_file_range)
AC_CHECK_FUNCS(sync_file_range)

AC_CONFIG_FILES(Makefile docs/header.html libeatmydata.spec)

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
