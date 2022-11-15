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
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <getopt.h>
#include <sys/inotify.h>
#include <errno.h>
#include <stdbool.h>
#include <signal.h>
#include <stdatomic.h>

#include "string_list.h"
#include "memory.h"
#include "log.h"

struct monitor {
	int wd;
	char path[MAX_STRING_LIST_STRING_LEN];
};

static atomic_bool quit = false;

static void signal_handler(int32_t signal)
{
	log_dbg("Caught signal: %d", signal);
	quit = true;
}

static int32_t run_commands(struct string_list *commands)
{
	while (commands) {
		log_inf("Running command: %s", commands->val);
		int32_t status = system(commands->val);
		if (status == -1) {
			perror("system");
			return EXIT_FAILURE;
		};
		commands = commands->next;
	}
	return EXIT_SUCCESS;
}

static int32_t init_monitors(int32_t fd, struct monitor *monitors,
			     struct string_list *file_list, size_t fcount)
{
	struct string_list *it = file_list;
	size_t index = 0;
	while (it) {
		log_inf("Watching: %s", it->val);
		strncpy(monitors[index].path, it->val,
			MAX_STRING_LIST_STRING_LEN);
		monitors[index].wd = inotify_add_watch(
			fd, it->val, IN_CLOSE_WRITE | IN_MODIFY | IN_CREATE);
		if (monitors[index].wd == -1) {
			perror("inotify_add_watch");
			return EXIT_FAILURE;
		}
		index++;
		it = it->next;
	}
	return EXIT_SUCCESS;
}

static int32_t run_monitors(int32_t fd, struct monitor *monitors,
			    struct string_list *cmd_list, size_t fcount)
{
	/* Setup pollfd */
	struct pollfd pfd;
	pfd.fd = fd;
	pfd.events = POLLIN;

	/* Align bytes because reasons. See `man inotify` (code example) */
	char buf[4096]
		__attribute__((aligned(__alignof__(struct inotify_event))));

	ssize_t read_len;

	log_dbg("Entering main loop");
	while (true) {
		if (poll(&pfd, 1, 500)) {
			if (quit)
				break;

			log_inf("Change detected");

			read_len = read(fd, buf, 4096);
			if (read_len == -1) {
				perror("read");
				return EXIT_FAILURE;
			}

			for (char *ptr = buf; ptr < buf + read_len;
			     ptr += sizeof(struct inotify_event)) {
				struct inotify_event *event =
					(struct inotify_event *)ptr;

				if (event->mask & IN_CLOSE_WRITE) {
					log_dbg("Event: IN_CLOSE_WRITE");
				} else if (event->mask & IN_MODIFY) {
					log_dbg("Event: IN_MODIFY");
				} else if (event->mask & IN_CREATE) {
					log_dbg("Event: IN_CREATE");
				}
				for (size_t i = 0; i < fcount; ++i) {
					if (monitors[i].wd == event->wd) {
						log_inf("Path:\t%s",
							monitors[i].path);
						break;
					}
				}
				if (event->len)
					log_inf("File:\t%s", event->name);
			}

			if (run_commands(cmd_list) != EXIT_SUCCESS) {
				return EXIT_FAILURE;
			}
		}
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

	/* Setup signal handler */
	signal(SIGINT, signal_handler);
	signal(SIGHUP, signal_handler);

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

	/* Setup the monitors */
	struct monitor *monitors = NULL;
	monitors = ec_calloc(fcount, sizeof(struct monitor));
	init_monitors(fd, monitors, file_list, fcount);

	run_monitors(fd, monitors, cmd_list, fcount);

cleanup:
	log_inf("Shutting down");

	if (fd)
		close(fd);
	if (wd)
		free(wd);
	if (cmd_list)
		string_list_destroy(cmd_list);
	if (file_list)
		string_list_destroy(file_list);
	if (monitors)
		free(monitors);

	return 0;
}
