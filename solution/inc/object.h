#ifndef _OBJECT_
#define _OBJECT_

#include <stdlib.h>

#include "vector.h"
#include "material.h"
#include "pixel.h"

typedef struct _verticie {
  Vector position;
  Vector normal;
} Vertex;

typedef struct _triangle {
  Vertex *verticies[3];
} Triangle;

typedef struct _sphere {
  Vector center;
  double radius;
} Sphere;

typedef struct _object {
  Vertex *verticies;
  int n_verticies;
  Triangle *triangles;
  int n_triangles;
  Pixel color;
  Material material;
  Sphere bounding_volume;
} Object;

Object *new_object(void);
int free_object(Object *object);

#endif