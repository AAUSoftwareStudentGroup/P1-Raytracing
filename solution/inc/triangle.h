#ifndef _TRIANGLE_
#define _TRIANGLE_

#include "vector.h"

typedef struct _verticie {
  Vector position;
  Vector normal;
} Vertex;

typedef struct _triangle {
  Vertex *verticies[3];
  Vector edges[3];
} Triangle;

#endif