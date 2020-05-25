#include "camera.h"
#include "globals.h"
#include "classes/scene.h"

struct camera camera_create(struct vector pos, struct vector vel) {
  struct camera camera;
  camera.pos = pos;
  camera.vel = vel;
  return camera;
}

struct camera camera_create_with_defaults() {
  return camera_create(
    vector_create(0,0),
    vector_create(0,0)
  );
}

struct rect world_to_view(struct camera *this, struct rect rect) {
  rect.pos.x -= this->pos.x;
  rect.pos.y -= this->pos.y;
  return rect;
}

void camera_update(struct camera *this) {
  this->pos = vector_plus(
    this->pos,
    vector_times(
      this->vel,
      globals_singleton()->delta_time
    )
  );
}

void camera_follow_player(struct camera *this) {
  struct entity *player = scene_get_player(globals_singleton()->scene);

  if (!player) {
    return;
  }
  float x = this->pos.x;
  float y = this->pos.y;
  if (player->pos.x + player->dims.x > this->pos.x + WINDOW_WIDTH - CAMERA_FOLLOW_PADDING) {
    x = player->pos.x - (WINDOW_WIDTH - CAMERA_FOLLOW_PADDING) + player->dims.x;
  }
  else if (player->pos.x < this->pos.x + CAMERA_FOLLOW_PADDING) {
    x = player->pos.x - CAMERA_FOLLOW_PADDING;
  }
  if (player->pos.y + player->dims.y > this->pos.y + WINDOW_HEIGHT - CAMERA_FOLLOW_PADDING) {
    y = player->pos.y - (WINDOW_HEIGHT - CAMERA_FOLLOW_PADDING) + player->dims.y;
  }
  else if (player->pos.y < this->pos.y + CAMERA_FOLLOW_PADDING) {
    y = player->pos.y - CAMERA_FOLLOW_PADDING;
  }
  
  if (x < 0) {
    x = 0;
  }
  if (y < 0) {
    y = 0;
  }
  // handle x or y > map wifth and set it to map width
  this->pos = vector_create(
      x, 
      y
      );
}
