#include "raytracer.h"
#include "input.h"
#include "scene.h"
#include "camera.h"
#include "image.h"

int main(int argc, char* argv[]) {
  Scene *scene;
  Camera *camera;
  Image *image;

  input_parse(argc, argv, &scene, &camera);
  image = raytracer_render(scene, camera);

  image_write(image, "out.ppm");

  return 0;
}