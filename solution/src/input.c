#include "input.h"

int input_parse(int argc, char* argv[], Scene **scene, Camera **camera) {
  int i;

  *scene = new_scene();
  *camera = new_camera(1920, 1080);

  (*scene)->n_objects = 1;

  (*scene)->objects = (Object**)malloc((*scene)->n_objects * sizeof(Object*));
  for(i = 0; i < (*scene)->n_objects; i++)
    (*scene)->objects[i] = new_object();

  (*scene)->objects[0]->verticies = (Vector*)malloc(3*sizeof(Vector));
  (*scene)->objects[0]->verticies[0] = (Vector){0,-0,-2000};
  (*scene)->objects[0]->verticies[1] = (Vector){500,500,-2000};
  (*scene)->objects[0]->verticies[2] = (Vector){400,100,-2000};
  Triangle *ts = (Triangle*)malloc(1*sizeof(Triangle));
  ts[0].verticies[0] = &((*scene)->objects[0]->verticies[0]);
  ts[0].verticies[1] = &((*scene)->objects[0]->verticies[1]);
  ts[0].verticies[2] = &((*scene)->objects[0]->verticies[2]);
  (*scene)->objects[0]->triangles = ts;
  (*scene)->objects[0]->color.red = 1.0;
  (*scene)->objects[0]->color.green = 0.75;  
  (*scene)->objects[0]->color.blue = 0.5; 
  (*scene)->objects[0]->material.ambient_coefficient = 0.85;

  return 1;
}