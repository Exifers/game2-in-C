#include <stdbool.h>
#include <stdio.h>
#include <SDL.h>

#include "classes/color.h"
#include "globals.h"
#include "io.h"
#include "load.h"

int main(void) {
  io_pre_loop_hook();

  struct scene *scene = load_scene("maps/1.map");
  if (!scene) {
    return -1;
  }
  globals_singleton()->scene = scene;

  while(!globals_singleton()->quit) {
    io_begin_loop_hook();

    scene_update(scene);
    scene_draw(scene);

    io_end_loop_hook();
  }

  scene_free(scene);

  io_post_loop_hook();
}
