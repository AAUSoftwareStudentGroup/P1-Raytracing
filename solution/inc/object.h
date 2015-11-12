#ifndef _OBJECT_
#define _OBJECT_

#include <stdlib.h>

#include "vector.h"
#include "material.h"
#include "pixel.h"

typedef struct _triangle {
  Vector *verticies[3];
} Triangle;

typedef struct _object {
  Vector *verticies;
  int n_verticies;
  Triangle *triangles;
  int n_triangles;
  Pixel color;
  Material material;
} Object;

Object *new_object(void);
int free_object(Object *object);

#endif