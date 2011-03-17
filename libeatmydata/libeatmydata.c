/* BEGIN LICENSE
 * Copyright (C) 2008-2010 Stewart Smith <stewart@flamingspork.com>
 * This program is free software: you can redistribute it and/or modify it 
 * under the terms of the GNU General Public License version 3, as published 
 * by the Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranties of 
 * MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR 
 * PURPOSE.  See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along 
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 * END LICENSE */

#include "config.h"
#include "libeatmydata/visibility.h"

#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <stdarg.h>

/* 
#define CHECK_FILE "/tmp/eatmydata"
*/

typedef int (*libc_open_t)(const char*, int, ...);
typedef int (*libc_fsync_t)(int);
typedef int (*libc_sync_t)(void);
typedef int (*libc_fdatasync_t)(int);
typedef int (*libc_msync_t)(void*, size_t, int);

static libc_open_t libc_open= NULL;
static libc_fsync_t libc_fsync= NULL;
static libc_sync_t libc_sync= NULL;
static libc_fdatasync_t libc_fdatasync= NULL;
static libc_msync_t libc_msync= NULL;

#define ASSIGN_DLSYM_OR_DIE(name)			\
        libc_##name = (libc_##name##_##t)(intptr_t)dlsym(RTLD_NEXT, #name);			\
        if (!libc_##name || dlerror())				\
                _exit(1);

int LIBEATMYDATA_API msync(void *addr, size_t length, int flags);

void __attribute__ ((constructor)) eatmydata_init(void);

void __attribute__ ((constructor)) eatmydata_init(void)
{
	ASSIGN_DLSYM_OR_DIE(open);
	ASSIGN_DLSYM_OR_DIE(fsync);
	ASSIGN_DLSYM_OR_DIE(sync);
	ASSIGN_DLSYM_OR_DIE(fdatasync);
	ASSIGN_DLSYM_OR_DIE(msync);
}

static int eatmydata_is_hungry(void)
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

int LIBEATMYDATA_API fsync(int fd)
{
	if (eatmydata_is_hungry()) {
		errno= 0;
		return 0;
	}

	return (*libc_fsync)(fd);
}

/* no errors are defined for this function */
void LIBEATMYDATA_API sync(void)
{
	if (eatmydata_is_hungry())
		return;

	(*libc_sync)();
}

int LIBEATMYDATA_API open(const char* pathname, int flags, ...)
{
	va_list ap;
	mode_t mode;

	va_start(ap, flags);
	mode= va_arg(ap, mode_t);
	va_end(ap);

	/* In pthread environments the dlsym() may call our open(). */
	/* We simply ignore it because libc is already loaded       */
	if (!libc_open) {
		errno = EFAULT;
		return -1;
	}

	if (eatmydata_is_hungry())
		flags &= ~(O_SYNC|O_DSYNC);

	return (*libc_open)(pathname,flags,mode);
}

int LIBEATMYDATA_API fdatasync(int fd)
{
	if (eatmydata_is_hungry()) {
		errno= 0;
		return 0;
	}

	return (*libc_fdatasync)(fd);
}

int LIBEATMYDATA_API msync(void *addr, size_t length, int flags)
{
	if (eatmydata_is_hungry()) {
		errno= 0;
		return 0;
	}

	return (*libc_msync)(addr, length, flags);
}
