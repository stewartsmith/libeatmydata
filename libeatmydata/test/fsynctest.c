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

#include "libeatmydata/portability.h"

#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <sys/mman.h>

int main(int argc, char* argv[])
{
	(void)argc; (void)argv;
	int fd;
	int i;
	ssize_t ret;
	fd= open("fsynctestdata",O_CREAT|O_RDWR|O_SYNC|O_DSYNC|O_TRUNC, 0700);
	assert(fd > 0);
	assert(errno == 0);
	for (i = 0; i < 1000; ++i) {
		ret= write(fd, "a", 1);
		assert(errno == 0 && ret == 1);
		fsync(fd);
		assert(errno == 0);
		ret= write(fd, "a", 1);
		assert(errno == 0 && ret == 1);
		fdatasync(fd);
		assert(errno == 0);
		ret= write(fd, "a", 1);
		assert(errno == 0 && ret == 1);
		sync();
		assert(errno == 0);
	}
	close(fd);
	unlink("fsynctestdata");
	msync(0,0,0);
	assert(errno == 0);
	return 0;
}
