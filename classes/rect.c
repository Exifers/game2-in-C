#include "classes/rect.h"

#include "io.h"
#include "warn.h"

struct rect rect_create(struct vector pos, struct vector dims) {
  struct rect rect;
  rect.pos = pos;
  rect.dims = dims;
  return rect;
}

void rect_dump(struct rect *this) {
  printf("Rect {pos:");
  vector_dump(&this->pos);
  printf(", dims:");
  vector_dump(&this->dims);
  printf("}\n");
}

enum collision rect_collides(struct rect *this, struct rect *other) {
  const SDL_Rect sdl_rect_this = io_convert_rect(*this); 
  const SDL_Rect sdl_rect_other = io_convert_rect(*other);

  if (SDL_RectEquals(&sdl_rect_this, &sdl_rect_other)) {
    // default behaviour for two equals rect colliding each other
    return TOP; 
  }

  SDL_Rect result;

  if (!SDL_IntersectRect(&sdl_rect_this, &sdl_rect_other, &result)) {
    return NONE;
  }

  // result is the structure filled with intersection of rectangle this with
  // rectangle other. It must have at least one common point with other, we'
  // re looking for that common point here.
  if (result.x == sdl_rect_other.x && result.y == sdl_rect_other.y) { 
    if (result.w == result.h) {
      return TOP_LEFT;
    }
    else if (result.w > result.h) {
      return TOP;
    }
    else if (result.w < result.h) {
      return LEFT;
    }
  }
  else if (result.x + result.w == sdl_rect_other.x + sdl_rect_other.w
      && result.y == sdl_rect_other.y) {
    if (result.w == result.h) {
      return TOP_RIGHT;
    }
    else if (result.w > result.h) {
      return TOP;
    }
    else if (result.w < result.h) {
      return RIGHT;
    }
  }
  else if (result.x == sdl_rect_other.x
      && result.y + result.h == sdl_rect_other.y + sdl_rect_other.h) {
    if (result.w == result.h) {
      return BOTTOM_LEFT;
    }
    else if (result.w > result.h) {
      return BOTTOM;
    }
    else if (result.w < result.h) {
      return LEFT;
    }
  }
  else if (result.x + result.w == sdl_rect_other.x + sdl_rect_other.w
  && result.y + result.h == sdl_rect_other.y + sdl_rect_other.h) {
    if (result.w == result.h) {
      return BOTTOM_RIGHT;
    }
    else if (result.w > result.h) {
      return BOTTOM;
    }
    else if (result.w < result.h) {
      return RIGHT;
    }
  }

//  warn(
//    "collision between two rects couldn't have been calculated properly."
//  );
  return NONE;
}
