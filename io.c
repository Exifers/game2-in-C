#include "io.h"

void io_pre_loop_hook() {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window;
  window = SDL_CreateWindow(
			    WINDOW_TITLE,
			    SDL_WINDOWPOS_UNDEFINED,
			    SDL_WINDOWPOS_UNDEFINED,
			    WINDOW_WIDTH,
			    WINDOW_HEIGHT,
			    SDL_WINDOW_OPENGL
			    );
  io_globals_singleton()->window = window;
  io_globals_singleton()->surface = SDL_GetWindowSurface(window);
}

void io_begin_loop_hook() {
  SDL_Event event;
  while(SDL_PollEvent(&event)) {
    globals_add_event(globals_singleton(), event_create(event));
  }
}

void io_end_loop_hook() {
  SDL_UpdateWindowSurface(io_globals_singleton()->window);

  if (globals_has_event(globals_singleton(), SDL_QUIT)) {
    globals_singleton()->quit = true;
  }

  globals_delete_all_events(globals_singleton());
}

void io_post_loop_hook() {
  SDL_DestroyWindow(io_globals_singleton()->window);
  SDL_Quit();
}

struct io_globals *io_globals_singleton() {
  static struct io_globals io_globals;
  static bool set;
  if (!set) {
    memset(&io_globals, '\0', sizeof(struct io_globals));
    set = true;
  }
  return &io_globals;
}

void io_fill_screen(struct color color) {
  SDL_FillRect(io_globals_singleton()->surface, NULL, io_convert_color(color));
}

void io_draw_rect(struct rect rect, struct color color) {
  SDL_Rect sdl_rect = io_convert_rect(rect);
  SDL_FillRect(io_globals_singleton()->surface, &sdl_rect, io_convert_color(color));
}

Uint32 io_convert_color(struct color color) {
  return SDL_MapRGB(
		    io_globals_singleton()->surface->format,
		    color.r,
		    color.g,
		    color.b
		    );
}

SDL_Rect io_convert_rect(struct rect rect) {
  SDL_Rect sdl_rect;
  sdl_rect.x = rect.pos.x;
  sdl_rect.y = rect.pos.y;
  sdl_rect.w = rect.dims.x;
  sdl_rect.h = rect.dims.y;
  return sdl_rect;
}

bool io_key_pressed(SDL_Keycode key) {
  const SDL_Scancode scan_code = SDL_GetScancodeFromKey(key);
  const Uint8 *key_state = SDL_GetKeyboardState(NULL);
  if (key_state[scan_code]) {
    return true;
  }
  return false;
}

