#include "graphics.h"

#include <stdio.h>

#define WIN_WIDTH 640
#define WIN_HEIGHT 480

bool
init_window_ctx(window_ctx_t *win_ctx){
	win_ctx->win = SDL_CreateWindow(
		"Buffalo Byte Simulator",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		WIN_WIDTH, WIN_HEIGHT,
		SDL_WINDOW_SHOWN
	);
	if(!win_ctx->win){
		fprintf(stderr, "ERR: Failed to create SDL window, SDL_Error %s\n", SDL_GetError());
		return false;
	}
	win_ctx->renderer = SDL_CreateRenderer(win_ctx->win, -1, SDL_RENDERER_ACCELERATED);
	if(!win_ctx->renderer){
		fprintf(stderr, "ERR: Failed to create SDL renderer, SDL_Error %s\n", SDL_GetError());
		return false;
	}
	win_ctx->sdl_surface = SDL_CreateRGBSurface(
		0,
		WIN_WIDTH, WIN_HEIGHT,
		32,
		0xff0000, /* R */
		0x00ff00, /* G */
		0x0000ff, /* B */
		0x000000  /* A */
	);
	win_ctx->cr_surface = cairo_image_surface_create_for_data(
		win_ctx->sdl_surface->pixels,
		CAIRO_FORMAT_RGB24,
		win_ctx->sdl_surface->w,
		win_ctx->sdl_surface->h,
		win_ctx->sdl_surface->pitch
	);
	win_ctx->cr = cairo_create(win_ctx->cr_surface);
	return true;
}

void
cleanup_window_ctx(window_ctx_t *win_ctx){
	if(win_ctx->win)
		SDL_DestroyWindow(win_ctx->win);
	if(win_ctx->renderer)
		SDL_DestroyRenderer(win_ctx->renderer);
}

int
main(int argc, char **argv){
	bool running;
	window_ctx_t win_ctx;
	SDL_Event e;

	init_window_ctx(&win_ctx);
	running = true;
	while(running){
		while(SDL_PollEvent(&e)) if(e.type == SDL_QUIT) running = false;
	}
	cleanup_window_ctx(&win_ctx);
	return 0;
}
