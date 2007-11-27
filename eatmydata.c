
#ifndef RTLD_NEXT
#  define _GNU_SOURCE
#endif

#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dlfcn.h>

int errno;

int fsync(int fd)
{
	errno=0;
	return 0;
}

int open(const char* pathname, int flags, ...)
{
	int (*libc_open)(const char*,int,...);
	*(void**)(&libc_open) = dlsym(RTLD_NEXT, "open");
	if(dlerror()) {
		errno = EACCES;
		return -1;
	}

	flags &= !(O_SYNC|O_DSYNC);

	return (*libc_open)(pathname,flags);
}
