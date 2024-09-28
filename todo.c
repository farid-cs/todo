#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TITLE_LENGTH 100

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

const char* FILE_NAME = "todo.bin";

Todo read_file(const char* file_name)
{
	Todo todo = {0};
	FILE* file = fopen(file_name, "rb");
	if (file) {
		fread(&todo, 1, sizeof(todo), file);
		fclose(file);
	}
	return todo;
}

void ls(void)
{
	Todo todo = read_file(FILE_NAME);

	for (size_t i = 0; i != todo.len; i++) {
		if (todo.list[i].done) {
			printf("%lu \x1B[9m%s\x1B[0m\n", i+1, todo.list[i].title);
		} else {
			printf("%lu %s\n", i+1,  todo.list[i].title);
		}
	}
}

void add(const char* task)
{
	Todo todo = read_file(FILE_NAME);

	if (sizeof(todo.list)/sizeof(*todo.list) <= todo.len) {
		return;
	}

	strcpy(todo.list[todo.len].title, task);
	todo.list[todo.len].done = 0;
	todo.len++;

	FILE* file = fopen(FILE_NAME, "wb");
	fwrite(&todo, 1, sizeof(todo), file);
	fclose(file);
}

void rm(char* str_id)
{
	Todo todo = read_file(FILE_NAME);
	size_t id = strtol(str_id, NULL, 10) - 1;

	if (id >= todo.len) {
		fprintf(stderr, "error: unknown task id -- '%s'\n", str_id);
		exit(1);
	}

	for (size_t i = id; i != todo.len; i++) {
		memcpy(todo.list + i, todo.list + i + 1, sizeof(Task));
	}

	todo.len--;

	FILE* file = fopen(FILE_NAME, "wb");
	fwrite(&todo, 1, sizeof(todo), file);
	fclose(file);
}

void complete(char* str_id)
{
	Todo todo = read_file(FILE_NAME);
	size_t id = strtol(str_id, NULL, 10) - 1;

	if (id >= todo.len) {
		fprintf(stderr, "error: unknown task id -- '%s'\n", str_id);
		exit(1);
	}

	todo.list[id].done = 1;

	FILE* file = fopen(FILE_NAME, "wb");
	fwrite(&todo, 1, sizeof(todo), file);
	fclose(file);
}

void usage()
{
	fputs("usage:\n"
	"      todo [ls]\n"
	"      todo add <task>\n"
	"      todo rm <task_id>\n"
	"      todo do <task_id>\n",
	stderr);
}

int main(int argc, char** argv)
{
	if (argc == 1) {
		ls();
		return 0;
	}
	if (!strcmp(argv[1], "ls")) {
		ls();
		return 0;
	}
	if (!strcmp(argv[1], "add")) {
		if (argc == 2) {
			fputs("error: add requires a task\n", stderr);
			usage();
			return 1;
		}
		add(argv[2]);
		return 0;
	}
	if (!strcmp(argv[1], "rm")) {
		if (argc == 2) {
			fputs("error: rm requires a task id\n", stderr);
			usage();
			return 1;
		}
		rm(argv[2]);
		return 0;
	}
	if (!strcmp(argv[1], "do")) {
		if (argc == 2) {
			fputs("error: do requires a task id\n", stderr);
			usage();
			return 1;
		}
		complete(argv[2]);
		return 0;
	}
	fprintf(stderr, "error: unknown command -- '%s'\n", argv[1]);
	usage();
	return 1;
}
