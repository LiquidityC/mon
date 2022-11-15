[![Build and Check](https://github.com/LiquidityC/mon/actions/workflows/build-check.yml/badge.svg)](https://github.com/LiquidityC/mon/actions/workflows/build-check.yml)

## mon - react to changes in a file system path

**mon** Is a utility program that can trigger user provided shell commands when
file system changes occur in a provided directory path. It can be useful if you
want to trigger tests automatically while developing some code. Or
automatically move new files once created. The main goal is to do it's job
through a simple command line interface and not require any deep understanding
of file system events.

## Synopsis

**mon** -f *FILE* -c *COMMAND*

## Examples

**mon** -p */path/to/dir* -c *"make test"*

## Compiling

In the source root type `make`. To build a release version use `RELEASE_BUILD=1
make`. You can also generate man files `make man`, this requires
[pandoc](https://pandoc.org/). Running tests and formatting the source code
`make check` and `make fmt` require
[cppcheck](https://cppcheck.sourceforge.io/) and
[clang-format](https://clang.llvm.org/docs/ClangFormat.html).

## Installation

The command `make install` will build a file structure of the project in the
linux format. By default this will b output into the `<project-root>/out`
directory. This can be controlled by the `PREFIX` environment variable. Eg.
`PREFIX=~/.local make install`. In most cases you will want to combine this
with the `RELEASE_BUILD` environment variable. Eg. `RELEASE_BUILD=1
PREFIX=~/.local make install`
