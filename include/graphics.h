#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>
#include <cairo/cairo.h>

#include <stdbool.h>

typedef struct window_ctx_t{
	SDL_Window            *win;
	SDL_Renderer          *renderer;
	SDL_Surface           *sdl_surface;
	cairo_surface_t       *cr_surface;
	cairo_t               *cr;
} window_ctx_t;

bool init_window_ctx(window_ctx_t *);
void cleanup_window_ctx(const window_ctx_t *);
void present_window(const window_ctx_t *);
void clear_window(const window_ctx_t *);

#endif
