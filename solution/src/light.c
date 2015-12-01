#include "light.h"

Vector point_light_get_sample(PointLight *l) {
  return l->position;
}