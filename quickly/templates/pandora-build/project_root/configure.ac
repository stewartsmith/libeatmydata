#
# sentence_name
#
# Copyright (C) 2009-2010 Monty Taylor, Eric Day
# All rights reserved.
#
# Use and distribution licensed under the BSD license.  See
# the COPYING file in this directory for full text.
#

AC_INIT([project_name],[0.1],[https://bugs.launchpad.net/project_name])
AC_CONFIG_SRCDIR([project_name/class_name.h])
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_MACRO_DIR([m4])

PANDORA_CANONICAL_TARGET(require-cxx, warnings-always-on)

# Increment if interfaces have been added, removed, or changed.
LIBall_caps_name_VERSION_CURRENT=0

# Increment if source code has changed, set to zero if current is incremented.
LIBall_caps_name_VERSION_REVISION=0

# Increment if interfaces have been added, set to zero if interfaces have been
# removed or changed.
LIBall_caps_name_VERSION_AGE=0

AC_SUBST(LIBall_caps_name_VERSION_CURRENT)
AC_SUBST(LIBall_caps_name_VERSION_REVISION)
AC_SUBST(LIBall_caps_name_VERSION_AGE)

AC_DEFINE_UNQUOTED([LIBall_caps_name_VERSION_CURRENT],
                   [$LIBall_caps_name_VERSION_CURRENT],
                   [Current portion of libproject_name version])
AC_DEFINE_UNQUOTED([LIBall_caps_name_VERSION_REVISION],
                   [$LIBall_caps_name_VERSION_REVISION],
                   [Revision portion of libproject_name version])
AC_DEFINE_UNQUOTED([LIBall_caps_name_VERSION_AGE],
                   [$LIBall_caps_name_VERSION_AGE],
                   [Age portion of libproject_name version])

LIBall_caps_name_VERSION=$LIBall_caps_name_VERSION_CURRENT:$LIBall_caps_name_VERSION_REVISION:$LIBall_caps_name_VERSION_AGE

AC_SUBST(LIBall_caps_name_VERSION)

AC_LANG_PUSH(C++)
PANDORA_REQUIRE_PTHREAD
PANDORA_REQUIRE_LIBDL
AC_LANG_POP

PANDORA_PLUGINS

AC_DEFINE_UNQUOTED([all_caps_name_MODULE_EXT], ["$acl_cv_shlibext"],
                   [Extension to use for modules.])

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
echo "   * Configured modules:"
echo ""

for module in `echo $PANDORA_PLUGIN_LIST | sed 's/_/::/g' | tr ',' '\n' | sort`
do
  echo "     - $module"
done

echo ""
echo "---"
