#ifndef _SCENE_
#define _SCENE_

#include <stdlib.h>

#include "object.h"
#include "light.h"
#include "kdnode.h"

typedef struct _scene {
  Object **objects;
  unsigned int n_objects;
  PointLight **lights;
  unsigned int n_lights;
  Pixel ambient_intensity;
} Scene;

Scene *new_scene(void);
int free_scene(Scene *scene);

#endif