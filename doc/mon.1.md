% MON(1) mon 0.1.0
% Linus Probert
% November 2022

# NAME

mon - react to changes in a file system path

# SYNOPSIS

**mon** -f *FILE* -c *COMMAND*

# EXAMPLES

**mon** -p */path/to/dir* -c *"make test"*

# DESCRIPTION

**mon** Is a utility program that can trigger user provided shell commands when
file system changes occur in a provided directory path. It can be useful if you
want to trigger tests automatically while developing some code. Or
automatically move new files once created. The main goal is to do it's job
through a simple command line interface and not require any deep understanding
of file system events.

# OPTIONS

**-h**
: Print help

**-p** _PATH_
: A file or directory to monitor. Can be provided multiple times.

**-c** _COMMAND_
: A command to run when a change is detected. Can be provided multiple times.

**-C**
: Clear the terminal before executing the commands. Equivalent to '**-c** *clear*' as a first command.

**-v**, **-V**
: Verbose (*-v*) and extra verbose (*-V*) output

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
