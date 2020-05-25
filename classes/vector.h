#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>

struct vector {
  float x;
  float y;
};

struct vector vector_create(float x, float y);

struct vector vector_plus(struct vector lhs, struct vector rhs);
struct vector vector_times(struct vector this, float amount);
struct vector vector_normalize(struct vector this);

void vector_dump(struct vector *this);

#endif /* VECTOR_H */
