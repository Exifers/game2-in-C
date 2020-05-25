#ifndef EVENT_H
#define EVENT_H

#include <SDL.h>

struct event {
  SDL_Event event;
  struct event *next;
};

struct event *event_create(SDL_Event event);
void event_free(struct event *this);

#endif /* EVENT_H */
