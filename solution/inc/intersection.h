#ifndef _INTERSECTION_
#define _INTERSECTION_

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#include <float.h>

#include "ray.h"
#include "triangle.h"
#include "aabb.h"

int intersection_triangle_aabb(Triangle triangle, AABB bounding_box);
int intersection_ray_aabb(Ray r, AABB box, double *tmin, double *tmax);

#endif