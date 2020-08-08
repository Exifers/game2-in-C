#include "classes/entity.h"
#include "camera.h"

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
  entity->on_wall_left = false;
  entity->on_wall_right = false;
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
    case DOOR:
      color = color_create(255,255,255);
      dims = vector_create(50,100);
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
      entity_handle_player_encounters_door(this);
      break;
    case ENEMY:
      entity_update_enemy(this);
    default:
      break;
  }
  if (this->type != WALL && this->type != DOOR) {
    if (globals_singleton()->gravity_enabled) {
      entity_apply_gravity(this);
    }
    entity_apply_horizontal_friction(this);
    entity_set_grounded(this);
    entity_set_on_wall(this);
    entity_handle_collisions(this);
    entity_apply_vel(this);
  }
}

void entity_set_grounded(struct entity *this) {
  struct entity *copy = entity_copy(this);
  entity_step_time(copy);

  struct entity *cur = globals_singleton()->scene->entity;
  while (cur) {
    if (cur == this || cur->type != WALL) {
      cur = cur->next;
      continue;
    }
    enum collision collision = entity_collides(copy, cur);
    if (collision == TOP) {
      this->grounded = true;
      entity_free(copy);
      return;
    }
    cur = cur->next;
  }
  this->grounded = false;
  entity_free(copy);
}

void entity_handle_player_encounters_door(struct entity *this) {
  struct entity *cur = globals_singleton()->scene->entity;
  while(cur) {
    if (cur->type != DOOR) {
      cur = cur->next;
      continue;
    }
    if (entity_collides(this, cur) != NONE) {
      // scene_free(globals_singleton()->scene);
      struct scene *next_scene = load_next_scene();
      if (next_scene) {
        globals_singleton()->scene = next_scene;
      }
      return;
    }
    cur = cur->next;
  }
}

void entity_set_on_wall(struct entity *this) {
  // copy for testing for on_wall_right
  struct entity *copy_r = entity_copy(this);
  copy_r->vel = vector_create(1,0);

  // copy  for testing for on_wall_left
  struct entity *copy_l = entity_copy(this);
  copy_l->vel = vector_create(-1,0);

  entity_step_time(copy_r);
  entity_step_time(copy_l);

  this->on_wall_right = false;
  this->on_wall_left = false;

  struct entity *cur = globals_singleton()->scene->entity;
  while (cur) {
    if (cur == this || cur->type != WALL) {
      cur = cur->next;
      continue;
    }
    enum collision collision_r = entity_collides(copy_r, cur);
    enum collision collision_l = entity_collides(copy_l, cur);
    if (collision_l == RIGHT
        || collision_l == TOP_RIGHT
        || collision_l == BOTTOM_RIGHT) {
      this->on_wall_left = true;
    }
    if (collision_r == LEFT
        || collision_r == TOP_LEFT
        || collision_r == BOTTOM_LEFT) {
      this->on_wall_right = true;
    }
    cur = cur->next;
  }
  entity_free(copy_r);
  entity_free(copy_l);
}

void entity_update_enemy(struct entity *this) {

}

void entity_update_player(struct entity *this) {
  struct vector vel = this->vel;
  float move_factor = 0.03;
  float jump_factor = 5;
  if (io_key_pressed(SDLK_RIGHT)) {
    // go right
    vel = vector_plus(vel, vector_create(move_factor, 0));
  }
  if (io_key_pressed(SDLK_LEFT)) {
    // go left
    vel = vector_plus(vel, vector_create(-move_factor,0));
  }
  if (globals_has_keydown_event(globals_singleton(), SDLK_UP)
      && globals_singleton()->fly_enabled) {
    // fly up
    vel = vector_plus(vel, vector_create(0,-move_factor));
  }
  if (globals_has_keydown_event(globals_singleton(), SDLK_DOWN)
      && globals_singleton()->fly_enabled) {
    // fly down
    vel = vector_plus(vel, vector_create(0,move_factor));
  }
  if (io_key_pressed(SDLK_SPACE) && this->grounded) {
    // jump
    vel = vector_plus(vel, vector_create(0, -jump_factor));
  }
  if (globals_has_keydown_event(globals_singleton(), SDLK_SPACE)
      && !this->grounded && (this->on_wall_left || this->on_wall_right)) {
    // wall jump
    vel = entity_get_wall_jump_vel(this);
  }
  this->vel = vel;
}

struct vector entity_get_wall_jump_vel(struct entity *this) {
  struct vector wall_jump_vel = vector_create(0,0);
  float wall_jump_factor_x = 7;
  float wall_jump_factor_y = -3;
  if (this->on_wall_left) {
    wall_jump_vel = vector_plus(
      wall_jump_vel,
      vector_create(wall_jump_factor_x, wall_jump_factor_y)
      );
  }
  if (this->on_wall_right) {
    wall_jump_vel = vector_plus(
      wall_jump_vel,
      vector_create(-wall_jump_factor_x, wall_jump_factor_y)
      );
  }
  return wall_jump_vel;
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
  if (globals_singleton()->gravity_enabled) {
    entity_apply_gravity(this);
  }
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

void entity_apply_horizontal_friction(struct entity *this) {
  this->vel = vector_create(
    this->vel.x * (1 - globals_singleton()->horizontal_friction),
    this->vel.y
  );
}

void entity_draw(struct entity *this) {
  struct rect rect;
  rect.pos = this->pos;
  rect.dims = this->dims;
  struct color color = this->color;
  struct rect view_rect = world_to_view(&globals_singleton()->camera, rect);
  io_draw_rect(view_rect, color);
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
          break;
        case LEFT:
          this->vel = vector_create(
            0,
            this->vel.y
          );
          break;
        case BOTTOM_LEFT:
          break;
        case BOTTOM:
          this->vel = vector_create(
            this->vel.x,
            0
          );
          break;
        case BOTTOM_RIGHT:
          break;
        case RIGHT:
          this->vel = vector_create(
            0,
            this->vel.y
          );
          break;
        case TOP_RIGHT:
          break;
        default:
          break;
      }
    }
    cur = cur->next;
  }
  entity_free(copy);
}
