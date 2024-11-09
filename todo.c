#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TITLE_LENGTH 100
#define LIST_LENGTH 100
#define FILE_NAME "/home/farid/.local/share/todo.bin"

typedef struct Task Task;
struct Task {
	char title[TITLE_LENGTH];
	int done;
};

typedef struct Todo Todo;
struct Todo {
	Task list[100];
	size_t len;
};

Todo
read_file(const char *filepath)
{
	Todo todo = {0};
	FILE *file = fopen(filepath, "rb");
	if (file) {
		fread(&todo, 1, sizeof todo, file);
		fclose(file);
	}
	return todo;
}

void
save_todo_to_file(const char *filepath, Todo todo)
{
	FILE *file = fopen(filepath, "wb");
	fwrite(&todo, 1, sizeof todo, file);
	fclose(file);
}

void
ls(void)
{
	Todo todo = read_file(FILE_NAME);

	for (size_t i = 0; i != todo.len; i++) {
		const char *strike = todo.list[i].done ? "\x1B[9m" : "";
		const char *reset = "\x1B[0m";

		printf("%lu: %s%s%s\n", i, strike, todo.list[i].title, reset);
	}
}

void
add(const char *title)
{
	Todo todo = read_file(FILE_NAME);
	char *dest = todo.list[todo.len].title;

	if (LIST_LENGTH <= todo.len) {
		fprintf(stderr, "error: too many tasks");
		exit(1);
	}
	strcpy(dest, title);
	todo.list[todo.len++].done = 0;
	save_todo_to_file(FILE_NAME, todo);
}

size_t
arg_to_id(const char *arg)
{
	return strtoul(arg, NULL, 10);
}

void
rm(char *str_id)
{
	Todo todo = read_file(FILE_NAME);
	size_t id = arg_to_id(str_id);

	if (id >= todo.len) {
		fprintf(stderr, "error: task id out of range -- '%s'\n", str_id);
		exit(1);
	}
	for (size_t i = id; i != todo.len; i++)
		memcpy(todo.list + i, todo.list + i + 1, sizeof *todo.list);
	todo.len--;
	save_todo_to_file(FILE_NAME, todo);
}

void
complete(char *str_id)
{
	Todo todo = read_file(FILE_NAME);
	size_t id = arg_to_id(str_id);

	if (id >= todo.len) {
		fprintf(stderr, "error: task id out of range -- '%s'\n", str_id);
		exit(1);
	}
	todo.list[id].done = 1;
	save_todo_to_file(FILE_NAME, todo);
}

void
usage()
{
	fputs("usage:\n"
	"      todo [ls]\n"
	"      todo add <task>\n"
	"      todo rm <task_id>\n"
	"      todo do <task_id>\n",
	stderr);
}

int
main(int argc, char *argv[])
{
	if (argc == 1 || !strcmp(argv[1], "ls")) {
		ls();
		return 0;
	}
	if (!strcmp(argv[1], "add")) {
		if (argc == 2) {
			fputs("error: add requires a task\n", stderr);
			return 1;
		}
		add(argv[2]);
		return 0;
	}
	if (!strcmp(argv[1], "rm")) {
		if (argc == 2) {
			fputs("error: rm requires a task id\n", stderr);
			return 1;
		}
		rm(argv[2]);
		return 0;
	}
	if (!strcmp(argv[1], "do")) {
		if (argc == 2) {
			fputs("error: do requires a task id\n", stderr);
			return 1;
		}
		complete(argv[2]);
		return 0;
	}
	fprintf(stderr, "error: invalid command -- '%s'\n", argv[1]);
	usage();
	return 1;
}
