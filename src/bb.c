#include "bb.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BB_ACCEL 1
#define BB_MAX_VELOCITY 0.5
#define BB_ANGULAR_VELOCITY M_PI

void
add_command(command_list_t **list, enum command cmd, float arg){
	command_list_t *new, *cur;

	new = malloc(sizeof(command_list_t));
	new->next = NULL;
	new->cmd = cmd;
	new->arg = arg;
	if(!*list) *list = new;
	else{
		for(cur = *list; cur->next != NULL; cur = cur->next);
		cur->next = new;
	}
}

bool
pop_command(command_list_t **list){
	command_list_t *next;

	if(!*list) return 0;
	next = (*list)->next;
	free(*list);
	*list = next;
	return 1;
}

void
free_command_list(command_list_t **list){
	while(pop_command(list));
	*list = NULL;
}

bool
load_command_list(command_list_t **list, char *filepath){
	FILE *fp;
	char cmd_str[16];
	enum command cmd;
	float arg;

	fp = fopen(filepath, "r");
	if(!fp){
		fprintf(stderr, "ERR: Failed to open %s, file does not exist\n", filepath);
		return 0;
	}
	while(fscanf(fp, "%s", cmd_str) != EOF){
		if(!strcmp(cmd_str, "FORWARD")) cmd = FORWARD;
		else if(!strcmp(cmd_str, "BACK")) cmd = BACK;
		else if(!strcmp(cmd_str, "ROTATE")) cmd = ROTATE;
		else{
			fprintf(stderr, "ERR: Command %s is not valid\n", cmd_str);
			fclose(fp);
			return false;
		}
		if(fscanf(fp, "%f\n", &arg) == EOF){
			fprintf(stderr, "ERR: No argument provided to %s command\n", cmd_str);
			fclose(fp);
			return false;
		}
		add_command(list, cmd, arg);
	}
	fclose(fp);
	return true;
}

void
init_bb(buffalo_byte_t *bb, float x, float y){
	bb->x = x;
	bb->y = y;
	bb->velocity = bb->accel = 0;
}
