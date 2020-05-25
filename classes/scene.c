#include "classes/scene.h"

struct scene *scene_create(struct color background) {
  struct scene *scene = malloc(sizeof(struct scene));
  scene->background = background;
  scene->entity = NULL;
  return scene;
}

void scene_free(struct scene *this) {
  struct entity *cur = this->entity;
  while(cur) {
    struct entity *next = cur->next;
    free(cur);
    cur = next;
  }
  this->entity = NULL;
}

void scene_update(struct scene *this) {
  io_fill_screen(this->background);

  struct entity *cur = this->entity;
  while(cur) {
    entity_update(cur);
    cur = cur->next;
  }
}

void scene_draw(struct scene *this) {
  struct entity *cur = this->entity;

  while (cur) {
    entity_draw(cur);
    cur = cur->next;
  }
}


void scene_add_entity(struct scene *this, struct entity *entity) {
  struct entity *cur = this->entity;

  if (!cur) {
    this->entity = entity;
    return;
  }

  while(cur->next) {
    cur = cur->next;
  }

  cur->next = entity;
}
