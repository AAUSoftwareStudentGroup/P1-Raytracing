#ifndef _CAMERA_
#define _CAMERA_

#include "vector.h"

typedef struct _camera {
  Vector *up;
  Vector *right;
  Vector *forward;
  Vector *position;
  unsigned int width, height;
  double distance;
} Camera;

int camera_look_at_point(Camera *cam, vector *point);
int camera_rotate_vertical(Camera *cam, double angle);
int camera_rotate_horizontal(Camera *cam, double angle);

#endif