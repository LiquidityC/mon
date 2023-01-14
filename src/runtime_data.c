/**
 * mon - React to change in a filesystem path
 * Copyright (C) 2023  Linus Probert
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
#include "runtime_data.h"
#include "string_list.h"
#include "memory.h"

#include <stdlib.h>

struct runtime_data *rd_create(void)
{
	struct runtime_data *rd = ec_malloc(sizeof(struct runtime_data));
	rd->cmd_list = NULL;
	rd->file_list = NULL;
	rd->monitors = NULL;
	rd->clear_first = false;
	rd->fcount = 0;
	return rd;
}

void rd_destroy(struct runtime_data *rd)
{
	if (rd->cmd_list)
		string_list_destroy(rd->cmd_list);
	if (rd->file_list)
		string_list_destroy(rd->file_list);
	if (rd->monitors)
		free(rd->monitors);
	free(rd);
}
