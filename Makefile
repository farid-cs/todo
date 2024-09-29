CFLAGS = -Wall -Wextra -O2
LDFLAGS = -s
PREFIX = ~/.local

todo: todo.c
install: todo
	cp todo ${PREFIX}/bin

clean:
	rm -f todo

.PHONY: clean
