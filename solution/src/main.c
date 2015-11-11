#include "raytracer.h"
#include "input.h"
#include "scene.h"
#include "image.h"

int main(int argc, char* argv[]) {
  Scene *scene;
  Image *img;

  scene = input_parse(argc, argv);
  img = raytracer_render(scene);

  image_write(img, "out.ppm");

  return 0;
}