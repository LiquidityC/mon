CC			= gcc
LDFLAGS		=
CFLAGS		= -g -Iinc -Wall
RM			= rm
MODULE		= mon
FORMAT		= clang-format
CHECK		= cppcheck

SRC = \
	src/main.c \
	src/command_list.c \
	src/memory.c \
	src/string_utils.c

OBJ=$(patsubst %.c,%.o,$(SRC))

all: $(MODULE)

$(OBJ): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(MODULE): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

run: $(MODULE)
	@./$(MODULE)

clean:
	$(RM) -rf $(OBJ) $(MODULE)

fmt:
	@$(FORMAT) -i src/*.[ch]

check:
	@$(CHECK) -x c --std=c11 -Iinc -i/usr/include --enable=all --suppress=missingIncludeSystem .

.PHONY: $(MODULE) clean all fmt run check
