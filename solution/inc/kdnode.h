#ifndef _KDNODE_
#define _KDNODE_

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#include <stdlib.h>

#include "vector.h"
#include "triangle.h"
#include "aabb.h"
#include "intersection.h"

typedef struct _KDNode {
  struct _KDNode *low, *high;
  AABB box;
  Triangle **triangles;
  int n_triangles;
} KDNode;

int kdnode_build_subnodes(KDNode *node, int level);
int kdnode_is_leaf(KDNode *node);
KDNode *new_kdnode(void);

#endif