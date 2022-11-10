#ifndef _STRING_UTILS_H_
#define _STRING_UTILS_H_

#include <stddef.h>
#include <stdint.h>

#include "command_list.h"

size_t string_parse_command(const char *src, char delim, char **buffer, size_t nmemb,
		  size_t maxlen);

#endif // _STRING_UTILS_H_
