CC = cc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -g
PREFIX = ~/.local

todo: todo.c
install: todo
	cp todo ${PREFIX}/bin

clean:
	rm -f todo

.PHONY: clean
