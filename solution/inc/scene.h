#ifndef _SCENE_
#define _SCENE_

#include <stdlib.h>

#include "object.h"
#include "light.h"

typedef struct _scene {
  Object **objects;
  unsigned int n_objects;
  Light **lights;
  unsigned int n_lights;
} Scene;

Scene *new_scene(void);
int free_scene(Scene *scene);

#endif