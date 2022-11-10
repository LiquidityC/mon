CC			= gcc
LDFLAGS		=
CFLAGS		= -g
RM			= rm
MODULE		= onchange
CLANGFORMAT	= clang-format

SRC = \
	src/main.c

OBJ=$(patsubst %.c,%.o,$(SRC))

all: $(MODULE)

$(OBJ): %.o: %.c
	$(CC) -c $< -o $@

$(MODULE): $(OBJ)
	$(CC) -o $@ $^

run: $(MODULE)
	@./$(MODULE)

clean:
	$(RM) -rf $(OBJ) $(MODULE)

fmt:
	@$(CLANGFORMAT) -i src/*.[ch]

.PHONY: $(MODULE) clean all fmt run
