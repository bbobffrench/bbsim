#include "graphics.h"

#include <stdio.h>
#include <math.h>

#define WIN_WIDTH 1200
#define WIN_HEIGHT 720

/* For translating BB coordinates to Cairo coordinates */
#define PX_OFFSET_X WIN_WIDTH / 2
#define PX_OFFSET_Y WIN_HEIGHT / 2
#define PX_SCALE 1000 

/* Size of the BB graphical representation */
#define BB_WIDTH 25

const unsigned fg_rgb[3] = {0xff, 0xff, 0xff};
const unsigned bg_rgb[3] = {0x00, 0x00, 0x00};

char
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
		return 0;
	}
	win_ctx->renderer = SDL_CreateRenderer(win_ctx->win, -1, SDL_RENDERER_ACCELERATED);
	if(!win_ctx->renderer){
		fprintf(stderr, "ERR: Failed to create SDL renderer, SDL_Error: %s\n", SDL_GetError());
		return 0;
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
	return 1;
}

void
cleanup_window_ctx(const window_ctx_t *win_ctx){
	if(win_ctx->cr_surface) cairo_surface_destroy(win_ctx->cr_surface);
	if(win_ctx->cr) cairo_destroy(win_ctx->cr);
	if(win_ctx->sdl_surface) SDL_FreeSurface(win_ctx->sdl_surface);
	if(win_ctx->renderer) SDL_DestroyRenderer(win_ctx->renderer);
	if(win_ctx->win)SDL_DestroyWindow(win_ctx->win);
	SDL_Quit();
}

void
present_window(const window_ctx_t *win_ctx){
	SDL_Texture *texture;

	cairo_surface_flush(win_ctx->cr_surface);
	texture = SDL_CreateTextureFromSurface(win_ctx->renderer, win_ctx->sdl_surface);
	SDL_RenderCopy(win_ctx->renderer, texture, NULL, NULL);
	SDL_RenderPresent(win_ctx->renderer);
	SDL_DestroyTexture(texture);
}

void
clear_window(const window_ctx_t *win_ctx){
	cairo_set_source_rgb(win_ctx->cr, bg_rgb[0], bg_rgb[1], bg_rgb[2]);
	cairo_paint(win_ctx->cr);
}

void
draw_bb(const window_ctx_t *win_ctx, const buffalo_byte_t *bb){
	double x_transl = (PX_OFFSET_X + (bb->x * PX_SCALE)) + BB_WIDTH / 2;
	double y_transl = (PX_OFFSET_Y + (-bb->y * PX_SCALE)) + BB_WIDTH / 2;

	cairo_save(win_ctx->cr);
	cairo_set_source_rgb(win_ctx->cr, fg_rgb[0], fg_rgb[1], fg_rgb[2]);
	cairo_translate(win_ctx->cr, x_transl, y_transl);
	cairo_rotate(win_ctx->cr, bb->angle * M_PI / 180);
	cairo_rectangle(win_ctx->cr, 0, 0, BB_WIDTH, BB_WIDTH);
	cairo_fill(win_ctx->cr);
	cairo_restore(win_ctx->cr);
	return;
}
