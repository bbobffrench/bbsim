#include "graphics.h"

#include <stdio.h>

#define WIN_WIDTH 1200
#define WIN_HEIGHT 720

bool
init_window_ctx(window_ctx_t *win_ctx){
	win_ctx->win = NULL;
	win_ctx->renderer = NULL;
	win_ctx->sdl_surface = NULL;
	win_ctx->cr_surface = NULL;
	win_ctx->cr = NULL;
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		fprintf(stderr, "ERR: Failed to initialize SDL, SDL_Error: %s", SDL_GetError());
		return 0;
	}
	win_ctx->win = SDL_CreateWindow(
		"Buffalo Byte Simulator",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		WIN_WIDTH, WIN_HEIGHT,
		SDL_WINDOW_SHOWN
	);
	if(!win_ctx->win){
		fprintf(stderr, "ERR: Failed to create SDL window, SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	win_ctx->renderer = SDL_CreateRenderer(win_ctx->win, -1, SDL_RENDERER_ACCELERATED);
	if(!win_ctx->renderer){
		fprintf(stderr, "ERR: Failed to create SDL renderer, SDL_Error: %s\n", SDL_GetError());
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
	if(win_ctx->cr_surface) cairo_surface_destroy(win_ctx->cr_surface);
	if(win_ctx->cr) cairo_destroy(win_ctx->cr);
	if(win_ctx->sdl_surface) SDL_FreeSurface(win_ctx->sdl_surface);
	if(win_ctx->renderer) SDL_DestroyRenderer(win_ctx->renderer);
	if(win_ctx->win)SDL_DestroyWindow(win_ctx->win);
	SDL_Quit();
}

int
main(int argc, char **argv){
	bool running;
	window_ctx_t win_ctx;
	SDL_Texture *texture;
	SDL_Event e;

	init_window_ctx(&win_ctx);
	cairo_set_source_rgb(win_ctx.cr, 0xFF, 0xFF, 0xFF);
	cairo_rectangle(win_ctx.cr, 20, 20, 50, 50);
	cairo_fill(win_ctx.cr);
	cairo_surface_flush(win_ctx.cr_surface);
	texture = SDL_CreateTextureFromSurface(win_ctx.renderer, win_ctx.sdl_surface);
	SDL_RenderCopy(win_ctx.renderer, texture, NULL, NULL);
	SDL_RenderPresent(win_ctx.renderer);

	running = true;
	while(running){
		while(SDL_PollEvent(&e)) if(e.type == SDL_QUIT) running = false;
	}
	cleanup_window_ctx(&win_ctx);
	return 0;
}
