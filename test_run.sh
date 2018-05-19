#!/bin/bash
#
# Libeatmydata
#
# Copyright (C) 2010-2014 Stewart Smith (stewart@flamingspork.com)
# Based heavily on work by:
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

if [[ $OSTYPE == *"darwin"* ]]; then
  ./start_suspended.sh "$1" "$LIBEATMYDATA_TEST_ARGS" &
  test_pid=$!
  # Wait for the test program launcher to become suspended
  while [[ "$(ps -o state= -p $test_pid)" != *"T+"* ]]; do
        sleep 1
  done
  dtruss -p $test_pid 2>> $name-test.result.run &
  dtruss_pid=$!
  kill -CONT $test_pid
  eret=wait $test_pid
  kill $dtruss_pid
  wait $dtruss_pid
else
  export LD_PRELOAD=./.libs/libeatmydata.so
  strace -o $name-test.result.run "$1" "$LIBEATMYDATA_TEST_ARGS"
  eret=$?
fi

if [ $eret != 0 ]; then
	exit 2;
fi

SYNC_IN_TRACE='^[a-z]*sync\|O_SYNC'
if [[ $OSTYPE == *"darwin"* ]]; then
    # 0x33 == F_FULLFSYNC
    SYNC_IN_TRACE="$SYNC_IN_TRACE|^fcntl\\\(.*, 0x33"
fi

grep "$SYNC_IN_TRACE" $name-test.result.run
ret=$?
rm $name-test.result.run
if [ $ret == 1 ]; then
	exit 0;
else
	exit 1;
fi

