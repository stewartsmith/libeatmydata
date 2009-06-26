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

/* 
#define CHECK_FILE "/tmp/eatmydata"
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
static int (*libc_fsync)(int)= NULL;
static int (*libc_sync)()= NULL;
static int (*libc_fdatasync)(int)= NULL;
static int (*libc_msync)(void*, size_t, int)= NULL;

#define ASSIGN_DLSYM_OR_DIE(name)			\
        libc_##name = dlsym(RTLD_NEXT, #name);			\
        if (!libc_##name || dlerror())				\
                _exit(1);
    
void __attribute__ ((constructor)) eatmydata_init(void)
{
        ASSIGN_DLSYM_OR_DIE(open);
	ASSIGN_DLSYM_OR_DIE(fsync);
	ASSIGN_DLSYM_OR_DIE(sync);
	ASSIGN_DLSYM_OR_DIE(fdatasync);
	ASSIGN_DLSYM_OR_DIE(msync);
}

int eatmydata_is_hungry(void)
{
	/* Init here, as it is called before any libc functions */
	if(!libc_open)
		eatmydata_init();

#ifdef CHECK_FILE
	static struct stat buf;
	int old_errno, stat_ret;

	old_errno= errno;
	stat_ret= stat(CHECK_FILE, &buf);
	errno= old_errno;

	/* Treat any error as if file doesn't exist, for safety */
	return !stat_ret;
#else
	/* Always hungry! */
	return 1;
#endif
}

int fsync(int fd)
{
	if (eatmydata_is_hungry()) {
		errno= 0;
		return 0;
	}

	return (*libc_fsync)(fd);
}

/* no errors are defined for this function */
void sync(void)
{
	if (eatmydata_is_hungry())
		return;

	(*libc_sync)();
}

int open(const char* pathname, int flags, ...)
{
	va_list ap;
	mode_t mode;

	va_start(ap, flags);
	mode= va_arg(ap, mode_t);
	va_end(ap);

	if (eatmydata_is_hungry())
		flags &= ~(O_SYNC|O_DSYNC);

	return (*libc_open)(pathname,flags,mode);
}

int fdatasync(int fd)
{
	if (eatmydata_is_hungry()) {
		errno= 0;
		return 0;
	}

	return (*libc_fdatasync)(fd);
}

int msync(void *addr, size_t length, int flags)
{
	if (eatmydata_is_hungry()) {
		errno= 0;
		return 0;
	}

	return (*libc_msync)(addr, length, flags);
}
