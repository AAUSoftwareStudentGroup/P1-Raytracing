#include "object.h"

Object *new_object(void) {
  Object *object = (Object*)malloc(sizeof(Object));
  return object;
}

int free_object(Object *object) {
  free(object);
  return 1;
}
