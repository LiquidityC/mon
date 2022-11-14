% MON(1) mon 0.1.0
% Linus Probert
% November 2022

# NAME

mon - react to changes in a filesystem path

# SYNOPSIS

**mon** -f *FILE* -c *COMMAND*

# EXAMPLES

**mon** -p */path/to/dir* -c *"make test"*

# DESCRIPTION

**mon** monitors a directory or file for change (writes). If one occurs the
provided commands will execute in order of apperance.

# OPTIONS

**-p** _PATH_, **--path**=_PATH_
: A file or directory to monitor. Can be provided multiple times.

**-c** _COMMAND_, **--command**=_COMMAND_
: A command to run when a change is detected. Can be provided multiple times.

# BUGS

Please report any bugs at <https://github.com/LiquidityC/mon>

# COPYRIGHT
Copyright (C) 2022  Linus Probert

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
