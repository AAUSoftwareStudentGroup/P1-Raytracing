#include "raytracer.h"
#include "input.h"
#include "scene.h"
#include "camera.h"
#include "image.h"
#include <stdio.h>
int main(int argc, char* argv[]) {
  Scene *scene;
  Camera *camera;
  Image *image;

  if(input_parse(argc, argv, &scene, &camera) == 0) {
    return -1;
  }
  image = raytracer_render(scene, camera);
  image_write(image, "out.ppm");
  return 0;
}