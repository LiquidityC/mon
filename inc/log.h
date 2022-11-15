/**
 * mon - React to change in a filesystem path
 * Copyright (C) 2022  Linus Probert
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
#ifndef _LOG_H_
#define _LOG_H_

enum log_level { LOG_LVL_NONE, LOG_LVL_ERROR, LOG_LVL_INFO, LOG_LVL_DEBUG };

void set_log_level(enum log_level level);

void log_out(enum log_level level, const char *fmt, ...);

#define log_dbg(fmt, args...) log_out(LOG_LVL_DEBUG, fmt, ##args)
#define log_inf(fmt, args...) log_out(LOG_LVL_INFO, fmt, ##args)
#define log_err(fmt, args...) log_out(LOG_LVL_ERROR, fmt, ##args)

#endif // _LOG_H_
