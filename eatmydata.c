/*
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

#ifndef RTLD_NEXT
#  define _GNU_SOURCE
#endif

#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <stdarg.h>

int errno;

static int (*libc_open)(const char*, int, ...)= NULL;

void __attribute__ ((constructor)) eatmydata_init(void)
{
        libc_open = dlsym(RTLD_NEXT, "open");
        if (!libc_open || dlerror())
                _exit(1);
}

int fsync(int fd)
{
	errno=0;
	return 0;
}

/* no errors are defined for this function */
void sync(void)
{
}

int open(const char* pathname, int flags, ...)
{
	va_list ap;
	mode_t mode;

	va_start(ap, flags);
	mode= va_arg(ap, mode_t);
	va_end(ap);

	flags &= ~(O_SYNC|O_DSYNC);

	if(!libc_open)
		eatmydata_init();

	return (*libc_open)(pathname,flags,mode);
}

int fdatasync(int fd)
{
	errno= 0;
	return 0;
}

int msync(void *addr, size_t length, int flags)
{
	errno = 0;
	return 0;
}
