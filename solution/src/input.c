#include "input.h"

int input_parse(int argc, char* argv[], Scene **scene, Camera **camera) {
  int i;

  *scene = new_scene();
  *camera = new_camera(1000, 1000);

  (*scene)->n_objects = 1;

  (*scene)->objects = (Object**)malloc((*scene)->n_objects * sizeof(Object*));
  for(i = 0; i < (*scene)->n_objects; i++)
    (*scene)->objects[i] = new_object();

  (*scene)->objects[0]->verticies = (Vector*)malloc(3*sizeof(Vector));
  (*scene)->objects[0]->verticies[0] = (Vector){-1000,1000,1000};
  (*scene)->objects[0]->verticies[1] = (Vector){ 1000,1000,1000};
  (*scene)->objects[0]->verticies[2] = (Vector){    0,1000,   0};
  Triangle *ts = (Triangle*)malloc(1*sizeof(Triangle));
  ts[0].verticies[0] = &((*scene)->objects[0]->verticies[0]);
  ts[0].verticies[1] = &((*scene)->objects[0]->verticies[1]);
  ts[0].verticies[2] = &((*scene)->objects[0]->verticies[2]);
  (*scene)->objects[0]->n_verticies = 3;
  (*scene)->objects[0]->n_triangles = 1;

  (*scene)->objects[0]->triangles = ts;
  (*scene)->objects[0]->color.red = 1.0;
  (*scene)->objects[0]->color.green = 0.75;  
  (*scene)->objects[0]->color.blue = 0.5; 
  (*scene)->objects[0]->material.ambient_coefficient = 0.85;
  (*scene)->objects[0]->material.diffuse_coefficient = 0.75;
  
  (*scene)->ambient_intensity = create_from_color_temperature(10000);
  (*scene)->lights = (PointLight**)malloc(sizeof(PointLight*));
  (*scene)->lights[0] = (PointLight*)malloc(sizeof(PointLight));
  (*scene)->n_lights = 1;
  (*scene)->lights[0]->position = (Vector){0, 0, 0};
  (*scene)->lights[0]->intensity = create_from_color_temperature(10000);
  
  return 1;
}