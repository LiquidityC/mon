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

void *ec_calloc(size_t nmemb, size_t size)
{
	void *ptr = calloc(nmemb, size);
	if (ptr == NULL) {
		perror("calloc");
		exit(EXIT_FAILURE);
	}
	return ptr;
}
