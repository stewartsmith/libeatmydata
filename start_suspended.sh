#!/bin/bash
#
# Libeatmydata
#
# Copyright (C) 2017 Stewart Smith (stewart@flamingspork.com)
# All rights reserved.
#
# Use and distribution licensed under the BSD license. See the
# COPYING file in the root project directory for full text.
#

# A helper script to start a process in suspended state with macOS dynamic
# library preloaded, neccessitated by bash version < 4 there which does not
# have $BASHPID enabling an inline solution
# (kill -s STOP $BASPID; exec foo arg) &

kill -TSTP $$

export DYLD_LIBRARY_PATH=libeatmydata/.libs
export DYLD_FORCE_FLAT_NAMESPACE=1
export DYLD_INSERT_LIBRARIES=./.libs/libeatmydata.dylib

exec $@
exit $?
