AC_PREREQ(2.52)

AC_INIT([libeatmydata],[1.0], [http://www.flamingspork.com/projects/libeatmydata])
AC_CONFIG_SRCDIR([eatmydata.c])
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])

PANDORA_CANONICAL_TARGET

AC_CONFIG_FILES(Makefile)

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
echo "   * Warnings as failure:       $ac_cv_warnings_as_errors"
echo ""
echo "---"


