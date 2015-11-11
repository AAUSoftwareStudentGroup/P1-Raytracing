#ifndef _RAYTRACER_
#define _RAYTRACER_

#include "scene.h"
#include "camera.h"
#include "image.h"
#include "ray.h"

Image* raytracer_render(Scene *scene, Camera *camera);
Pixel raytracer_trace(Ray ray, Scene *scene);

#endif