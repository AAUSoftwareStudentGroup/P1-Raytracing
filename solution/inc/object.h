#ifndef _OBJECT_
#define _OBJECT_

#include <stdlib.h>

#include "vector.h"
#include "material.h"
#include "pixel.h"
#include "kdnode.h"
#include "triangle.h"

typedef struct _object {
  Vertex *verticies;
  int n_verticies;
  Triangle *triangles;
  int n_triangles;
  Pixel color;
  Material material;
  KDNode root;
} Object;

int object_is_point_inside_aabb(Vector v, AABB box);
int object_is_triangle_in_aabb(Triangle t, AABB box);
Object *new_object(void);
int free_object(Object *object);

#endif