#ifndef BUFFALO_BYTE_H
#define BUFFALO_BYTE_H

#include <stdbool.h>

enum command{FORWARD, BACK, ROTATE};

typedef struct command_list{
	struct command_list *next;
	enum command cmd;
	float arg;
} command_list_t;

typedef struct buffalo_byte{
	float x, y;
	float velocity;
	int accel;
} buffalo_byte_t;

void add_command(command_list_t **, enum command, float);
bool pop_command(command_list_t **);
void free_command_list(command_list_t **);
bool load_command_list(command_list_t **, char *);

void init_bb(buffalo_byte_t *, float, float);

#endif
