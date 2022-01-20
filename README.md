libeatmydata
============

An LD_PRELOAD library that disables all forms of writing data safely to disk.
fsync() becomes a NO-OP, O_SYNC is removed etc.

The idea is to use in testing to get faster test runs where real durability is
not required.

***DO NOT*** use libeatmydata on software where you care about what it stores. It's called lib***EAT-MY-DATA*** for a *reason*.

see http://www.flamingspork.com/projects/libeatmydata

Installing
------------

Debian/Ubuntu and friends:
```
apt install eatmydata
```

To build from source, you will need `autoconf`, `autoconf-archive`, `automake`, `libtool`, and a compiler (say, `gcc`).

Then, build and install like so:
```
git clone https://github.com/stewartsmith/libeatmydata.git # (or extract from source tarball)
cd libeatmydata
autoreconf -i # (if building from git only)
./configure
make
make check
sudo make install
```


Usage
-----

```
eatmydata foo
```

Performance Improvements
------------------------

When running part of the MySQL test suite in 2007 on my laptop:

```
TEST                           RESULT         TIME (ms)   TIME (with libeatmydata)
----------------------------------------------------------------------------------

main.innodb-lock               [ pass ]           4523     4323
main.innodb-replace            [ pass ]            102       56
main.innodb-ucs2               [ pass ]           5786     1084
main.innodb                    [ pass ]          78306    24900
main.innodb_gis                [ pass ]           2647     1544
main.innodb_mysql              [ pass ]          86810    68579
main.innodb_notembedded        [ pass ]            198      150
main.innodb_timeout_rollback   [ pass ]           2990     2750
main.innodb_trx_weight         [ pass ]           1608      841
---------------------------------------------------------------
Stopping All Servers
All 9 tests were successful.
The servers were restarted 7 times

WITHOUT: Spent 182.97 seconds actually executing testcases
WITH   : Spent 104.227 seconds actually executing testcases

WITHOUT:               WITH:
real    3m36.053s      real    2m10.610s
user    0m42.323s      user    0m41.939s
sys     0m2.844s       sys     0m2.356s
```


Talks/Video
-----------

libeatmydata was the product of a talk I gave back at linux.conf.au 2007, titled
"Eat My Data: How Everybody gets File IO Wrong" - I also gave this talk at OSCON.
This talk went over the common mistakes people make when using POSIX file IO
routines.

You can watch a video recording of this talk at:

- https://youtu.be/LMe7hf2G1po
- http://www.linux.org.au/conf/2007/talk/278.html

Copyright
---------

(C)2007-2022 Stewart Smith
and other contributors (see AUTHORS)
See LICENSE for full text of GPLv3
