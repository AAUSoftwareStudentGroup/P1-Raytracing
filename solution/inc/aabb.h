#ifndef _AABB_
#define _AABB_

#include "vector.h"
#include "triangle.h"

typedef struct _AABB {
  Vector low, high;
} AABB;

int aabb_is_point_inside(Vector v, AABB box);
int aabb_is_triangle_in(Triangle t, AABB box);

#endif