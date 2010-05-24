#!/bin/sh
#
# sentence_name
#
# Copyright (C) 2010 Eric Day (eday@oddments.org)
# All rights reserved.
#
# Use and distribution licensed under the BSD license. See the
# COPYING file in the root project directory for full text.
#

# Get filename we want to run without path
name=`echo $1 | sed 's/.*\/\(project_name\/.*[^\/]*\)$/\1/'`

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
if [ -n "$all_caps_name_TEST_PREFIX" ]
then
  if [ -n "$all_caps_name_TEST_FILTER" ]
  then
    # If filter variable is set, only apply prefix to those that match
    for x in $all_caps_name_TEST_FILTER
    do
      if [ "x$x" = "x$name" ]
      then
        prefix="$libtool_prefix $all_caps_name_TEST_PREFIX"
        with=" (with prefix after filter)"
        break
      fi
    done
  else
    prefix="$libtool_prefix $all_caps_name_TEST_PREFIX"
    with=" (with prefix)"
  fi
fi

# Set this to fix broken libtool test
ECHO=`which echo`
export ECHO

# This needs to be set because of broken libtool on OSX
DYLD_LIBRARY_PATH=project_name/.libs
export DYLD_LIBRARY_PATH

$prefix $1 $all_caps_name_TEST_ARGS
