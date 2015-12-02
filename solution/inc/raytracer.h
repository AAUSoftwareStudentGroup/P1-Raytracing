#ifndef _RAYTRACER_
#define _RAYTRACER_

#include <stdlib.h>
#include "scene.h"
#include "camera.h"
#include "image.h"
#include "ray.h"
#include "intersection.h"


typedef struct _intersection {
  Vector normal;
  Material material;
  Pixel color;
  Triangle *triangle;
  Ray ray;
  double t;
} Intersection;

Image* raytracer_render(Scene *scene, Camera *camera);
Ray raytracer_calculate_ray(int x, int y, Camera *camera);
Pixel raytracer_trace(Ray ray, Scene *scene);
int raytracer_scene_intersection(Ray ray, Scene *scene, 
                                 Intersection *intersection);
int raytracer_object_intersection(Ray ray, Object *object, Intersection *intersection);
int raytracer_kdtree_intersection(Ray ray, KDNode *node, Intersection *intersection);
int raytracer_triangle_intersection(Ray ray, Triangle *triangle, Intersection *intersection);
Pixel raytracer_phong(Intersection intersection, Scene *scene);
Pixel raytracer_phongv2(Intersection *intersection, Scene *scene);
Intersection *new_intersection(void);
Intersection create_intersection(void);

#endif