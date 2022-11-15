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
#ifndef _MON_H_
#define _MON_H_

#include <stdint.h>
#include <stddef.h>
#include <unistd.h>

int32_t mon_init(void);

int32_t mon_watch(int32_t *wd, const char *fpath);

ssize_t mon_read(char *buf, size_t buflen);

void mon_close(void);

#endif // _MON_H_
