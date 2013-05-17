#!/bin/bash
#
# Libeatmydata
#
# Copyright (C) 2010 Eric Day (eday@oddments.org)
# All rights reserved.
#
# Use and distribution licensed under the BSD license. See the
# COPYING file in the root project directory for full text.
#

# Get filename we want to run without path
name=`echo $1 | sed 's/.*\/\(libeatmydata\/.*[^\/]*\)$/\1/'`

ext=`echo $name | sed 's/.*\.\([^.]*$\)/\1/'`
if [ "x$ext" = "x$name" ]
then
  ext=""
fi

if [ ! "x$ext" = "xsh" ]
then
  libtool_prefix="libtool --mode=execute"
fi

# Set prefix if it was given through environment
if [ -n "$LIBEATMYDATA_TEST_PREFIX" ]
then
  if [ -n "$LIBEATMYDATA_TEST_FILTER" ]
  then
    # If filter variable is set, only apply prefix to those that match
    for x in $LIBEATMYDATA_TEST_FILTER
    do
      if [ "x$x" = "x$name" ]
      then
        prefix="$libtool_prefix $LIBEATMYDATA_TEST_PREFIX"
        with=" (with prefix after filter)"
        break
      fi
    done
  else
    prefix="$libtool_prefix $LIBEATMYDATA_TEST_PREFIX"
    with=" (with prefix)"
  fi
fi

# Set this to fix broken libtool test
ECHO=`which echo`
export ECHO

# This needs to be set because of broken libtool on OSX
DYLD_LIBRARY_PATH=libeatmydata/.libs
export DYLD_LIBRARY_PATH

LD_PRELOAD=./.libs/libeatmydata.so DYLD_FORCE_FLAT_NAMESPACE=1 DYLD_INSERT_LIBRARIES=./.libs/libeatmydata.dylib strace -o test.result.run $1 $LIBEATMYDATA_TEST_ARGS
grep '^[a-z]*sync\|O_SYNC' test.result.run
ret=$?
rm test.result.run
if [ $ret == 1 ]; then
	exit 0;
else
	exit $ret;
fi

