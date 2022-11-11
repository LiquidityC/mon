#ifndef _STRING_LIST_H_
#define _STRING_LIST_H_

#include <stdint.h>
#include <stddef.h>

#define MAX_STRING_LIST_STRING_LEN 250

struct string_list {
	char cmd[MAX_STRING_LIST_STRING_LEN];
	struct string_list *next;
};

struct string_list *string_list_create(const char *cmd);

void string_list_add(struct string_list *node, const char *cmd);

void string_list_destroy(struct string_list *node);


#endif // _STRING_LIST_H_
