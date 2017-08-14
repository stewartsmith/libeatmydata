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

/**
 * @file
 * @brief libeatmydata portability abstraction header
 */

#ifndef LIBEATMYDATA_PORTABILITY_H
#define LIBEATMYDATA_PORTABILITY_H

#include "config.h"

/*
 * Mac OS X 10.7 doesn't declare fdatasync().
 */
#if defined(HAVE_FDATASYNC) && HAVE_DECL_FDATASYNC == 0
int fdatasync(int fd);
#endif

#endif
