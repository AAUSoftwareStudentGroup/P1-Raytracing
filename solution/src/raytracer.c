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
      direction = vector_add(direction, vector_scale(camera->up, -y+(double)(camera->height)/2));
      direction = vector_add(direction, vector_scale(camera->right, x-(double)(camera->width)/2));

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
    pixel = raytracer_phong(intersection, scene);
  }
  return pixel;
}

int raytracer_scene_intersection(Ray ray, Scene *scene, Intersection **intersection) {
  int i;
  double lowest_t = -1;
  Intersection* temporary_intersection = malloc(sizeof(Intersection));

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
/*
int raytracer_object_intersection(Ray ray, Object *object, Intersection **intersection) {
  // Find normal-vector to plane
  // Crossproduct (AB, CA)
  
  // TODO: Check for nærmeste triangle istedet for den første!
  int i;
  for(i = 0; i < object->n_triangles;i++) {
    Vector AB = vector_subtract(*object->triangles[i].verticies[1], *object->triangles[i].verticies[0]);
    Vector AC = vector_subtract(*object->triangles[i].verticies[2], *object->triangles[i].verticies[0]);
    Vector BC = vector_subtract(*object->triangles[i].verticies[2], *object->triangles[i].verticies[1]);

    Vector normal_vector_plane = vector_normalize(vector_cross(AB, AC));
    
    double d = vector_dot(normal_vector_plane, AB);

    double t = (d - vector_dot( normal_vector_plane, ray.initial_point)) / vector_dot(normal_vector_plane, ray.direction);

    // Q is a point on the plane n which triangles[i]->verticies lies, but not neccesarily inside the triangles[i]->verticies
    Vector Q = ray_get_point_of_intersection(ray, t);

    Vector AQ = vector_subtract(Q, *object->triangles[i].verticies[0]);
    Vector BQ = vector_subtract(Q, *object->triangles[i].verticies[1]);
    Vector CQ = vector_subtract(Q, *object->triangles[i].verticies[2]);

    // Inside-outside test
    if( (vector_dot(vector_cross(AB, AQ), normal_vector_plane) >= 0) &&
      (vector_dot(vector_cross(BC, AQ), normal_vector_plane) >= 0) &&
      (vector_dot(vector_cross(vector_scale(AC, -1), AQ), normal_vector_plane) >= 0)) {
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
*/
int raytracer_object_intersection(Ray ray, Object *object, Intersection **intersection) {
  double lowest_t, temporary_t;
  int i;

  lowest_t = temporary_t = -1;

  for(i = 0; i < object->n_triangles; i++) {
    if(raytracer_triangle_intersection(ray, &(object->triangles[i]), &temporary_t)) {
      if(temporary_t < lowest_t || lowest_t == -1) {
        lowest_t = temporary_t;
      }
    }
  }
  (*intersection)->t = lowest_t;
  return lowest_t > 0;
}

int raytracer_triangle_intersection(Ray ray, Triangle *triangle, double *time) {
  *time = -1;

  Vector v01 = vector_normalize(vector_subtract(*(triangle->verticies[1]), *(triangle->verticies[0])));
  Vector v02 = vector_normalize(vector_subtract(*(triangle->verticies[2]), *(triangle->verticies[0])));
  Vector v10 = vector_normalize(vector_subtract(*(triangle->verticies[0]), *(triangle->verticies[1])));
  Vector v12 = vector_normalize(vector_subtract(*(triangle->verticies[2]), *(triangle->verticies[1])));
  Vector v20 = vector_normalize(vector_subtract(*(triangle->verticies[0]), *(triangle->verticies[2])));
  Vector v21 = vector_normalize(vector_subtract(*(triangle->verticies[1]), *(triangle->verticies[2])));
  Vector tri_normal = vector_normalize(vector_cross(v01, v02));
  
  if(vector_dot(ray.direction, tri_normal) > 0)
    tri_normal = vector_scale(tri_normal, -1);
  
  double denominator = vector_dot(ray.direction, tri_normal);
  if(denominator == 0)
    return 0;

  *time = vector_dot(
            vector_subtract(
                *(triangle->verticies[0]), 
                ray.initial_point
              ), 
              tri_normal
            ) / denominator;

  // If time is positive: check if point is inside triangle
  if(*time > 0) {

    Vector p = ray_get_point_of_intersection(ray, *time);
    Vector v0p = vector_subtract(p, *(triangle->verticies[0]));
    Vector v1p = vector_subtract(p, *(triangle->verticies[1]));
    Vector v2p = vector_subtract(p, *(triangle->verticies[2]));
    if( vector_dot(vector_cross(v02, v01), vector_cross(v02, v0p)) >= 0 &&
        vector_dot(vector_cross(v10, v12), vector_cross(v10, v1p)) >= 0 &&
        vector_dot(vector_cross(v21, v20), vector_cross(v21, v2p)) >= 0 ) {
      return 1;
    }
  }
  return 0;
}

Pixel raytracer_phong(Intersection *intersection, Scene *scene) {
  Pixel result = create_pixel(1,0,0);
  // calculate phong shading

  return result;
}