#ifndef COLOR_H
#define COLOR_H

struct color {
  float r;
  float g;
  float b;
};

struct color color_create(float r, float g, float b);

#endif /* COLOR_H */
