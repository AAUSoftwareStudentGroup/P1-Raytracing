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

  Vector v01 = vector_normalize(vector_subtract(triangle->verticies[1]->position, 
                                triangle->verticies[0]->position));
  Vector v02 = vector_normalize(vector_subtract(triangle->verticies[2]->position, 
                                triangle->verticies[0]->position));
  Vector v10 = vector_normalize(vector_subtract(triangle->verticies[0]->position, 
                                triangle->verticies[1]->position));
  Vector v12 = vector_normalize(vector_subtract(triangle->verticies[2]->position, 
                                triangle->verticies[1]->position));
  Vector v20 = vector_normalize(vector_subtract(triangle->verticies[0]->position, 
                                triangle->verticies[2]->position));
  Vector v21 = vector_normalize(vector_subtract(triangle->verticies[1]->position, 
                                triangle->verticies[2]->position));
  Vector tri_normal = vector_normalize(vector_cross(v01, v02));
  
  if(vector_dot(ray.direction, tri_normal) > 0)
    tri_normal = vector_scale(tri_normal, -1);
  
  double denominator = vector_dot(ray.direction, tri_normal);
  if(denominator == 0)
    return 0;

  time = vector_dot(vector_subtract(triangle->verticies[0]->position, 
                     ray.initial_point), tri_normal) / denominator;

  // If time is positive: check if point is inside triangle
  if(time > 0) {
    Vector p = ray_get_point(ray, time);
    Vector v0p = vector_subtract(p, triangle->verticies[0]->position);
    Vector v1p = vector_subtract(p, triangle->verticies[1]->position);
    Vector v2p = vector_subtract(p, triangle->verticies[2]->position);
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
  
  Vector intersection_point = ray_get_point(intersection->ray, intersection->t);
  
  double m_a = intersection->material.ambient_coefficient;
  Pixel C = intersection->color;
  Pixel A = scene->ambient_intensity;
  double m_l = intersection->material.diffuse_coefficient;
  double m_s = intersection->material.specular_coefficient;
  double m_sp = intersection->material.material_smoothness;
  Vector vN = vector_normalize(intersection->normal);
  Pixel S, ambient, diffuse, specular, I;
  Vector vI, vR, vU;
  double temp, temp2;
  int i, j;
  double m_sm = intersection->material.material_metalness;
  Pixel P = create_pixel(0,0,0);
  
  
  ambient = pixel_multiply(pixel_scale(C, m_a), A);
  vU = vector_scale(intersection->ray.direction, 1.0);
  S = pixel_add(pixel_scale(C, m_sm), pixel_scale(create_pixel(1.0,1.0,1.0),(1-m_sm)));

  
  P = pixel_add(P, ambient);
  
  for(i=0; i<scene->n_lights; i++){
    I = scene->lights[i]->intensity;
    vI = vector_normalize(vector_subtract(scene->lights[i]->position, intersection_point));
    vR = vector_normalize(vector_add(vector_scale(vI, -1), vector_scale(vN, vector_dot(vI, vN) * 2)));
    diffuse = pixel_multiply(pixel_scale(C, m_l * MAX(vector_dot(vI, vN), 0)), I);
    // specular = pixel_multiply(I, pixel_scale(S, m_s * MAX(0,vector_dot(vN, vector_bisector(vI, vU)))) );
    specular = pixel_multiply(S, pixel_scale(I, m_s * pow(MAX(-vector_dot(vR, vU), 0), m_sp)));
    P = pixel_add(P, diffuse);
    P = pixel_add(P, specular);
  }   
  
  P = pixel_add(ambient, pixel_add(diffuse, specular));
  return P;
}

Intersection *new_intersection(void){
  return (Intersection*)malloc(sizeof(Intersection));
}