#include "camera.h"

Camera *new_camera(unsigned int width, unsigned int height) {
  Camera *camera;
  camera = (Camera*)malloc(sizeof(Camera));
  camera->width = width;
  camera->height = height;
  camera->position = (Vector){0,0,0};
  camera->up = (Vector){0,0,1};
  camera->right = (Vector){1,0,0};
  camera->forward = (Vector){0,1,0};
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

int camera_set_angle(Camera *camera, double vertical_angle, double horizontal_angle){
  camera->up = (Vector){0,0,1};
  camera->right = (Vector){1,0,0};
  camera->forward = (Vector){0,1,0};
  
  /* Rotating up and forward vector vertically */
  camera->up = vector_rotate_around_x(camera->up, vertical_angle);
  camera->forward = vector_rotate_around_x(camera->forward, vertical_angle);
  
  /* Rotating up, forward and right vector horizontally */
  camera->up = vector_rotate_around_z(camera->up, horizontal_angle);
  camera->forward = vector_rotate_around_z(camera->forward, horizontal_angle);
  camera->right = vector_rotate_around_z(camera->right, horizontal_angle);
  
  return 1;
}