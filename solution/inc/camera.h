#ifndef _CAMERA_
#define _CAMERA_

#include <stdlib.h>

#include "vector.h"

typedef struct _camera {
  Vector up;
  Vector right;
  Vector forward;
  Vector position;
  unsigned int width, height;
  double distance;
} Camera;

Camera *new_camera(unsigned int width, unsigned int height);
int free_camera(Camera *camera);

int camera_look_at_point(Camera *camera, Vector *point);
int camera_set_angle(Camera *camera, double vertical_angle, double horizontal_angle);


#endif