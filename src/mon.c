/**
 * mon - React to change in a filesystem path
 * Copyright (C) 2022  Linus Probert
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/inotify.h>
#include "mon.h"

static int32_t fd = -1;

int32_t mon_init(void)
{
	fd = inotify_init();
	if (fd == -1) {
		perror("inotify_init");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int32_t mon_watch(int32_t *wd, const char *path)
{
	*wd = inotify_add_watch(fd, path,
				IN_CLOSE_WRITE | IN_MODIFY | IN_CREATE);
	if (*wd == -1) {
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

ssize_t mon_read(char *buf, size_t buflen)
{
	return read(fd, buf, buflen);
}

void mon_close(void)
{
	if (fd != -1)
		close(fd);
}
