#ifndef ENTITY_H
#define ENTITY_H

#include <stdlib.h>

#include "classes/vector.h"
#include "classes/color.h"
#include "io.h"
#include "classes/rect.h"

enum entity_type {
		  WALL,
		  PLAYER,
		  ENEMY,
      AIR
};

struct entity {
  struct vector pos;
  struct vector vel;
  struct vector dims;
  struct color color;
  enum entity_type type;
  bool grounded;
  struct entity *next;
};

struct entity *entity_create(
			     struct vector pos,
			     struct vector vel,
			     struct vector dims,
			     struct color color,
			     enum entity_type type,
           bool grounded
			     );

struct entity *entity_create_with_defaults(
                            struct vector pos,
                            struct vector dims,
                            enum entity_type type
                            );

void entity_free(struct entity *this);
void entity_update(struct entity *this);
void entity_draw(struct entity *this);
void entity_update_enemy(struct entity *this);
void entity_update_player(struct entity *this);
struct entity *entity_copy(struct entity *this);
void entity_step_time(struct entity *this);
void entity_handle_collisions(struct entity *this);
enum collision entity_collides(struct entity *this, struct entity *other);
struct rect entity_to_rect(struct entity *this);
void entity_apply_gravity(struct entity *this);
void entity_apply_vel(struct entity *this);
void entity_set_gounded(struct entity *this);

#endif /* ENTITY_H */
