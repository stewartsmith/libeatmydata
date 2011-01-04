libs = libeatmydata.so
CC ?= gcc

all: $(libs)

clean:
	rm -f libeatmydata.so* *.o fsynctest eatmydatatest

dist_files :=\
	eatmydata.c\
	fsynctest.c\
	fsynctest.result\
	LICENSE\
	Makefile\
	Makefile.solaris\
	README

dist:
	tarname=libeatmydata-`bzr log -r-1|sed 's/^revno: //p;d'`;\
	rm -rf $$tarname $$tarname.tar.bz2;\
	mkdir $$tarname &&\
	cp $(dist_files) $$tarname &&\
	tar cjf $$tarname.tar.bz2 $$tarname &&\
	rm -rf $$tarname

distclean: clean
	rm -f config.log config.status config.h

eatmydata.o: eatmydata.c
	$(CC) -c $(CFLAGS) -fPIC -o $@ $<

libeatmydata.so: eatmydata.o
	$(CC) -shared -Wl,-soname,$@ $(LDFLAGS) -o $@ $< -ldl

fsynctest: fsynctest.c
	$(CC) $(FFLAGS) $(LDFLAGS) -o fsynctest fsynctest.c

eatmydatatest: eatmydatatest.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $@.c

test: runeatmydatatest
	@echo "Tests succeeded"

testpat := '^[a-z]*sync\|O_SYNC'
runfsynctest: fsynctest $(libs)
	LD_PRELOAD=./libeatmydata.so strace -o fsynctest.result.run ./fsynctest
	! grep $(testpat) fsynctest.result.run
	rm fsynctest.result.run
	@echo
	@echo "Good: no *sync calls"
	@#grep $(testpat) fsynctest.result.run |sed -e 's%[0-9]* vars%%' > fsynctest.result.run.out
	@#mv fsynctest.result.run.out fsynctest.result.run
	@#diff fsynctest.result fsynctest.result.run

runeatmydatatest: eatmydatatest $(libs)
	LD_PRELOAD=./libeatmydata.so ./eatmydatatest
