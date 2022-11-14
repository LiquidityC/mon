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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <getopt.h>
#include <sys/inotify.h>

#include "string_list.h"

static int32_t run_commands(struct string_list *commands)
{
	while (commands) {
		printf("Running command: %s\n", commands->cmd);
		int32_t status = system(commands->cmd);
		if (status == -1) {
			perror("system");
			return EXIT_FAILURE;
		};
		commands = commands->next;
	}
	return EXIT_SUCCESS;
}

int main(int32_t argc, char *argv[])
{
	int32_t opt;

	/* Prepare the command list */
	struct string_list *cmd_list = NULL;
	struct string_list *file_list = NULL;

	while ((opt = getopt(argc, argv, "p:c:")) != -1) {
		switch (opt) {
		case 'c': {
			if (cmd_list == NULL) {
				cmd_list = string_list_create(optarg);
			} else {
				string_list_add(cmd_list, optarg);
			}
		} break;
		case 'p': {
			if (file_list == NULL) {
				file_list = string_list_create(optarg);
			} else {
				string_list_add(file_list, optarg);
			}
		} break;
		default:
			break;
		}
	}

	(void)run_commands(cmd_list);

	string_list_destroy(cmd_list);
	string_list_destroy(file_list);

	return 0;
}
