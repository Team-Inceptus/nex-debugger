CFILES = $(shell find src/ -name "*.c")
CFLAGS = -Werror=implicit-function-declaration -fsanitize=address -Iinclude
CC = gcc

.PHONY: all
all: mkbin bin/nexdebug

bin/nexdebug: $(CFILES)
	$(CC) $(CFLAGS) $^ -o $@

mkbin:
	mkdir -p bin
