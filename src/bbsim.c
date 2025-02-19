#include "bb.h"
#include "graphics.h"

#include <stdio.h>

int
main(int argc, char **argv){
	command_list_t *list;

	load_command_list(&list, "list.txt");
	for(; list != NULL; list = list->next){
		printf("cmd: %d, arg: %f\n", list->cmd, list->arg);
	}
	free_command_list(&list);
	return 0;
}
