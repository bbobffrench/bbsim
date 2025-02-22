#include "bb.h"
#include "graphics.h"

int
main(int argc, char **argv){
	buffalo_byte_t bb;
	window_ctx_t win_ctx;
	SDL_Event e;
	char running = 1;

	bb_init(&bb);
	if(!init_window_ctx(&win_ctx)){
		cleanup_window_ctx(&win_ctx);
		fprintf(stderr, "ERR: Failed to initialize window context\n");
		return 1;
	}
	bb.theta = 0.3;
	clear_window(&win_ctx);
	draw_bb(&win_ctx, &bb, 0);
	present_window(&win_ctx);
	while(running) while(SDL_PollEvent(&e)) if(e.type == SDL_QUIT) running = 0;
	cleanup_window_ctx(&win_ctx);
	return 0;
}
