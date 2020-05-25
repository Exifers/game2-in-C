#ifndef RECT_H
#define RECT_H

#include <stdio.h>
#include <SDL.h>

#include "classes/vector.h"

struct rect {
  struct vector pos;
  struct vector dims;
};

enum collision {
  TOP,
  TOP_LEFT,
  LEFT,
  BOTTOM_LEFT,
  BOTTOM,
  BOTTOM_RIGHT,
  RIGHT,
  TOP_RIGHT,
  NONE
};

struct rect rect_create(struct vector pos, struct vector dims);
void rect_dump(struct rect *this);

/* checks if the rect collides with another rect.
 * It returns an enum collision based on which edge or which corner it has
 * reached on other.
 */
enum collision rect_collides(struct rect *this, struct rect *other);

#endif /* RECT_H */
