#
# Libeatmydata
#
### BEGIN LICENSE
# Copyright (C) 2008-2010 Stewart Smith <stewart@flamingspork.com>
# This program is free software: you can redistribute it and/or modify it 
# under the terms of the GNU General Public License version 3, as published 
# by the Free Software Foundation.
# 
# This program is distributed in the hope that it will be useful, but 
# WITHOUT ANY WARRANTY; without even the implied warranties of 
# MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR 
# PURPOSE.  See the GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License along 
# with this program.  If not, see <http://www.gnu.org/licenses/>.
### END LICENSE

ACLOCAL_AMFLAGS = -I m4
#LDADD = libeatmydata.la

AUTOMAKE_OPTIONS = parallel-tests

bin_PROGRAMS =
check_PROGRAMS =
noinst_PROGRAMS =
lib_LTLIBRARIES =
noinst_LTLIBRARIES =
nobase_include_HEADERS =
noinst_HEADERS =
CLEANFILES = $(bin_SCRIPTS) $(libexec_SCRIPTS)
DISTCLEANFILES = config/top.h
EXTRA_DIST = \
  fsynctest.result \
  docs/Doxyfile \
  docs/header.html.in \
  test_run.sh \
  eatmydata.in \
  eatmydata.sh.in \
  debian/changelog \
  debian/compat \
  debian/control \
  debian/copyright \
  debian/docs \
  debian/README.Debian \
  debian/rules \
  debian/source \
  build_emd_rpm.sh \
  libeatmydata.spec


bin_SCRIPTS = eatmydata
libexec_SCRIPTS = eatmydata.sh

do_subst = @SED@ \
	-e 's!@''libdir''@!$(libdir)!g' \
	-e 's!@''libexecdir''@!$(libexecdir)!g'

eatmydata: eatmydata.in Makefile
	$(do_subst) $(srcdir)/eatmydata.in > $@
	chmod +x eatmydata

eatmydata.sh: eatmydata.sh.in Makefile
	$(do_subst) $(srcdir)/eatmydata.sh.in > $@

#
# libeatmydata
#

lib_LTLIBRARIES += libeatmydata.la

libeatmydata_la_SOURCES = \
  libeatmydata/libeatmydata.c

noinst_HEADERS += \
  libeatmydata/visibility.h

libeatmydata_la_CFLAGS = \
	$(AM_CFLAGS) \
  $(NO_WERROR) \
	-DBUILDING_LIBEATMYDATA

libeatmydata_la_LIBADD = $(LIBDL_LIBS) -lpthread
libeatmydata_la_LDFLAGS = $(AM_LDFLAGS) -avoid-version

#install-exec-hook:
#	find $(DESTDIR)$(libdir) -type f -name \*.la |xargs rm -f

check_PROGRAMS += \
  libeatmydata/test/fsynctest \
  libeatmydata/test/tst-cancel4 \
  libeatmydata/test/tst-key4 \
  libeatmydata/test/eatmydatatest \
  libeatmydata/test/eatmydatatest_largefile

libeatmydata_test_tst_cancel4_LDADD= -lpthread

libeatmydata_test_tst_key4_LDADD= -lpthread

#
# Simple libeatmydata application
#

TESTS = $(check_PROGRAMS)

LOG_COMPILER = ${top_srcdir}/test_run.sh

check-verbose:
	@LIBEATMYDATA_TEST_ARGS="v" make check

check-debug:
	@LIBEATMYDATA_TEST_ARGS="vvv" make check

check-valgrind:
	@LIBEATMYDATA_TEST_PREFIX="valgrind -q --leak-check=full --show-reachable=yes --suppressions=valgrind.suppressions" make check

check-valgrind-debug:
	@LIBEATMYDATA_TEST_PREFIX="valgrind -q --leak-check=full --show-reachable=yes --suppressions=valgrind.suppressions" LIBEATMYDATA_TEST_ARGS="vvv" make check

check-gdb:
	@LIBEATMYDATA_TEST_PREFIX="gdb -q" make check

check-time:
	@LIBEATMYDATA_TEST_PREFIX="/usr/bin/time" make check

check-strace:
	@LIBEATMYDATA_TEST_PREFIX="strace -c" make check

check-truss:
	@LIBEATMYDATA_TEST_PREFIX="truss -c" make check

docs: all
	@echo "/**" > docs/mainpage.h
	@echo "@mainpage README" >> docs/mainpage.h
	@echo "<pre>" >> docs/mainpage.h
	@cat ${top_srcdir}/README >> docs/mainpage.h
	@echo "</pre>" >> docs/mainpage.h
	@echo "*/" >> docs/mainpage.h
	${DOXYGEN} ${top_srcdir}/docs/Doxyfile
	@rm docs/mainpage.h

lcov: lcov-clean check
	@echo
	@echo "------------------------------------------------------"
	@echo "Make sure ./configure was run with '--enable-coverage'"
	@echo "------------------------------------------------------"
	@echo
	cd libeatmydata && lcov --directory . --base-directory .. --capture --output-file lcov.out
	cd libeatmydata && lcov --directory . --base-directory .. --extract lcov.out `pwd`/\* --output-file lcov_extract.out
	genhtml -o lcov -t libeatmydata libeatmydata/lcov_extract.out

lcov-clean: clean
	find . -name '*.gcno' -exec rm {} \;
	find . -name '*.gcda' -exec rm {} \;
	find . -name 'lcov*.out' -exec rm {} \;
