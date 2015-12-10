#include "intersection.h"

/* finds if a triangle intersects or is inside AABB */
int intersection_triangle_aabb(Triangle triangle, AABB bounding_box){
  int i;
  double tmin, tmax;
  Ray edge_ray;
  for(i = 0; i < VERTICES_IN_TRIANGLE; i++){
    edge_ray = (Ray){triangle.verticies[i]->position, triangle.edges[i]};
    if(intersection_ray_aabb(edge_ray, bounding_box, &tmin, &tmax) && tmin <= 1)
      return 1;
  }
  return 0;
}

/* Finds the entry and exit points of a ray trough AABB if any */
int intersection_ray_aabb(Ray r, AABB box, double *tmin, double *tmax) {
  VectorAxis axis;
  double t1, t2;

  *tmin = -DBL_MAX;
  *tmax =  DBL_MAX;

  for(axis = X_AXIS; axis <= Z_AXIS; axis++) {
    if(vector_get_component(r.direction, axis) == 0 &&
       (vector_get_component(r.initial_point, axis) < vector_get_component(box.low, axis) ||
        vector_get_component(r.initial_point, axis) > vector_get_component(box.high, axis))) {
      return 0;
    }
  }

  for(axis = X_AXIS; axis <= Z_AXIS; axis++) {
    intersection_ray_axis_aligned_plane(r, box.low, axis, &t1);
    intersection_ray_axis_aligned_plane(r, box.high, axis, &t2);
    *tmin = MAX(*tmin, MIN(t1, t2));
    *tmax = MIN(*tmax, MAX(t1, t2));
  }
  return *tmax >= *tmin && *tmax > 0;
}

/* finds intersectionpoint between ray and axis-aligned plane */
int intersection_ray_axis_aligned_plane(Ray r, Vector plane_position, VectorAxis axis, double *t) {
  *t = (vector_get_component(plane_position, axis) - 
        vector_get_component(r.initial_point, axis)) / 
        vector_get_component(r.direction, axis);
  return 1;
}

/* Finds intersectionpoint between ray and plane */
int intersection_ray_plane(Ray r, Plane p, double *time) {
  double denominator;

  /* test ray_plane intersection */
  denominator = vector_dot(r.direction, p.normal);

  if(denominator == 0)
    return 0;

  *time = vector_dot(vector_subtract(p.point, r.initial_point), p.normal) / denominator;

  return 1;
}