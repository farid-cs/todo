CC = cc
CFLAGS = -std=c11 -pedantic -Wall -Wextra -g
PREFIX = ~/.local

todo: todo.c
install: todo
	cp todo ${PREFIX}/bin

clean:
	rm -f todo

.PHONY: clean
