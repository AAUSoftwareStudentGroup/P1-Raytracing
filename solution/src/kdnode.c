#include "kdnode.h"

/* builds kdtree recursively */
int kdnode_build_subnodes(KDNode *node, int level) {

  /* Initialize variables */
  double cut_position;
  int i, j, n_same_triangles;
  Vector box_size;
  VectorAxis axis, largest_axis;

  cut_position = 0;
  n_same_triangles = 0;
  node->low = new_kdnode();
  node->high = new_kdnode();
  node->low->box = node->box;
  node->high->box = node->box;
  node->low->triangles  = (Triangle**)malloc(node->n_triangles*sizeof(Triangle*));
  node->high->triangles = (Triangle**)malloc(node->n_triangles*sizeof(Triangle*));

  box_size = vector_subtract(node->box.high, node->box.low);
  
  /* Find widest axis on box, to get lowest amount of triangles that are in both boxes */
  largest_axis = X_AXIS;
  for(axis = 0; axis <= 2; axis++) {
    if(vector_get_component(box_size, axis) > vector_get_component(box_size, largest_axis))
      largest_axis = axis;
  }

  /* Find mid-point in box */
  for(i = 0; i < node->n_triangles; i++) {
    for(j = 0; j < VERTICES_IN_TRIANGLE; j++) {
      cut_position += vector_get_component(node->triangles[i]->verticies[j]->position, largest_axis)
      / (node->n_triangles*VERTICES_IN_TRIANGLE);
    }
  }
  /* Let mid-point be the seperator for the high and low box */
  vector_set_component(&node->low->box.high, largest_axis, cut_position);
  vector_set_component(&node->low->box.low, largest_axis, cut_position);

  /* Fill triangles in corresponding box. If a triangle is in both boxes, it's put in both boxes */
  for(i = 0; i < node->n_triangles; i++) {
    if(intersection_triangle_aabb(*(node->triangles[i]), node->low->box)) {
        node->low->triangles[node->low->n_triangles++] = node->triangles[i];
    }
    if(intersection_triangle_aabb(*(node->triangles[i]), node->high->box)) {
        node->high->triangles[node->high->n_triangles++] = node->triangles[i];
    }

    /* If triangle is in both boxes */
    if(node->low->n_triangles && node->high->n_triangles &&
       node->low->triangles[node->low->n_triangles-1] ==
       node->high->triangles[node->high->n_triangles-1])
      n_same_triangles++;
  }

  /* Return if more than 50% of triangles appear in both branches  */
  /*           or at most 2 triangles in either branch             */
  /*           or depth of tree too deep                           */
  if(++level > 30 || n_same_triangles > node->n_triangles/2 ||
     node->low->n_triangles <= 2 || node->high->n_triangles <= 2) {
    return 1;
  }
  /* Avoid leaking memory */
  free(node->triangles);

  /* Call recursively until returns in if-statement above */
  kdnode_build_subnodes(node->low, level);
  kdnode_build_subnodes(node->high, level);
  return 1;
}

int kdnode_is_leaf(KDNode *node) {
  return node->low == NULL || node->high == NULL;
}

KDNode *new_kdnode(void) {
  KDNode *n = (KDNode*)malloc(sizeof(KDNode));
  
  n->n_triangles = 0;
  n->low = NULL;
  n->high = NULL;
  n->triangles = NULL;
  return n;
}

int free_kdnode(KDNode *node) {
  if(kdnode_is_leaf(node)) {
    free(node->triangles);
  } 
  else {
    free_kdnode(node->low);
    free_kdnode(node->high);
  }
  return 1;
}