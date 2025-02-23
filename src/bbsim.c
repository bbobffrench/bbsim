#include "bb.h"
#include "graphics.h"

#define FPS 30

static char
event_loop(window_ctx_t *win_ctx, buffalo_byte_t *bb){
	SDL_Event e;
	static char straight;
	static double ref_angle = 0;

	SDL_Delay((double)1 / FPS * 1000);
	while(SDL_PollEvent(&e)){
		if(e.type == SDL_QUIT) return 0;
		else if(e.type == SDL_KEYDOWN)
			switch(e.key.keysym.sym){
			case SDLK_1:
				if(!straight) bb->motor_l = 1;
				break;
			case SDLK_2:
				if(!straight) bb->motor_r = 1;
				break;
			case SDLK_3:
				if(!straight){
					ref_angle = bb->angle;
					bb->motor_l = 1;
					bb->motor_r = 0;
				}
				straight = 1;
				break;
			}
		else if(e.type == SDL_KEYUP)
			switch(e.key.keysym.sym){
			case SDLK_1:
				if(!straight) bb->motor_l = 0;
				break;
			case SDLK_2:
				if(!straight) bb->motor_r = 0;
				break;
			case SDLK_3:
				straight = 0;
				bb->motor_l = bb->motor_r = 0;
				break;
			}
	}
	if(straight){
		if(bb->angle > ref_angle + 10){
			bb->motor_l = 0;
			bb->motor_r = 1;
		}
		else if(bb->angle < ref_angle - 10){
			bb->motor_l = 1;
			bb->motor_r = 0;
		}
	}
	clear_window(win_ctx);
	bb_update(bb, (double)1 / FPS);
	draw_bb(win_ctx, bb);
	present_window(win_ctx);
	return 1;
}

int
main(int argc, char **argv){
	buffalo_byte_t bb;
	window_ctx_t win_ctx;

	if(!init_window_ctx(&win_ctx)){
		cleanup_window_ctx(&win_ctx);
		fprintf(stderr, "ERR: Failed to initialize window context\n");
		return 1;
	}
	bb_init(&bb);
	clear_window(&win_ctx);
	while(event_loop(&win_ctx, &bb));
	cleanup_window_ctx(&win_ctx);
	return 0;
}
