#ifndef _PLANE_
#define _PLANE_

#include "vector.h"

typedef struct _plane {
  Vector point;
  Vector normal;
} Plane;

Plane create_plane(Vector point, Vector normal);

#endif