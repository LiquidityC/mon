#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#define MAX_CMD_LENGTH 250
#define MAX_COMMANDS 5

typedef struct CommandList {
	size_t len;
	char cmd[MAX_COMMANDS][MAX_CMD_LENGTH];
} CommandList;

static void ParseCommands(CommandList *list, char *cmd)
{
	char *end;
	char *start = cmd;

	/* For each ',' in the command string */
	while ((end = strchr(start, ',')) != NULL) {
		if (start == end) {
			start++;
			continue;
		}
		char orig = *end;
		*end = '\0';
		strncpy(list->cmd[list->len], start, MAX_CMD_LENGTH);
		*end = orig;
		start = end + 1;
		list->len++;

		if (list->len >= MAX_COMMANDS) {
			fprintf(stderr,
				"[!!] Too many commands. Max allowed is: %d\n",
				MAX_COMMANDS);
			exit(EXIT_FAILURE);
		}
	}

	/* Include the final command if available */
	if (*start != '\0') {
		strncpy(list->cmd[list->len], start, MAX_CMD_LENGTH);
		list->len++;
	}
}

static int32_t RunCommands(CommandList *list)
{
	int32_t status;
	for (size_t i = 0; i < list->len; ++i) {
		printf("Running command: %s\n", list->cmd[i]);
		status = system(list->cmd[i]);
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
	CommandList command_list;
	command_list.len = 0,
	memset(command_list.cmd, '\0',
	       MAX_COMMANDS * MAX_CMD_LENGTH * sizeof(char));

	while ((opt = getopt(argc, argv, "c:")) != -1) {
		switch (opt) {
		case 'c':
			ParseCommands(&command_list, optarg);
			break;
		default:
			break;
		}
	}

	return RunCommands(&command_list);
}
