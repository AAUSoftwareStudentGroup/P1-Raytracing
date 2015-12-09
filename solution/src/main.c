#include "raytracer.h"
#include "input.h"
#include "scene.h"
#include "camera.h"
#include "image.h"
#include <stdio.h>
#include <time.h>

int main(int argc, char* argv[]) {
  Scene *scene;
  Camera *camera;
  Image *image;

  unsigned long t0 = time(NULL);
  
  if(input_parse(argc, argv, &scene, &camera) == 0) {
    return -1;
  }
  
  image = raytracer_render(scene, camera);
  
  if(argc == 3)
    image_write(image, argv[2]);
  else
    image_write(image, "out.ppm");

  free_camera(camera);
  free_scene(scene);
  
  printf("%lus\n", time(NULL) - t0);
  return 0;
}