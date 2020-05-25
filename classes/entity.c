#include "classes/entity.h"

struct entity *entity_create(
			     struct vector pos,
			     struct vector vel,
			     struct vector dims,
			     struct color color,
			     enum entity_type type,
           bool grounded
			     ) {
  struct entity *entity = malloc(sizeof(struct entity));
  entity->pos = pos;
  entity->vel = vel;
  entity->dims = dims;
  entity->color = color;
  entity->type = type;
  entity->next = NULL;
  entity->grounded = false;
  return entity;
}

struct entity *entity_create_with_defaults(
    struct vector pos,
    struct vector dims,
    enum entity_type type
    ) {
  struct color color;
  switch(type) {
    case PLAYER:
      color = color_create(255,0,0);
      break;
    case WALL:
      color = color_create(0,0,0);
      break;
    case ENEMY:
      color = color_create(100,255,200);
      break;
    default:
      color = color_create(255,255,255);
      break;
  }
  struct vector vel = vector_create(0,0);
  bool grounded = false;
  return entity_create(
      pos,
      vel,
      dims,
      color,
      type,
      grounded
      );
}

void entity_free(struct entity *this) {
  free(this);
}

void entity_update(struct entity *this) {
  switch(this->type) {
    case PLAYER:
      entity_update_player(this);
      break;
    case ENEMY:
      entity_update_enemy(this);
    default:
      break;
  }
  if (this->type != WALL) {
    entity_apply_gravity(this);
    entity_set_gounded(this);
    entity_handle_collisions(this);
    entity_apply_vel(this);
  }
}

void entity_set_gounded(struct entity *this) {
  struct entity *copy = entity_copy(this);
  entity_step_time(copy);

  struct entity *cur = globals_singleton()->scene->entity;
  while (cur) {
    if (cur == this) {
      cur = cur->next;
      continue;
    }
    enum collision collision = entity_collides(copy, cur);
    if (collision == TOP) {
      this->grounded = true;
      return;
    }
    cur = cur->next;
  }
  this->grounded = false;
}

void entity_update_enemy(struct entity *this) {

}

void entity_update_player(struct entity *this) {
  struct vector vel = this->vel;
  float move_factor = 0.01;
  float jump_factor = 5;
  if (io_key_pressed(SDLK_RIGHT)) {
    vel = vector_plus(vel, vector_create(move_factor, 0));
  }
  if (io_key_pressed(SDLK_LEFT)) {
    vel = vector_plus(vel, vector_create(-move_factor,0));
  }
  if (io_key_pressed(SDLK_SPACE) && this->grounded) {
    vel = vector_plus(vel, vector_create(0, -jump_factor));
  }
  this->vel = vel;
}

struct entity *entity_copy(struct entity *this) {
  struct entity *copy = entity_create(
      this->pos,
      this->vel,
      this->dims,
      this->color,
      this->type,
      this->grounded
      );
  return copy;
}

void entity_step_time(struct entity *this) {
  entity_apply_gravity(this);
  entity_apply_vel(this);
}

void entity_apply_gravity(struct entity *this) {
  this->vel = vector_plus(
      this->vel,
      vector_times(
        globals_singleton()->gravity,
        globals_singleton()->delta_time
        )
      );
}

void entity_apply_vel(struct entity *this) {
  this->pos = vector_plus(
      this->pos, 
      vector_times(
        this->vel,
        globals_singleton()->delta_time
        )
      );
}

void entity_draw(struct entity *this) {
  struct rect rect;
  rect.pos = this->pos;
  rect.dims = this->dims;
  io_draw_rect(rect, this->color);
}

enum collision entity_collides(struct entity *this, struct entity *other) {
  struct rect this_rect = entity_to_rect(this);
  struct rect other_rect = entity_to_rect(other);
  return rect_collides(&this_rect, &other_rect);
}

struct rect entity_to_rect(struct entity *this) {
  struct rect rect;
  rect.pos = this->pos;
  rect.dims = this->dims;
  return rect;
}

void entity_handle_collisions(struct entity *this) {
  // we copy the entity and apply step_time to it to see if it will collide
  // on the next frame
  struct entity *copy = entity_copy(this);
  entity_step_time(copy);

  struct entity *cur = globals_singleton()->scene->entity;
  struct vector bump = vector_create(0,0);
  while(cur) {
    if (cur == this) {
      cur = cur->next;
      continue;
    }
    if (cur->type != WALL) {
      cur = cur->next;
      continue;
    }
    enum collision collision = entity_collides(copy, cur);
    if (collision != NONE) {
      switch(collision) {
        case TOP:
          this->vel = vector_create(
            this->vel.x,
            0
          );
          break;
        case TOP_LEFT:
          this->vel = vector_create(0,0);
          break;
        case LEFT:
          this->vel = vector_create(
            0,
            this->vel.y
          ); 
          break;
        case BOTTOM_LEFT:
          this->vel = vector_create(0,0);
          break;
        case BOTTOM:
          this->vel = vector_create(
            this->vel.x,
            0
          );
          break;
        case BOTTOM_RIGHT:
          this->vel = vector_create(0,0);
          break;
        case RIGHT:
          this->vel = vector_create(
            0,
            this->vel.y
          ); 
          break;
        case TOP_RIGHT:
          this->vel = vector_create(0,0);
          break;
        default:
          break;
      }
    }
    cur = cur->next;
  }
  entity_free(copy);
}