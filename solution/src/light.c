#include "light.h"

/* Create random position within radius */
Vector point_light_get_sample(PointLight *l) {
  Vector rand_pos = {rand()-RAND_MAX/2, rand()-RAND_MAX/2, rand()-RAND_MAX/2};
  rand_pos = vector_normalize(rand_pos);
  rand_pos = vector_scale(rand_pos, l->radius);
  return vector_add(l->position, rand_pos);
}