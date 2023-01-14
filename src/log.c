/**
 * mon - React to change in a filesystem path
 * Copyright (C) 2023  Linus Probert
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include <stdarg.h>
#include <stdio.h>
#include "log.h"

#define MAX_LOG_LEN 512

// TODO: This looks terrible... rethink it?
#ifdef RELEASE_BUILD
static enum log_level current_log_level = LOG_LVL_NONE;
#elif DEBUG_BUILD
static enum log_level current_log_level = LOG_LVL_DEBUG;
#else
static enum log_level current_log_level = LOG_LVL_INFO;
#endif

void set_log_level(enum log_level level)
{
	current_log_level = level;
}

void log_out(enum log_level level, const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	if (level <= current_log_level) {
		char buffer[MAX_LOG_LEN];
		vsnprintf(buffer, MAX_LOG_LEN, fmt, args);

		switch (level) {
		case LOG_LVL_DEBUG:
			printf("[**] %s\n", buffer);
			break;
		case LOG_LVL_INFO:
			printf("[--] %s\n", buffer);
			break;
		case LOG_LVL_ERROR:
			fprintf(stderr, "[!!] %s\n", buffer);
			break;
		case LOG_LVL_NONE:
		default:
			break;
		}
	}
	va_end(args);
}
