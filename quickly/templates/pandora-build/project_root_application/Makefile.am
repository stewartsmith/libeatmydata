#
# sentence_name
#
### BEGIN LICENSE
### END LICENSE

ACLOCAL_AMFLAGS = -I m4
LDADD = libproject_name.la
AM_DEFAULT_SOURCE_EXT = .cc


bin_PROGRAMS =
check_PROGRAMS =
noinst_PROGRAMS =
lib_LTLIBRARIES =
noinst_LTLIBRARIES =
nobase_include_HEADERS =
noinst_HEADERS =
pkgplugin_LTLIBRARIES =
CLEANFILES =
EXTRA_DIST = \
  docs/Doxyfile \
  docs/header.html.in \
  test_run.sh


#
# libproject_name
#

lib_LTLIBRARIES += libproject_name.la

libproject_name_la_SOURCES = \
  project_name/class_name.cc

nobase_include_HEADERS += \
  project_name/class_name.h \
  project_name/visibility.h

libproject_name_la_CXXFLAGS = \
	$(AM_CXXFLAGS) \
	-DBUILDING_all_caps_name

libproject_name_la_LIBADD = $(LIBDL_LIBS)
libproject_name_la_LDFLAGS = $(AM_LDFLAGS) -version-info $(LIBall_caps_name_VERSION)

check_PROGRAMS += \
  project_name/test/basic

#
# Simple project_name application
#

bin_PROGRAMS += bin/project_name
bin_project_name_SOURCES = bin/project_name.cc

include config/pandora-plugin.am

TESTS = $(check_PROGRAMS)

TESTS_ENVIRONMENT = ${top_srcdir}/test_run.sh

check-verbose:
	@all_caps_name_TEST_ARGS="v" make check

check-debug:
	@all_caps_name_TEST_ARGS="vvv" make check

check-valgrind:
	@all_caps_name_TEST_PREFIX="valgrind -q --leak-check=full --show-reachable=yes --suppressions=valgrind.suppressions" make check

check-valgrind-debug:
	@all_caps_name_TEST_PREFIX="valgrind -q --leak-check=full --show-reachable=yes --suppressions=valgrind.suppressions" all_caps_name_TEST_ARGS="vvv" make check

check-gdb:
	@all_caps_name_TEST_PREFIX="gdb -q" make check

check-time:
	@all_caps_name_TEST_PREFIX="/usr/bin/time" make check

check-strace:
	@all_caps_name_TEST_PREFIX="strace -c" make check

check-truss:
	@all_caps_name_TEST_PREFIX="truss -c" make check

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
	cd project_name && lcov --directory . --base-directory .. --capture --output-file lcov.out
	cd project_name && lcov --directory . --base-directory .. --extract lcov.out `pwd`/\* --output-file lcov_extract.out
	genhtml -o lcov -t project_name project_name/lcov_extract.out

lcov-clean: clean
	find . -name '*.gcno' -exec rm {} \;
	find . -name '*.gcda' -exec rm {} \;
	find . -name 'lcov*.out' -exec rm {} \;
