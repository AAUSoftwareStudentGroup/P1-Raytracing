#include "object.h"

Object *new_object(void) {
  Object *object = (Object*)malloc(sizeof(Object));
  return object;
}

int free_object(Object *object) {
  if(object->root.low != NULL)
    free_kdnode(object->root.low);
  if(object->root.high != NULL)
    free_kdnode(object->root.high);

  free(object);
  return 1;
}
