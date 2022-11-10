#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <getopt.h>

#include "command_list.h"
#include "string_utils.h"

static int32_t run_commands(struct command_list *list)
{
	for (size_t i = 0; i < list->len; ++i) {
		printf("Running command: %s\n", list->cmd[i]);
		int32_t status = system(list->cmd[i]);
		if (status == -1) {
			perror("system");
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}

int main(int32_t argc, char *argv[])
{
	int32_t opt;

	/* Prepare the command list */
	struct command_list *cmd_list = command_list_create();

	while ((opt = getopt(argc, argv, "c:")) != -1) {
		switch (opt) {
		case 'c': {
			cmd_list->len = string_parse_command(optarg, ',',
							     cmd_list->cmd,
							     MAX_COMMANDS,
							     MAX_CMD_LENGTH);
		} break;
		default:
			break;
		}
	}

	return run_commands(cmd_list);

	command_list_destroy(cmd_list);
}
