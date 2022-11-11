#include "memory.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

void *ec_malloc(size_t size)
{
	void *ptr = malloc(size);
	if (ptr == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	return ptr;
}
