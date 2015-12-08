#ifndef _PLANE_
#define _PLANE_

#include "vector.h"

typedef struct _plane {
  Vector normal;
  Vector point;
} Plane;

Plane create_plane(Vector point, Vector normal);

#endif