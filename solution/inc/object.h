#ifndef _OBJECT_
#define _OBJECT_

#include <stdlib.h>

#include "vector.h"
#include "material.h"
#include "pixel.h"

struct _verticie;
struct _triangle;
struct _sphere;
struct _AABB;
struct _object;

typedef struct _verticie {
  Vector position;
  Vector normal;
} Vertex;

typedef struct _triangle {
  Vertex *verticies[3];
  struct _object *parent;
} Triangle;

typedef struct _sphere {
  Vector center;
  double radius;
} Sphere;

typedef struct _AABB {
  Vector low, high;
} AABB;

typedef struct _object {
  Vertex *verticies;
  int n_verticies;
  Triangle *triangles;
  int n_triangles;
  Pixel color;
  Material material;
  Sphere bounding_volume;
} Object;

int object_is_point_inside_aabb(Vector v, AABB box);
Object *new_object(void);
int free_object(Object *object);

#endif