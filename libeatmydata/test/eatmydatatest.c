/*
    Copyright (C): 2010, Modestas Vainius <modax@debian.org>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>

int failed_tests = 0;

#define DATAFILENAME "eatmydatatest.dat"
#define TESTCASE(value, expected, desc) \
  { errno=0; if (value != expected) {					\
        fprintf(stderr, "Test case failed: %s (err=%d).\n", desc, value); \
        if (errno != 0) perror("-- System errno (if applicable)"); \
        failed_tests++; \
      } \
    }

// Returns true if file status flag was not filtered out.
static int test_open_flag(int flag)
{
    int fd, res;

    res = -1;
    fd = open(DATAFILENAME, O_CREAT|O_TRUNC|O_WRONLY|flag, S_IRUSR|S_IWUSR);
    if (fd != -1) {
        res = fcntl(fd, F_GETFL);
    }
    if (res == -1) {
        res = errno;
        close(fd);
        unlink(DATAFILENAME);
        errno = res;
        return -1;
    }
    close(fd);
    unlink(DATAFILENAME);
    return res & flag;
}

int main(int argc, char** argv)
{
    int fd;
    (void)argc;
    (void)argv;

    TESTCASE(test_open_flag(O_SYNC), 0, "open(O_SYNC) flag was not filtered out");
    TESTCASE(test_open_flag(O_DSYNC), 0, "open(O_DSYNC) flag was not filtered out");

    // Open and close FD in order to make sure its invalid.
    fd = open(DATAFILENAME, O_CREAT|O_TRUNC|O_WRONLY, S_IRUSR|S_IWUSR);
    if (fd == -1)
        perror("unable to open " DATAFILENAME);

    // Now test fsync and friends by feeding them an invalid file descriptor.
    // System implementation should fail with EBADF while libeatmydata
    // implementation will succeed.
    TESTCASE(fsync(fd), 0, "system fsync() was not overridden");
    TESTCASE(fdatasync(fd), 0, "system fdatasync() was not overridden");
    TESTCASE(msync(0, 1, 1), 0, "system msync() was not overridden");

    if (failed_tests > 0) {
        fprintf(stderr, "%d test cases failed\n", failed_tests);
        return 2;
    }
    if (close(fd) == -1)
        perror("unable to close " DATAFILENAME " file descriptor");
    unlink(DATAFILENAME);

    return 0;
}
