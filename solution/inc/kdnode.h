#ifndef _KDNODE_
#define _KDNODE_

#include <stdlib.h>

#include "object.h"
#include "vector.h"

typedef struct _KDNode {
  struct _KDNode *low, *high;
  AABB box;
  Triangle **geometry;
  int n_geometry;
} KDNode;

int kdnode_build(KDNode *root, Object **objects, int n_objects);

#endif