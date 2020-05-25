#ifndef GLOBALS_H
#define GLOBALS_H

#include <SDL.h>
#include <stdbool.h>
#include <stdlib.h>

#include "classes/event.h"
#include "classes/scene.h"
#include "classes/vector.h"
#include "camera.h"

struct globals {
  bool quit;
  float delta_time;
  struct camera camera;
  struct vector gravity;
  struct scene *scene;
  struct event *event;
  bool show_gizmos;
  bool gravity_enabled;
  bool fly_enabled;
  float horizontal_friction;
};

// singleton
struct globals *globals_singleton();

void globals_add_event(struct globals *this, struct event *event);
void globals_delete_all_events(struct globals *this);
bool globals_has_event(struct globals *this, Uint32 sdl_event);
bool globals_has_keydown_event(struct globals *this, SDL_Keycode key_code);
void globals_update(struct globals *this);

#endif /* GLOBALS_H */
