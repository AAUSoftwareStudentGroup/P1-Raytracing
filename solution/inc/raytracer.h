#ifndef _RAYTRACER_
#define _RAYTRACER_

#include "scene.h"
#include "camera.h"
#include "image.h"
#include "ray.h"

#define EPSILON 0.000001

typedef struct _intersection {
  Vector normal;
  Material material;
  Pixel color;
  Triangle *triangle;
  double t;
} Intersection;

Image* raytracer_render(Scene *scene, Camera *camera);
Pixel raytracer_trace(Ray ray, Scene *scene);
int raytracer_scene_intersection(Ray ray, Scene *scene, 
                                 Intersection **intersection);
int raytracer_object_intersection(Ray ray, Object *object, Intersection **intersection);
int raytracer_triangle_intersection(Ray ray, Triangle *triangle, Intersection **intersection);
Pixel raytracer_phong(Intersection *intersection, Scene *scene);
Intersection *new_intersection(void);

#endif