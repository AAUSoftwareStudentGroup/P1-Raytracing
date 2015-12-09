#include "intersection.h"

int intersection_triangle_aabb(Triangle triangle, AABB bounding_box){
  int i;
  double tmin, tmax;
  Ray edge_ray;
  for(i = 0; i < 3; i++){
    edge_ray = (Ray){triangle.verticies[i]->position, triangle.edges[i]};
    if(intersection_ray_aabb(edge_ray, bounding_box, &tmin, &tmax) && tmin <= 1)
      return 1;
  }
  return 0;
}

int intersection_ray_aabb(Ray r, AABB box, double *tmin, double *tmax) {
  *tmin = -DBL_MAX;
  *tmax =  DBL_MAX;

  if(r.direction.x == 0 && r.initial_point.x < box.low.x && r.initial_point.x > box.high.x){
    return 0;
  }
  else{
    double tx1 = (box.low.x - r.initial_point.x) / r.direction.x;
    double tx2 = (box.high.x - r.initial_point.x) / r.direction.x; 

    *tmin = MAX(*tmin, MIN(tx1, tx2));
    *tmax = MIN(*tmax, MAX(tx1, tx2));
  }

  if(r.direction.y == 0 && r.initial_point.y < box.low.y && r.initial_point.y > box.high.y){
    return 0;
  }
  else{
    double ty1 = (box.low.y - r.initial_point.y) / r.direction.y;
    double ty2 = (box.high.y - r.initial_point.y) / r.direction.y; 

    *tmin = MAX(*tmin, MIN(ty1, ty2));
    *tmax = MIN(*tmax, MAX(ty1, ty2));
  }

  if(r.direction.z == 0 && r.initial_point.z < box.low.z && r.initial_point.z > box.high.z){
    return 0;
  }
  else{
    double tz1 = (box.low.z - r.initial_point.z) / r.direction.z;
    double tz2 = (box.high.z - r.initial_point.z) / r.direction.z; 

    *tmin = MAX(*tmin, MIN(tz1, tz2));
    *tmax = MIN(*tmax, MAX(tz1, tz2));
  }
  return *tmax >= *tmin && *tmax > 0;
}

double intersection_ray_plane(Ray r, Plane p) {
  double denominator, time;

  // test ray_plane intersection
  denominator = vector_dot(r.direction, p.normal);

  if(denominator == 0)
    return 0;

  time = vector_dot(vector_subtract(p.point, r.initial_point), p.normal) / denominator;

  return time;
}


