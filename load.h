#ifndef LOAD_H
#define LOAD_H

#include <stdio.h>
#include <string.h>

#define MAP_WIDTH 12
#define MAP_HEIGHT 8

#include "classes/scene.h"

struct scene *load_scene(char *filename);

#endif /* LOAD_H */
