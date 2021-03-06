#ifndef _INTERSECTION_
#define _INTERSECTION_

#include <float.h>

#include "ray.h"
#include "triangle.h"
#include "aabb.h"
#include "plane.h"
#include "macros.h"

int intersection_triangle_aabb(Triangle triangle, AABB bounding_box);
int intersection_ray_aabb(Ray r, AABB box, double *tmin, double *tmax);
int intersection_ray_axis_aligned_plane(Ray r, Vector plane_position, VectorAxis axis, double *t);
int intersection_ray_plane(Ray r, Plane p, double *time);

#endif