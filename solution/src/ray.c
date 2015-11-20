#include "ray.h"

Ray create_ray(Vector p1, Vector p2) {
  return (Ray){p1, vector_normalize(p2)};
}

Vector ray_get_point(Ray ray, double t) {
    return vector_add(ray.initial_point, vector_scale(ray.direction, t));
}