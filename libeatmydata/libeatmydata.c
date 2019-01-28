/* BEGIN LICENSE
 * Copyright (C) 2008-2014 Stewart Smith <stewart@flamingspork.com>
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
#include "libeatmydata/portability.h"
#include "libeatmydata/visibility.h"

#undef _FILE_OFFSET_BITS // Hack to get open and open64 on 32bit
#undef __USE_FILE_OFFSET64
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <stdarg.h>
#include <stdio.h>
#ifdef HAVE_PTHREAD_H
#include <pthread.h>
#endif

/* 
#define CHECK_FILE "/tmp/eatmydata"
*/

typedef int (*libc_open_t)(const char*, int, ...);
#ifdef HAVE_OPEN64
typedef int (*libc_open64_t)(const char*, int, ...);
#endif
typedef int (*libc_fsync_t)(int);
typedef int (*libc_sync_t)(void);
typedef int (*libc_fdatasync_t)(int);
typedef int (*libc_msync_t)(void*, size_t, int);
#ifdef HAVE_SYNC_FILE_RANGE
typedef int (*libc_sync_file_range_t)(int, off64_t, off64_t, unsigned int);
#endif
#if defined(F_FULLFSYNC) && defined(__APPLE__)
typedef int (*libc_fcntl_t)(int, int, ...);
#endif

/* All the following are thread-local, to avoid initialization races between
 * threads. */
static TLS int init_running = 0;
static TLS int init_complete = 0;
static TLS libc_open_t libc_open= NULL;
#ifdef HAVE_OPEN64
static TLS libc_open64_t libc_open64= NULL;
#endif
static TLS libc_fsync_t libc_fsync= NULL;
static TLS libc_sync_t libc_sync= NULL;
static TLS libc_fdatasync_t libc_fdatasync= NULL;
static TLS libc_msync_t libc_msync= NULL;
#ifdef HAVE_SYNC_FILE_RANGE
static TLS libc_sync_file_range_t libc_sync_file_range= NULL;
#endif
#if defined(F_FULLFSYNC) && defined(__APPLE__)
static TLS libc_fcntl_t libc_fcntl= NULL;
#endif

#define ASSIGN_DLSYM_OR_DIE(name)			\
        libc_##name = (libc_##name##_##t)(intptr_t)dlsym(RTLD_NEXT, #name);			\
        if (!libc_##name)                       \
        {                                       \
                const char *dlerror_str = dlerror();                          \
                fprintf(stderr, "libeatmydata init error for %s: %s\n", #name,\
                        dlerror_str ? dlerror_str : "(null)");                \
                _exit(1);                       \
        }

#define ASSIGN_DLSYM_IF_EXIST(name)			\
        libc_##name = (libc_##name##_##t)(intptr_t)dlsym(RTLD_NEXT, #name);			\
						   dlerror();

#pragma weak pthread_testcancel

int LIBEATMYDATA_API msync(void *addr, size_t length, int flags);

void __attribute__ ((constructor)) eatmydata_init(void);

void __attribute__ ((constructor)) eatmydata_init(void)
{
	init_running++;
	ASSIGN_DLSYM_OR_DIE(open);
#ifdef HAVE_OPEN64
	ASSIGN_DLSYM_OR_DIE(open64);
#endif
	ASSIGN_DLSYM_OR_DIE(fsync);
	ASSIGN_DLSYM_OR_DIE(sync);
	ASSIGN_DLSYM_OR_DIE(fdatasync);
	ASSIGN_DLSYM_OR_DIE(msync);
#ifdef HAVE_SYNC_FILE_RANGE
	ASSIGN_DLSYM_IF_EXIST(sync_file_range);
#endif
#if defined(F_FULLFSYNC) && defined(__APPLE__)
	ASSIGN_DLSYM_OR_DIE(fcntl);
#endif
	init_running--;
	init_complete++;
}

static int eatmydata_is_hungry(void)
{
	/* Init here, as it is called before any libc functions */
	if(!init_complete)
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
		if (pthread_testcancel)
			pthread_testcancel();
		if (fcntl(fd, F_GETFD) == -1) {
		  return -1;
		}
		errno= 0;
		return 0;
	}

	return (*libc_fsync)(fd);
}

/* no errors are defined for this function */
void LIBEATMYDATA_API sync(void)
{
	if (eatmydata_is_hungry()) {
		return;
	}

	(*libc_sync)();
}

int LIBEATMYDATA_API open(const char* pathname, int flags, ...)
{
	va_list ap;
	mode_t mode;

	va_start(ap, flags);
#if SIZEOF_MODE_T < SIZEOF_INT
	mode= (mode_t) va_arg(ap, int);
#else
	mode= va_arg(ap, mode_t);
#endif
	va_end(ap);

	/* If we get called recursively during initialization (which should
	 * be rare but might happen), just fail. */
	if (init_running > 0) {
		errno = EFAULT;
		return -1;
	}

	if (eatmydata_is_hungry())
		flags &= ~(O_SYNC|O_DSYNC);

	return (*libc_open)(pathname,flags,mode);
}

#if !defined(__USE_FILE_OFFSET64) && defined(HAVE_OPEN64)
int LIBEATMYDATA_API open64(const char* pathname, int flags, ...)
{
	va_list ap;
	mode_t mode;

	va_start(ap, flags);
#if SIZEOF_MODE_T < SIZEOF_INT
	mode= (mode_t) va_arg(ap, int);
#else
	mode= va_arg(ap, mode_t);
#endif
	va_end(ap);

	/* If we get called recursively during initialization (which should
	 * be rare but might happen), just fail. */
	if (init_running > 0) {
		errno = EFAULT;
		return -1;
	}

	if (eatmydata_is_hungry())
		flags &= ~(O_SYNC|O_DSYNC);

	return (*libc_open64)(pathname,flags,mode);
}
#endif

int LIBEATMYDATA_API fdatasync(int fd)
{
	if (eatmydata_is_hungry()) {
		if (pthread_testcancel)
			pthread_testcancel();
		if (fcntl(fd, F_GETFD) == -1) {
		  return -1;
		}
		errno= 0;
		return 0;
	}

	return (*libc_fdatasync)(fd);
}

int LIBEATMYDATA_API msync(void *addr, size_t length, int flags)
{
	if (eatmydata_is_hungry()) {
		if (pthread_testcancel)
			pthread_testcancel();
		errno= 0;
		return 0;
	}

	return (*libc_msync)(addr, length, flags);
}

#ifdef HAVE_SYNC_FILE_RANGE
int LIBEATMYDATA_API sync_file_range(int fd, off64_t offset, off64_t nbytes,
				     unsigned int flags)
{
	if (eatmydata_is_hungry()) {
		if (pthread_testcancel)
			pthread_testcancel();
		if (fcntl(fd, F_GETFD) == -1) {
		  return -1;
		}
		errno= 0;
		return 0;
	}

	return (libc_sync_file_range)(fd, offset, nbytes, flags);
}
#endif

#if defined(F_FULLFSYNC) && defined(__APPLE__)

/* fcntl does not have a va_list version. We have to trust this one seen in
public Darwin sources:
int __FCNTL(int, int, void *); */

int LIBEATMYDATA_API fcntl(int fd, int cmd, ...)
{
	if ((eatmydata_is_hungry() && (cmd == F_FULLFSYNC))) {
		if (fcntl(fd, F_GETFD) == -1) {
			return -1;
		}
		errno= 0;
		return 0;
	} else {

		va_list args;
		va_start(args, cmd);
		void * arg= va_arg(args, void *);
		va_end(args);
		return ((libc_fcntl)(fd, cmd, arg));
	}
}

#endif
