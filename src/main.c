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
#include <errno.h>

#include "string_list.h"
#include "memory.h"
#include "log.h"

static int32_t run_commands(struct string_list *commands)
{
	while (commands) {
		log_inf("Running command: %s", commands->cmd);
		int32_t status = system(commands->cmd);
		if (status == -1) {
			perror("system");
			return EXIT_FAILURE;
		};
		commands = commands->next;
	}
	return EXIT_SUCCESS;
}

static int create_watch(int32_t fd, int32_t *wd, const char *path)
{
	log_inf("Setting up watch: %s\n", path);
	*wd = inotify_add_watch(fd, path,
				IN_CLOSE_WRITE | IN_MODIFY | IN_CREATE);
	if (*wd == -1) {
		log_err("Failed to watch '%s': %s\n", path, strerror(errno));
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int main(int32_t argc, char *argv[])
{
	int32_t opt;

	/* Prepare the command list */
	struct string_list *cmd_list = NULL;
	struct string_list *file_list = NULL;

	size_t fcount = 0;
	int32_t fd = 0;
	int32_t *wd = NULL;

	while ((opt = getopt(argc, argv, "vVp:c:")) != -1) {
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
			fcount++;
		} break;
		case 'v': {
			set_log_level(LOG_LVL_INFO);
		} break;
		case 'V': {
			set_log_level(LOG_LVL_DEBUG);
		} break;
		default:
			break;
		}
	}

	/* Set-up inotify */
	fd = inotify_init();
	if (fd == -1) {
		perror("inotify_init");
		goto cleanup;
	}

	wd = ec_calloc(fcount, sizeof(int32_t));
	struct string_list *it = file_list;
	size_t index = 0;
	while (it) {
		if (create_watch(fd, &wd[index++], it->cmd) != EXIT_SUCCESS)
			goto cleanup;
		it = it->next;
	}

	char buf[4096]
		__attribute__((aligned(__alignof__(struct inotify_event))));

	ssize_t read_len;
	while (1) {
		read_len = read(fd, buf, 4096);
		if (read_len == -1) {
			perror("read");
			goto cleanup;
		}

		for (char *ptr = buf; ptr < buf + read_len;
		     ptr += sizeof(struct inotify_event)) {
			struct inotify_event *event =
				(struct inotify_event *)ptr;

			log_inf("Change detected");
			if (event->mask & IN_CLOSE_WRITE) {
				log_dbg("IN_CLOSE_WRITE");
			} else if (event->mask & IN_MODIFY) {
				log_dbg("IN_MODIFY");
			} else if (event->mask & IN_CREATE) {
				log_dbg("IN_CREATE");
			}
			if (event->len)
				log_dbg("Filename:\t%s", event->name);
		}
		if (run_commands(cmd_list) != EXIT_SUCCESS) {
			goto cleanup;
		}
	}

cleanup:
	if (fd)
		(void)close(fd);
	if (wd)
		free(wd);
	if (cmd_list)
		string_list_destroy(cmd_list);
	if (file_list)
		string_list_destroy(file_list);

	return 0;
}
