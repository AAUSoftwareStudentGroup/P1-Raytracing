#include "aabb.h"

/* Returns true if point v is inside or on the border of box*/
int aabb_is_point_inside(Vector v, AABB box) {
  return v.x >= box.low.x && v.x <= box.high.x &&
         v.y >= box.low.y && v.y <= box.high.y &&
         v.z >= box.low.z && v.z <= box.high.z;
}