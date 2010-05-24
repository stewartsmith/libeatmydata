ACLOCAL_AMFLAGS= -I m4 --force

lib_LTLIBRARIES = libeatmydata.la

libeatmydata_la_SOURCES = eatmydata.c
libeatmydata_la_CFLAGS = ${AM_CFLAGS} ${NO_WERROR}
libeatmydata_la_LDFLAGS = -version-info 1:0:0
libeatmydata_la_LIBADD = ${LIBDL_LIBS}

EXTRA_DIST = test_run.sh

check_PROGRAMS = fsynctest_eatmydata

fsynctest_eatmydata_LDADD = libeatmydata.la
fsynctest_eatmydata_SOURCES = fsynctest.c

TESTS = $(check_PROGRAMS)

TESTS_ENVIRONMENT = ${top_srcdir}/test_run.sh

test: check

runfsynctest:
	@#grep $(testpat) fsynctest.result.run |sed -e 's%[0-9]* vars%%' > fsynctest.result.run.out
	@#mv fsynctest.result.run.out fsynctest.result.run
	@#diff fsynctest.result fsynctest.result.run

