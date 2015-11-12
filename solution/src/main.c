#include "raytracer.h"
#include "input.h"
#include "scene.h"
#include "camera.h"
#include "image.h"

int main(int argc, char* argv[]) {
/*
  double t = -1;
  int is_intersecting;
  Triangle tri;
  Vector *v_lst = (Vector*)malloc(3*sizeof(Vector));
  v_lst[0] = (Vector){  -1, 2,  1};
  v_lst[1] = (Vector){   1, 2,  1};
  v_lst[2] = (Vector){-0.5, 2, -1};
  tri.verticies[0] = v_lst;
  tri.verticies[1] = v_lst+1;
  tri.verticies[2] = v_lst+2;
  Ray r = create_ray((Vector){0,0,0}, vector_normalize((Vector){0,1,0}));

  is_intersecting = raytracer_triangle_intersection(r, &tri, &t);
  Vector p = ray_get_point_of_intersection(r, t);
  printf("intersects? %d: time - %lf - Point{%lf, %lf, %lf}\n", is_intersecting, t, p.x, p.y, p.z);
*/
  
  Scene *scene;
  Camera *camera;
  Image *image;

  input_parse(argc, argv, &scene, &camera);
  image = raytracer_render(scene, camera);

  image_write(image, "out.ppm");
  
  return 0;
}