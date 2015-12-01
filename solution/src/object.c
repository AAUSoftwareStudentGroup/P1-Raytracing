#include "object.h"

int object_is_point_inside_aabb(Vector v, AABB box) {
  return v.x >= box.low.x && v.x <= box.high.x &&
         v.y >= box.low.y && v.y <= box.high.y &&
         v.z >= box.low.z && v.z <= box.high.z;
}

Object *new_object(void) {
  Object *object = (Object*)malloc(sizeof(Object));
  return object;
}

int free_object(Object *object) {
  free(object);
  return 1;
}
