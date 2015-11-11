#include "raytracer.h"

Image* raytracer_render(Scene* scene, Camera *camera) {
  int x, y;
  Image *image;
  Ray ray;

  image = new_image(camera->width, camera->height);


  for(x = 0; x < camera->width; x++) {
    for(y = 0; y < camera->height; y++) {
      // beregn ray
      Vector direction;
      direction = vector_scale(camera->forward, camera->distance);
      direction = vector_add(direction, vector_scale(camera->up, y-camera->height/2));
      direction = vector_add(direction, vector_scale(camera->right, x-camera->width/2));

      ray = create_ray(camera->position, direction);

      // trace ray
      image->pixels[x][y] = raytracer_trace(ray, scene);
    }
  }


  return image;
}

Pixel raytracer_trace(Ray ray, Scene *scene) {
  
  return (Pixel){0.0, 0.0, 0.0};
}