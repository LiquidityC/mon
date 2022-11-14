CC			= gcc
LDFLAGS		=
CFLAGS		= -g -Iinc -Wall -fsanitize=address -fno-omit-frame-pointer
RM			= rm
MODULE		= mon
FORMAT		= clang-format
CHECK		= cppcheck
CP			= cp
ECHO		= echo
MKDIR		= mkdir
PANDOC		= pandoc

PREFIX ?= out

SRC = \
	src/main.c \
	src/string_list.c \
	src/memory.c

OBJ=$(patsubst %.c,%.o,$(SRC))

MDFILES = $(wildcard doc/mon.*.md)
MANFILES = $(subst doc/,doc/man/,$(patsubst doc/%.md,doc/%, $(MDFILES)))

default: $(MODULE)

all: $(MODULE) $(MANFILES)

$(OBJ): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(MODULE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

run: $(MODULE)
	@./$(MODULE)

clean:
	$(RM) -rf $(OBJ) $(MODULE) $(MANFILES) $(PREFIX)

fmt:
	@$(FORMAT) -i src/*.[ch]

check:
	@$(CHECK) -x c --std=c11 -Iinc -i/usr/include --enable=all --suppress=missingIncludeSystem .

$(MANFILES): $(MDFILES)
	@mkdir -p doc/man
	$(PANDOC) $< -s -t man -o $@

man: $(MANFILES)

install: $(MODULE) $(MANFILES)
	@$(MKDIR) -p $(PREFIX)/bin
	@$(MKDIR) -p $(PREFIX)/share/man/man1
	@$(ECHO) "Installing binary : $(PREFIX)/bin/$(MODULE)"
	@$(CP) -v $(MODULE) $(PREFIX)/bin/$(MODULE)
	@$(ECHO) "Installing man files"
	@$(CP) -v doc/man/*.1 $(PREFIX)/share/man/man1/

.PHONY: $(MODULE) clean all fmt run check install
