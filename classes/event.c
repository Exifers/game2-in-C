#include "classes/event.h"

struct event *event_create(SDL_Event sdl_event) {
  struct event *event = malloc(sizeof(struct event));
  memset(event, '\0', sizeof(struct event));
  event->event = sdl_event;
  event->next = NULL;
  return event;
}

void event_free(struct event *this) {
  free(this);
}
