#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

int errno;

int fsync(int fd)
{
	errno=0;
	return 0;
}
