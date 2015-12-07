#include "kdnode.h"

int kdnode_build_subnodes(KDNode *node, int level) {
  double cut_position;
  int i, j, n_same_triangles;

  cut_position = 0;
  n_same_triangles = 0;
  node->low = new_kdnode();
  node->high = new_kdnode();
  node->low->box = node->box;
  node->high->box = node->box;
  node->low->triangles = (Triangle**)malloc(node->n_triangles*sizeof(Triangle*));
  node->high->triangles = (Triangle**)malloc(node->n_triangles*sizeof(Triangle*));

  if(node->box.high.x - node->box.low.x > node->box.high.y - node->box.low.y && 
     node->box.high.x - node->box.low.x > node->box.high.z - node->box.low.z) { // x axis is longest axis
    for(i = 0; i < node->n_triangles; i++) {
      for(j = 0; j < 3; j++) {
        cut_position += node->triangles[i]->verticies[j]->position.x / (node->n_triangles*3);
      }
    }
    node->low->box.high.x = cut_position;
    node->high->box.low.x = cut_position;
  }
  else if(node->box.high.y - node->box.low.y > node->box.high.x - node->box.low.x && 
          node->box.high.y - node->box.low.y > node->box.high.z - node->box.low.z) { // y axis is longest axis
    for(i = 0; i < node->n_triangles; i++) {
      for(j = 0; j < 3; j++) {
        cut_position += node->triangles[i]->verticies[j]->position.y / (node->n_triangles*3);
      }
    }
    node->low->box.high.y = cut_position;
    node->high->box.low.y = cut_position;
  }
  else { // z axis is longest axis
    for(i = 0; i < node->n_triangles; i++) {
      for(j = 0; j < 3; j++) {
        cut_position += node->triangles[i]->verticies[j]->position.z / (node->n_triangles*3);
      }
    }
    node->low->box.high.z = cut_position;
    node->high->box.low.z = cut_position;
  }

  for(i = 0; i < node->n_triangles; i++) {
    if(intersection_triangle_aabb(*(node->triangles[i]), node->low->box)) {
        node->low->triangles[node->low->n_triangles++] = node->triangles[i];
    }
    if(intersection_triangle_aabb(*(node->triangles[i]), node->high->box)) {
        node->high->triangles[node->high->n_triangles++] = node->triangles[i];
    }
/*
    for(j = 0; j < 3; j++) {
      if(!(node->low->n_triangles && 
           node->low->triangles[node->low->n_triangles-1] == node->triangles[i]) &&
         aabb_is_point_inside(node->triangles[i]->verticies[j]->position, node->low->box)) {
        node->low->triangles[node->low->n_triangles++] = node->triangles[i];
      }
      if(!(node->high->n_triangles && 
           node->high->triangles[node->high->n_triangles-1] == node->triangles[i]) && 
         aabb_is_point_inside(node->triangles[i]->verticies[j]->position, node->high->box)) {
        node->high->triangles[node->high->n_triangles++] = node->triangles[i];
      }
    }
*/
    // if last triangle in both nodes is same
    if(node->low->n_triangles && node->high->n_triangles && node->low->triangles[node->low->n_triangles-1] == node->high->triangles[node->high->n_triangles-1])
      n_same_triangles++;
  }
  if(++level > 30 || n_same_triangles > node->n_triangles/2 || node->low->n_triangles <= 2 || node->high->n_triangles <= 2) {
    // return -- more than 50% of triangles appear in both branches
    return 1;
  }
  free(node->triangles);
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