#include "string_utils.h"
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

size_t string_parse_command(const char *src, char delim, char **buffer,
			    size_t nmemb, size_t maxlen)
{
	char *end;
	char *start = (char *)src;
	size_t len = 0;

	/* For each separator in the command string */
	while ((end = strchr(start, delim)) != NULL) {
		if (start == end) {
			start++;
			continue;
		}
		char orig = *end;
		*end = '\0';
		strncpy(buffer[len], start, maxlen);
		*end = orig;
		start = end + 1;
		len++;

		if (len >= nmemb) {
			break;
		}
	}

	/* Include the final command if available */
	if (*start != '\0') {
		strncpy(buffer[len], start, maxlen);
		len++;
	}

	return len;
}
