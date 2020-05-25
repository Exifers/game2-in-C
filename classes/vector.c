#include "math.h"

#include "vector.h"

struct vector vector_create(float x, float y) {
  struct vector vector;
  vector.x = x;
  vector.y = y;
  return vector;
}

struct vector vector_plus(struct vector lhs, struct vector rhs) {
  struct vector vector;
  vector.x = lhs.x + rhs.x;
  vector.y = lhs.y + rhs.y;
  return vector;
}

struct vector vector_times(struct vector this, float amount) {
  struct vector vector;
  vector.x = this.x * amount;
  vector.y = this.y * amount;
  return vector;
}

void vector_dump(struct vector *this) {
  printf("[%f, %f]", this->x, this->y);
}

struct vector vector_normalize(struct vector this) {
  if (this.x == 0 && this.y == 0) {
    return this;
  }
  float mag = sqrtf(this.x * this.x + this.y * this.y);
  return vector_times(this, 1/mag);
}
