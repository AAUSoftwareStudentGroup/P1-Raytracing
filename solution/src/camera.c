#include "camera.h"

Camera *new_camera(unsigned int width, unsigned int height) {
  Camera *camera;
  camera = (Camera*)malloc(sizeof(Camera));
  camera->width = width;
  camera->height = height;
  camera->position = (Vector){0,0,0};
  camera->up = (Vector){0,1,0};
  camera->right = (Vector){1,0,0};
  camera->forward = (Vector){0,0,-1};
  camera->distance = width;

  /* Initialiser resten af camera variablerne */
  
  return camera; 
}

int free_camera(Camera *camera) {
  free(camera);
  return 1;
}

int camera_look_at_point(Camera *camera, Vector *point) {
  return 1;
}

int camera_rotate_horizontal(Camera *camera, double angle) {
  return 1;
}

int camera_rotate_vertical(Camera *camera, double angle) {
  return 1;
}