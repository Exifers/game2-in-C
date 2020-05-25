#include "classes/color.h"

struct color color_create(float r, float g, float b) {
  struct color color;
  color.r = r;
  color.g = g;
  color.b = b;
  return color;
}
