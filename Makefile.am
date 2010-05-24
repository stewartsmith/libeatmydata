ACLOCAL_AMFLAGS= -I m4 --force

lib_LTLIBRARIES = libeatmydata.la

libeatmydata_la_SOURCES = eatmydata.c
libeatmydata_la_LDFLAGS = -version-info 1:0:0
libeatmydata_la_LIBADD = -ldl

EXTRA_DIST = LICENSE

SUBDIRS = .
noinst_PROGRAMS = fsynctest_eatmydata

fsynctest_eatmydata_LDADD = libeatmydata.la
fsynctest_eatmydata_SOURCES = fsynctest.c

test: runfsynctest

testpat := '^[a-z]*sync\|O_SYNC'
runfsynctest:
	strace -o fsynctest.result.run ./fsynctest_eatmydata
	! grep $(testpat) fsynctest.result.run
	rm fsynctest.result.run
	@echo
	@echo "Good: no *sync calls"
	@#grep $(testpat) fsynctest.result.run |sed -e 's%[0-9]* vars%%' > fsynctest.result.run.out
	@#mv fsynctest.result.run.out fsynctest.result.run
	@#diff fsynctest.result fsynctest.result.run

