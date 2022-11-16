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
#include "runtime_data.h"

#ifndef VERSION
#define VERSION "x.y.z"
#endif

static atomic_bool quit = false;

static void signal_handler(int32_t signal)
{
	log_dbg("Caught signal: %d", signal);
	quit = true;
}

static int32_t run_commands(struct runtime_data *rd)
{
	if (rd->clear_first)
		puts("\x1b[2J\x1b[H");

	struct string_list *it = rd->cmd_list;
	while (it) {
		log_inf("Running command: %s", it->val);
		int32_t status = system(it->val);
		if (status == -1) {
			perror("system");
			return EXIT_FAILURE;
		};
		it = it->next;
	}
	return EXIT_SUCCESS;
}

static int32_t init_monitors(int32_t fd, struct runtime_data *rd)
{
	rd->monitors = ec_calloc(rd->fcount, sizeof(struct monitor));
	struct string_list *it = rd->file_list;
	size_t index = 0;
	while (it) {
		log_inf("Watching: %s", it->val);
		strncpy(rd->monitors[index].path, it->val,
			MAX_STRING_LIST_STRING_LEN);
		rd->monitors[index].wd = inotify_add_watch(
			fd, it->val, IN_CLOSE_WRITE | IN_MODIFY | IN_CREATE);
		if (rd->monitors[index].wd == -1) {
			perror("inotify_add_watch");
			return EXIT_FAILURE;
		}
		index++;
		it = it->next;
	}
	return EXIT_SUCCESS;
}

static int32_t run_monitors(int32_t fd, struct runtime_data *rd)
{
	/* Setup pollfd */
	struct pollfd pfd;
	pfd.fd = fd;
	pfd.events = POLLIN;

	/* Align bytes because reasons. See `man inotify` (code example) */
	char buf[4096]
		__attribute__((aligned(__alignof__(struct inotify_event))));

	ssize_t read_len;
	bool ready_to_run = false;

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
			ready_to_run = true;

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
				for (size_t i = 0; i < rd->fcount; ++i) {
					if (rd->monitors[i].wd == event->wd) {
						log_inf("Path:\t%s",
							rd->monitors[i].path);
						break;
					}
				}
				if (event->len)
					log_inf("File:\t%s", event->name);
			}

		} else {
			/* Execute commands after a poll timeout. This will act as a form
             * of debounce to ensure that there are no pending events in the
             * stream */
			if (ready_to_run) {
				if (run_commands(rd) != EXIT_SUCCESS) {
					return EXIT_FAILURE;
				}
				ready_to_run = false;
			}
		}
	}
	return EXIT_SUCCESS;
}

static void print_usage(const char *prog)
{
	printf("USAGE: %s [options]\n", prog);
	printf("EXAMPLE: %s -p path/to/monitor -c 'command to run'\n", prog);
	printf("\n");
	printf("-h          Print this information\n");
	printf("-p PATH     A path/file to monitor for changes. Multiples allowed\n");
	printf("-c COMMAND  A command to run on change event. Multiples allowed\n");
	printf("-C          Clear screen before running commands\n");
	printf("-v          Verbose output\n");
	printf("-V          Debug output output\n");
}

static int32_t parse_options(int32_t argc, char **argv, struct runtime_data *rd)
{
	int32_t opt;

	while ((opt = getopt(argc, argv, "hvVp:Cc:")) != -1) {
		switch (opt) {
		case 'c': {
			if (rd->cmd_list == NULL) {
				rd->cmd_list = string_list_create(optarg);
			} else {
				string_list_add(rd->cmd_list, optarg);
			}
		} break;
		case 'p': {
			if (rd->file_list == NULL) {
				rd->file_list = string_list_create(optarg);
			} else {
				string_list_add(rd->file_list, optarg);
			}
			rd->fcount++;
		} break;
		case 'v': {
			set_log_level(LOG_LVL_INFO);
		} break;
		case 'V': {
			set_log_level(LOG_LVL_DEBUG);
		} break;
		case 'C': {
			rd->clear_first = true;
		} break;
		case 'h': {
			printf("mon v%s\n\n", VERSION);
			print_usage(argv[0]);
			return EXIT_FAILURE;
		} break;
		default:
			break;
		}
	}
	return EXIT_SUCCESS;
}

int main(int32_t argc, char *argv[])
{
	struct runtime_data *rd = rd_create();

	int32_t fd = 0;

	/* Setup signal handler */
	signal(SIGINT, signal_handler);
	signal(SIGHUP, signal_handler);

	if (parse_options(argc, argv, rd) != EXIT_SUCCESS) {
		goto cleanup;
	}

	if (!rd->file_list || !rd->cmd_list) {
		/* No commands or paths provided */
		log_err("At least one command (-c) and one path (-p) expected\n");
		print_usage(argv[0]);
		goto cleanup;
	}

	/* Set-up inotify */
	fd = inotify_init();
	if (fd == -1) {
		perror("inotify_init");
		goto cleanup;
	}

	/* Setup the monitors */
	struct monitor *monitors = NULL;
	monitors = ec_calloc(rd->fcount, sizeof(struct monitor));
	init_monitors(fd, rd);

	run_monitors(fd, rd);

cleanup:
	log_inf("Shutting down");

	if (fd)
		close(fd);
	if (monitors)
		free(monitors);
	rd_destroy(rd);

	return 0;
}
