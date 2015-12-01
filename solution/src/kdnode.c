#include "kdnode.h"

int kdnode_build(KDNode *root, Object **objects, int n_objects) {
  int i, j, k;
  Vector lowest, highest;


  root->low = NULL;
  root->high = NULL;

  lowest = highest = objects[0]->verticies[0].position;

  for(i = 0; i < n_objects; i++){
    for(j = 0; j < objects[i]->n_verticies; j++){
      if(objects[i]->verticies[j].position.x < lowest.x){
        lowest.x = objects[i]->verticies[j].position.x;
      }
      else if(objects[i]->verticies[j].position.x > highest.x){
        highest.x = objects[i]->verticies[j].position.x;
      }
      if(objects[i]->verticies[j].position.y < lowest.y){
        lowest.y = objects[i]->verticies[j].position.y;
      }
      else if(objects[i]->verticies[j].position.y > highest.y){
        highest.y = objects[i]->verticies[j].position.y;
      }
      if(objects[i]->verticies[j].position.z < lowest.z){
        lowest.z = objects[i]->verticies[j].position.z;
      }
      else if(objects[i]->verticies[j].position.z > highest.z){
        highest.z = objects[i]->verticies[j].position.z;
      }
    }
  }

  root->box.low = lowest;
  root->box.high = highest;

  for(i = j = 0; i < n_objects; i++) {
    j += objects[i]->n_triangles;
  }
  root->n_geometry = j;
  root->geometry = (Triangle**)malloc(j*sizeof(Triangle*));
  k = 0;
  for(i = 0; i < n_objects; i++) {
    for(j = 0; j < objects[i]->n_triangles; j++) {
      root->geometry[k++] = &(objects[i]->triangles[j]);
    }
  }


  return 1;
}