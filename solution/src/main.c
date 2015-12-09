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
  Configuration conf;

  unsigned long t0 = time(NULL);
  conf = create_configuration();
  
  if(input_parse(argc, argv, &scene, &camera, &conf) == 0) {
    return -1;
  }
  
  image = raytracer_render(scene, camera);
  
  image_write(image, conf.out_file);

  free_camera(camera);
  free_scene(scene);
  
  printf("%lus\n", time(NULL) - t0);
  return 0;
}