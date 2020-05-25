#ifndef GLOBALS_H
#define GLOBALS_H

#include <SDL.h>
#include <stdbool.h>
#include <stdlib.h>

#include "classes/event.h"
#include "classes/scene.h"
#include "classes/vector.h"

struct globals {
  bool quit;
  float delta_time;
  struct vector gravity;
  struct scene *scene;
  struct event *event;
};

// singleton
struct globals *globals_singleton();

void globals_add_event(struct globals *this, struct event *event);
void globals_delete_all_events(struct globals *this);
bool globals_has_event(struct globals *this, Uint32 sdl_event);

#endif /* GLOBALS_H */
