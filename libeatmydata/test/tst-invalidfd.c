/* BEGIN LICENSE
 * Copyright (C) 2017 Yura Sorokin, Stewart Smith
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
#include <errno.h>
#include <unistd.h>
#include <assert.h>

#ifdef HAVE_SYNC_FILE_RANGE
#include <fcntl.h>
#endif

int main()
{
	int fd = 42;
	int r = fdatasync(fd);
	assert(r == -1 && errno == EBADF);
	r = fsync(fd);
	assert(r == -1 && errno == EBADF);
#ifdef HAVE_SYNC_FILE_RANGE
	r = sync_file_range(fd, 0, 0, 0);
	assert(r == -1 && errno == EBADF);
#endif
	return 0;
}
