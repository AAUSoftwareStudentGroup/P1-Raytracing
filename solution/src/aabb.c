#include "aabb.h"

int aabb_is_point_inside(Vector v, AABB box) {
  return v.x >= box.low.x && v.x <= box.high.x &&
         v.y >= box.low.y && v.y <= box.high.y &&
         v.z >= box.low.z && v.z <= box.high.z;
}

int aabb_is_triangle_in(Triangle t, AABB box) {
  return 0;
}
