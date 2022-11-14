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
#ifndef _STRING_LIST_H_
#define _STRING_LIST_H_

#include <stdint.h>
#include <stddef.h>

#define MAX_STRING_LIST_STRING_LEN 250

struct string_list {
	char cmd[MAX_STRING_LIST_STRING_LEN];
	struct string_list *next;
};

struct string_list *string_list_create(const char *cmd);

void string_list_add(struct string_list *node, const char *cmd);

void string_list_destroy(struct string_list *node);


#endif // _STRING_LIST_H_
