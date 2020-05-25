#include "load.h"

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
  default:
    return NULL;
  }
  return entity_create_with_defaults(
                       vector_create(0,0),
                       vector_create(50,50),
                       type
                       );
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

  // checking format
  char *check_buffer = malloc(file_size + 1);
  check_buffer[file_size] = '\0';
  memcpy(check_buffer, content, file_size);
  char *line = strtok(check_buffer, "\n");
  size_t line_number = 0;
  while(line) {
    if (strlen(line) != MAP_WIDTH) {
      free(check_buffer);
      return NULL;
    }
    line = strtok(NULL, "\n");
    line_number++;
  }
  free(check_buffer);
  if (line_number != MAP_HEIGHT) {
    return NULL;
  }

  // building scene object
  struct scene *scene = scene_create(color_create(100,100,100));

  line = strtok(content, "\n");
  line_number = 0;
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
