#ifndef _COMMAND_LIST_H_
#define _COMMAND_LIST_H_

#include <stdint.h>
#include <stddef.h>

#define MAX_CMD_LENGTH 250
#define MAX_COMMANDS 5

struct command_list {
	size_t len;
	char **cmd;
};

struct command_list *command_list_create(void);

void command_list_destroy(struct command_list *cl);


#endif // _COMMAND_LIST_H_
