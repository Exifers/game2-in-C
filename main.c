#include <stdbool.h>
#include <stdio.h>
#include <SDL.h>

#include "classes/color.h"
#include "globals.h"
#include "io.h"
#include "load.h"

int main(void) {
  io_pre_loop_hook();

  struct scene *scene = load_next_scene("maps/1.map");
  if (!scene) {
    return -1;
  }
  globals_singleton()->scene = scene;

  while(!globals_singleton()->quit) {
    io_begin_loop_hook();

    scene_update(globals_singleton()->scene);
    scene_draw(globals_singleton()->scene);

    io_end_loop_hook();
  }

  io_post_loop_hook();
}
