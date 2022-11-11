#include <stdlib.h>
#include <string.h>
#include "string_list.h"
#include "memory.h"

struct string_list *string_list_create(const char *cmd)
{
	struct string_list *cl = ec_malloc(sizeof(struct string_list));
	strncpy(cl->cmd, cmd, MAX_STRING_LIST_STRING_LEN);
	cl->next = NULL;
	return cl;
}

void string_list_add(struct string_list *node, const char *cmd)
{
	while (node->next) {
		node = node->next;
	}
	node->next = string_list_create(cmd);
}

void string_list_destroy(struct string_list *node)
{
	while (node) {
		struct string_list *next = node->next;
		free(node);
		node = next;
	}
}
