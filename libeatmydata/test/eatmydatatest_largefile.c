/*
    Copyright (C): 2013, Stewart Smith <stewart@flamingspork.com>

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

/* This test exists to test exactly the same thing as eatmydatatest but
   while defining _FILE_OFFSET_BITS to 64 which should enforce large file
   support on 32bit systems */
#define _FILE_OFFSET_BITS 64
#include "eatmydatatest.c"
