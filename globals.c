#include "globals.h"

static struct globals globals_create() {
  struct globals globals;
  memset(&globals, '\0', sizeof(struct globals));
  globals.quit = false;
  globals.delta_time = 0.1f;
  globals.gravity = vector_create(0,0.1f);
  return globals;
}

struct globals *globals_singleton() {
  static struct globals globals;
  static bool set;
  if (!set) {
    globals = globals_create();
    set = true;
  }
  return &globals;
}

void globals_add_event(struct globals *this, struct event *event) {
  struct event *cur = this->event;
  if (!cur) {
    this->event = event;
    return;
  }
  if (!cur->next) {
    this->event->next = event;
    return;
  }
  while(cur->next) {
    cur = cur->next;
  }
  cur->next = event;
}

void globals_delete_all_events(struct globals *this) {
  struct event *cur = this->event;
  while(cur) {
    struct event *next = cur->next;
    event_free(cur);
    cur = next;
  }
  this->event = NULL;
}

bool globals_has_event(struct globals *this, Uint32 sdl_event) {
  struct event *event = this->event;
  while(event) {
    if (event->event.type == sdl_event) {
      return true;
    }
    event = event->next;
  }
  return false;
}
