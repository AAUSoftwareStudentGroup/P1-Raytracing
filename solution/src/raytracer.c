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

int raytracer_object_intersection(Ray ray, Object *object, Intersection **intersection) {
  double lowest_t;
  Intersection *nearest_intersection, *temporary_intersection;
  int i;
  
  nearest_intersection = new_intersection();
  temporary_intersection = new_intersection();
  nearest_intersection->t = -1;

  for(i = 0; i < object->n_triangles; i++) {
    if(raytracer_triangle_intersection(ray, &(object->triangles[i]), &temporary_intersection)) {
      if(temporary_intersection->t < nearest_intersection->t || nearest_intersection->t == -1) {
        *nearest_intersection = *temporary_intersection;
        nearest_intersection->triangle = &(object->triangles[i]);
        nearest_intersection->color = object->color;
        nearest_intersection->material = object->material;
      }
    }
  }
  *intersection = nearest_intersection;
  return nearest_intersection->t > 0;
}

int raytracer_triangle_intersection(Ray ray, Triangle *triangle, Intersection **intersection) {
  double time = -1;

  Vector v01 = vector_normalize(vector_subtract(*(triangle->verticies[1]), 
                                *(triangle->verticies[0])));
  Vector v02 = vector_normalize(vector_subtract(*(triangle->verticies[2]), 
                                *(triangle->verticies[0])));
  Vector v10 = vector_normalize(vector_subtract(*(triangle->verticies[0]), 
                                *(triangle->verticies[1])));
  Vector v12 = vector_normalize(vector_subtract(*(triangle->verticies[2]), 
                                *(triangle->verticies[1])));
  Vector v20 = vector_normalize(vector_subtract(*(triangle->verticies[0]), 
                                *(triangle->verticies[2])));
  Vector v21 = vector_normalize(vector_subtract(*(triangle->verticies[1]), 
                                *(triangle->verticies[2])));
  Vector tri_normal = vector_normalize(vector_cross(v01, v02));
  
  if(vector_dot(ray.direction, tri_normal) > 0)
    tri_normal = vector_scale(tri_normal, -1);
  
  double denominator = vector_dot(ray.direction, tri_normal);
  if(denominator == 0)
    return 0;

  time = vector_dot(vector_subtract(*(triangle->verticies[0]), 
                     ray.initial_point), tri_normal) / denominator;

  // If time is positive: check if point is inside triangle
  if(time > 0) {
    Vector p = ray_get_point_of_intersection(ray, time);
    Vector v0p = vector_subtract(p, *(triangle->verticies[0]));
    Vector v1p = vector_subtract(p, *(triangle->verticies[1]));
    Vector v2p = vector_subtract(p, *(triangle->verticies[2]));
    if( vector_dot(vector_cross(v02, v01), vector_cross(v02, v0p)) >= 0 &&
        vector_dot(vector_cross(v10, v12), vector_cross(v10, v1p)) >= 0 &&
        vector_dot(vector_cross(v21, v20), vector_cross(v21, v2p)) >= 0 ) {
      (*intersection)->t = time;
      (*intersection)->ray = ray;
      (*intersection)->normal = tri_normal;
      return 1;
    }
  }
  
  return 0;
}

Pixel raytracer_phong(Intersection *intersection, Scene *scene) {
  Pixel result = create_from_color_temperature(1200);
  
  return result;
}

Intersection *new_intersection(void){
  return (Intersection*)malloc(sizeof(Intersection));
}