#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <getopt.h>

#include "string_list.h"

static int32_t run_commands(struct string_list *commands)
{
	while (commands) {
		printf("Running command: %s\n", commands->cmd);
		int32_t status = system(commands->cmd);
		if (status == -1) {
			perror("system");
			return EXIT_FAILURE;
		}
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

	while ((opt = getopt(argc, argv, "f:c:")) != -1) {
		switch (opt) {
		case 'c': {
			if (cmd_list == NULL) {
				cmd_list = string_list_create(optarg);
			} else {
				string_list_add(cmd_list, optarg);
			}
		} break;
		case 'f': {
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

	return 0;
}
