#include "load.h"
#include <stdio.h>

struct entity *character_to_entity(char c) {
  enum entity_type type;
  switch (c) {
    case 'X':
      type = WALL;
      break;
    case 'p':
      type = PLAYER;
      break;
    case 'v':
      type = ENEMY;
      break;
    case 'D':
      type = DOOR;
      break;
    default:
      return NULL;
  }
  return entity_create_with_defaults(
      vector_create(0,0),
      vector_create(50,50),
      type
      );
}

struct scene *load_next_scene() {
  static int last_map_id;

  struct scene *scene = NULL;
  char filename[20];
  while (!scene) {
    sprintf(filename, "maps/%d.map", last_map_id++);
    scene = load_scene(filename);
    if (last_map_id > 100) {
      return NULL;
    }
  }
  return scene;
}

struct scene *load_scene(char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    return NULL;
  }

  // reading content
  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *content = malloc(file_size + 1);
  content[file_size] = '\0';
  fread(content, 1, file_size, file);
  fclose(file);

  // building scene object
  struct scene *scene = scene_create(color_create(100,100,100));

  char *line = strtok(content, "\n");
  size_t line_number = 0;
  while(line) {
    char *c = line;
    size_t column_number = 0;
    while(*c) {
      struct entity *entity = character_to_entity(*c);
      if (entity) {
        entity->pos = vector_create(50 * column_number, 50 * line_number);
        scene_add_entity(scene, entity);
      }
      c++;
      column_number++;
    }
    line = strtok(NULL, "\n");
    line_number++;
  }
  return scene;
}
