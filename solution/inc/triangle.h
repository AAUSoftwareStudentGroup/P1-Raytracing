#ifndef _TRIANGLE_
#define _TRIANGLE_

#include "vector.h"

#define VERTICES_IN_TRIANGLE 3

typedef struct _vertex {
  Vector position;
  Vector normal;
} Vertex;

typedef struct _triangle {
  Vertex *verticies[VERTICES_IN_TRIANGLE];
  Vector edges[VERTICES_IN_TRIANGLE];
} Triangle;

#endif
