CC			= gcc
LDFLAGS		=
CFLAGS		= -g -Iinc -Wall -fsanitize=address -fno-omit-frame-pointer
RM			= rm
MODULE		= mon
FORMAT		= clang-format
CHECK		= cppcheck

SRC = \
	src/main.c \
	src/string_list.c \
	src/memory.c

OBJ=$(patsubst %.c,%.o,$(SRC))

all: $(MODULE)

$(OBJ): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(MODULE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

run: $(MODULE)
	@./$(MODULE)

clean:
	$(RM) -rf $(OBJ) $(MODULE)

fmt:
	@$(FORMAT) -i src/*.[ch]

check:
	@$(CHECK) -x c --std=c11 -Iinc -i/usr/include --enable=all --suppress=missingIncludeSystem .

.PHONY: $(MODULE) clean all fmt run check
