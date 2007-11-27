#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	int fd;
	fd= open(argv[0],O_RDONLY|O_SYNC);
	fsync(fd);
	close(fd);
	return 0;
}
