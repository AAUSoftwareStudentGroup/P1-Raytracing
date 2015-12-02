#ifndef _LIGHT_
#define _LIGHT_

#include "vector.h"
#include "pixel.h"

typedef struct _pointlight {
  Vector position;
  Pixel color;
  double intensity;
  double radius;
  int sampling_rate;
} PointLight;

Vector point_light_get_sample(PointLight *l);

#endif