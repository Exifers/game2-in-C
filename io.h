#ifndef IO_H
#define IO_H

#include <SDL.h>
#include <stdbool.h>
#include <string.h>

#include "globals.h"
#include "classes/color.h"
#include "classes/rect.h"
#include "classes/event.h"

#define WINDOW_TITLE "Fuzzy"
#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 400

// state and events
bool io_key_pressed(SDL_Keycode key);

struct io_globals {
  SDL_Window *window;
  SDL_Surface *surface;
};

struct io_globals *io_globals_singleton();

void io_pre_loop_hook();
void io_begin_loop_hook();
void io_end_loop_hook();
void io_post_loop_hook();

void io_fill_screen(struct color color);
void io_draw_rect(struct rect rect, struct color color);

void *io_read_file_content(char *filename);

// converters
Uint32 io_convert_color(struct color color);
SDL_Rect io_convert_rect(struct rect rect);
struct rect io_convert_rect_back(SDL_Rect sdl_rect);

#endif /* IO_H */
