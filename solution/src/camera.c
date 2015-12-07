#include "camera.h"

Camera *new_camera(unsigned int width, unsigned int height) {
  Camera *camera;
  camera = (Camera*)malloc(sizeof(Camera));
  camera->width    = width;
  camera->height   = height;
  camera->position = (Vector){0,0,0};
  camera->right    = (Vector){1,0,0};
  camera->forward  = (Vector){0,1,0};
  camera->up       = (Vector){0,0,1};
  camera->distance = width;

  /* Initialiser resten af camera variablerne */
  
  return camera; 
}

int free_camera(Camera *camera) {
  free(camera);
  return 1;
}
/*
int camera_look_at_point(Camera *camera, Vector *point) {
  return 1;
}
*/
void camera_look_at_point(Camera *camera, Vector *point, double distance, double vertical_angle, double horizontal_angle) {

  camera->position = (Vector){0, distance, 0};
  camera_set_angle(camera, vertical_angle, horizontal_angle);
  
  camera->right   = vector_scale( camera->right, -1);
  camera->forward = vector_scale( camera->forward, -1);
  camera->up      = vector_scale( camera->up, -1);

  camera->position = vector_rotate_around_xz(camera->position, vertical_angle, horizontal_angle);
  camera->position = vector_add(camera->position, *point);
}

int camera_set_angle(Camera *camera, double vertical_angle, double horizontal_angle){
  camera->right   = (Vector){1,0,0};
  camera->forward = (Vector){0,1,0};
  camera->up      = (Vector){0,0,1};

  /* Rotating up and forward vector */
  camera->up      = vector_rotate_around_xz(camera->up,      vertical_angle, horizontal_angle);
  camera->forward = vector_rotate_around_xz(camera->forward, vertical_angle, horizontal_angle);

  camera->right   = vector_rotate_around_z(camera->right,   horizontal_angle);
  
  return 1;
}