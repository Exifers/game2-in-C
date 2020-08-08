#ifndef CAMERA_H
#define CAMERA_H

#include "classes/vector.h"

struct camera {
  struct vector pos;
  struct vector vel;
};

struct rect world_to_view(struct camera *this, struct rect rect);
struct camera camera_create(struct vector pos, struct vector vel);
struct camera camera_create_with_defaults();
void camera_update(struct camera *this);
void camera_follow_player(struct camera *this);

#define CAMERA_FOLLOW_PADDING 100

#endif /* CAMERA_H */
