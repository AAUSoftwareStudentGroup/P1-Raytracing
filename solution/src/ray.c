#include "ray.h"

Ray create_ray(Vector p1, Vector p2) {
  return (Ray){p1, p2};
}