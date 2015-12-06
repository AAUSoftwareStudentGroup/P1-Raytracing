#include "ray.h"

Ray create_ray(Vector origin, Vector direction) {
  return (Ray){origin, vector_normalize(direction)};
}

Vector ray_get_point(Ray ray, double t) {
  return vector_add(ray.initial_point, vector_scale(ray.direction, t));
}