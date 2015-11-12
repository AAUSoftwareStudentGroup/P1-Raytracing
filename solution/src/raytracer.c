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
  Intersection *intersection;
  Pixel pixel = {0,0,0};
  if( raytracer_scene_intersection(ray, scene, &intersection) ) {
    // shade based on intersection
    printf("Intersect\n");
    pixel = (Pixel){255, 255, 255};
  }
  return pixel;
}

int raytracer_scene_intersection(Ray ray, Scene *scene, Intersection **intersection) {
  int i;
  double lowest_t = -1;
  Intersection* temporary_intersection;

  for(i = 0; i < scene->n_objects; i++) {
    if(raytracer_object_intersection(ray, scene->objects[i], &temporary_intersection)) {
      if(temporary_intersection->t < lowest_t || lowest_t == -1) {
        *intersection = temporary_intersection;
        lowest_t = temporary_intersection->t;
      }
    }
  }
  return lowest_t > 0;
}

int raytracer_object_intersection(Ray ray, Object *object, Intersection **intersection) {
  // Find normalvector to plane
  // Crossproduct (AB, CA)
  int i;
  for(i = 0; i < object->n_triangles;i++)
  {
    Vector AB = vector_subtract(*object->triangles[i].verticies[1], *object->triangles[i].verticies[0]);
    Vector CA = vector_subtract(*object->triangles[i].verticies[0], *object->triangles[i].verticies[2]);
    Vector BC = vector_subtract(*object->triangles[i].verticies[2], *object->triangles[i].verticies[1]);

    Vector normal_vector_plane = vector_normalize(vector_cross(AB, CA));
    
    Vector d = vector_cross(normal_vector_plane, AB);

    double t = (vector_dot( vector_subtract(d, normal_vector_plane), ray.initial_point)) / vector_dot(normal_vector_plane, ray.direction);

    // Q is a point on the plane n which triangles[i]->verticies lies, but not neccesarily inside the triangles[i]->verticies
    Vector Q = ray_get_point_of_intersection(ray, t);

    Vector AQ = vector_subtract(Q, *object->triangles[i].verticies[0]);
    Vector BQ = vector_subtract(Q, *object->triangles[i].verticies[1]);
    Vector CQ = vector_subtract(Q, *object->triangles[i].verticies[2]);

    // Inside-outside test
    if( (vector_dot(vector_cross(AB, AQ), normal_vector_plane) >= 0) &&
      (vector_dot(vector_cross(BC, AQ), normal_vector_plane) >= 0) &&
      (vector_dot(vector_cross(CA, AQ), normal_vector_plane) >= 0)) {
      // Ray hits! Q is inside triangles[i]->verticies
  
      (*intersection)->surface_normal = normal_vector_plane;
      (*intersection)->triangle = &object->triangles[i];
      (*intersection)->t = t;
      (*intersection)->material = object->material;
      (*intersection)->color = object->color;
      return 1;
    }
  }
  return 0;
}