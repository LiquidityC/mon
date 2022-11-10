#include <stdlib.h>
#include <string.h>
#include "command_list.h"
#include "memory.h"

struct command_list *command_list_create(void)
{
	struct command_list *cl = ec_malloc(sizeof(struct command_list));
	cl->len = 0;
	cl->cmd = ec_calloc(MAX_COMMANDS, sizeof(char *));

	for (size_t i = 0; i < MAX_COMMANDS; i++) {
		cl->cmd[i] = ec_malloc(MAX_CMD_LENGTH * sizeof(char));
		memset(cl->cmd[i], 0, MAX_CMD_LENGTH);
	}

	return cl;
}

void command_list_destroy(struct command_list *cl)
{
	for (size_t i = 0; i < MAX_COMMANDS; ++i) {
		free(cl->cmd[i]);
	}
	free(cl->cmd);
	cl->cmd = NULL;
	free(cl);
}
