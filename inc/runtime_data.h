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
#ifndef _RUNTIME_DATA_H_
#define _RUNTIME_DATA_H_

#include <stddef.h>
#include <stdbool.h>
#include "string_list.h"

struct monitor {
	int wd;
	char path[MAX_STRING_LIST_STRING_LEN];
};

struct runtime_data {
	struct string_list *cmd_list;
	struct string_list *file_list;
	struct monitor *monitors;
	bool clear_first;
	size_t fcount;
};

struct runtime_data *rd_create(void);

void rd_destroy(struct runtime_data *rd);

#endif // _RUNTIME_DATA_H_
