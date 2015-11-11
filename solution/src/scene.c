#include "scene.h"

Scene *new_scene(void) {
  return (Scene*)malloc(sizeof(Scene));
}

int free_scene(Scene *scene) {
  free(scene);
  return 1;
}