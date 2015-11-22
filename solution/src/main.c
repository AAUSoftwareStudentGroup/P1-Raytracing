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
  char* w[] = {"woot", "untitled.ply"};

  if(input_parse(2, w, &scene, &camera) == 0) {
    fprintf(stderr, "Failed to initialize!\n");
    return -1;
  }
  image = raytracer_render(scene, camera);
  image_write(image, "out.ppm");
  return 0;
}