#ifndef _RAY_
#define _RAY_

#include "vector.h"

typedef struct _ray {
  Vector initial_point, direction;
} Ray;

Ray create_ray(Vector p1, Vector p2);
Vector ray_get_point(Ray ray, double t);

#endif