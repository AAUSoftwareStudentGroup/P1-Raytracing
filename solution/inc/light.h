#ifndef _LIGHT_
#define _LIGHT_

#include "vector.h"
#include "pixel.h"

typedef struct _light {
  Vector position;
  Pixel intensity;
} Light;

#endif