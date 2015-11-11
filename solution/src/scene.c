#include "scene.h"

Scene *new_scene(void) {
  Scene *scene = (Scene*)malloc(sizeof(Scene));
  return scene;
}

int free_scene(Scene *scene) {
  free(scene);
  return 1;
}