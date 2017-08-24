/* BEGIN LICENSE
 * Copyright (C) 2017 Stewart Smith <stewart@flamingspork.com>
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

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#define TESTFN "fcntltestdata"

int main(void)
{
	int fd, err;
	ssize_t write_ret;

	errno= 0;
	fd= open(TESTFN, O_CREAT | O_RDWR | O_SYNC | O_DSYNC | O_TRUNC, 0700);
	assert(fd > 0);
	assert(errno == 0);

#ifdef F_FULLFSYNC
	err= fcntl(fd, F_FULLFSYNC, 0);
	assert(err != -1);
	assert(errno == 0);
#endif

	write_ret= write(fd, "a", 1);
	assert(write_ret == 1);
	assert(errno == 0);

	err= close(fd);
	assert(err == 0);
	assert(errno == 0);

	err= unlink(TESTFN);
	assert(err == 0);
	assert(errno == 0);

	return 0;
}
