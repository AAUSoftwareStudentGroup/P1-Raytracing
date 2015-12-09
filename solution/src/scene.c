#include "scene.h"

Scene *new_scene(void) {
  Scene *scene = (Scene*)malloc(sizeof(Scene));
  return scene;
}

int free_scene(Scene *scene) {
  int i;

  free(scene->objects[0]->verticies);
  free(scene->objects[0]->triangles);
  
  for(i = scene->n_objects-1; i >= 0; i--) {
    free_object(scene->objects[i]);
  }
  free(scene->objects);

  for(i = 0; i < scene->n_lights; i++) {
    free_pointlight(scene->lights[i]);
  }
  free(scene->lights);

  free(scene);
  return 1;
}