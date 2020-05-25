#ifndef SCENE_H
#define SCENE_H

#include <stdlib.h>

#include "classes/entity.h"
#include "io.h"

struct scene {
  struct color background;
  struct entity *entity;
};

struct scene *scene_create(struct color background);
void scene_free(struct scene *this);
void scene_update(struct scene *this);
void scene_draw(struct scene *this);
void scene_add_entity(struct scene *this, struct entity *entity);
struct entity *scene_get_player(struct scene *this);

#endif /* SCENE_H */
