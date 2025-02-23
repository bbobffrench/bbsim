#include "bb.h"
#include "graphics.h"

#define FPS 60

int
main(int argc, char **argv){
	buffalo_byte_t bb;
	window_ctx_t win_ctx;
	SDL_Event e;
	char running = 1;

	bb_init(&bb);
	bb.motor_l = bb.motor_r = 1;
	if(!init_window_ctx(&win_ctx)){
		cleanup_window_ctx(&win_ctx);
		fprintf(stderr, "ERR: Failed to initialize window context\n");
		return 1;
	}
	bb.motor_l = bb.motor_r = 1;
	clear_window(&win_ctx);
	while(running){
		/* Limit framerate */
		SDL_Delay((double)1 / FPS * 1000);

		while(SDL_PollEvent(&e)) if(e.type == SDL_QUIT) running = 0;
		clear_window(&win_ctx);
		bb_update(&bb, (double)1 / FPS);
		draw_bb(&win_ctx, &bb);
		present_window(&win_ctx);

		if(bb.angle > 10){
			bb.motor_l = 0;
			bb.motor_r = 1;
		}
		else if(bb.angle < -10){
			bb.motor_l = 1;
			bb.motor_r = 0;
		}
	}
	cleanup_window_ctx(&win_ctx);
	return 0;
}
