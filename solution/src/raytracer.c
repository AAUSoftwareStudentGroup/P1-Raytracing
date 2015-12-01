#include "raytracer.h"

Image* raytracer_render(Scene* scene, Camera *camera) {
  int x, y;
  Image *image;
  Ray ray;
  srand(time(NULL));

  image = new_image(camera->width, camera->height);
  for(x = 0; x < camera->width; x++) {
    for(y = 0; y < camera->height; y++) {
      // beregn ray
      ray = calculate_ray(x, y, camera);

      // trace ray
      image->pixels[x][y] = raytracer_trace(ray, scene);
    }
    printf("%.1f\n", ((double)x + 1) / camera->width * 100);
  }

  return image;
}

Ray calculate_ray(int x, int y, Camera *camera){
  Vector direction;
  direction = vector_scale(camera->forward, camera->distance);
  direction = vector_add(direction, vector_scale(camera->up, -y + camera->height / 2.0));
  direction = vector_add(direction, vector_scale(camera->right, x - camera->width / 2.0));
  return create_ray(camera->position, direction);
}

Pixel raytracer_trace(Ray ray, Scene *scene) {
  Intersection *intersection;
  Pixel pixel = {0.05, 0.05, 0.05};
  if( raytracer_scene_intersection(ray, scene, &intersection) ) {
    pixel = raytracer_phongv2(intersection, scene);
  }
  return pixel;
}

int raytracer_scene_intersection(Ray ray, Scene *scene, Intersection **intersection) {
  int i;
  double lowest_t = -1;
  Intersection* temporary_intersection = new_intersection();

  for(i = 0; i < scene->n_objects; i++) {
    if(raytracer_ray_is_intersecting_bounding_sphere(ray, scene->objects[i]->bounding_volume) && raytracer_object_intersection(ray, scene->objects[i], &temporary_intersection)) {
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
  double denominator, time;
  Vector v01, v02, v10, v12, v20, v21, triangle_normal;
  time = -1;

  v01 = vector_normalize(vector_subtract(triangle->verticies[1]->position,
                                triangle->verticies[0]->position));
  v12 = vector_normalize(vector_subtract(triangle->verticies[2]->position,
                                triangle->verticies[1]->position));
  v20 = vector_normalize(vector_subtract(triangle->verticies[0]->position,
                                triangle->verticies[2]->position));
  triangle_normal = vector_normalize(vector_cross(v01, v12));

  denominator = vector_dot(ray.direction, triangle_normal);

  if(denominator == 0)
    return 0;

  time = vector_dot(vector_subtract(triangle->verticies[0]->position,
                     ray.initial_point), triangle_normal) / denominator;

  // If triangle on front of camera: check if point is inside triangle
  if(time > 0) {
    Vector p = ray_get_point(ray, time);
    Vector v0p = vector_subtract(p, triangle->verticies[0]->position);
    Vector v1p = vector_subtract(p, triangle->verticies[1]->position);
    Vector v2p = vector_subtract(p, triangle->verticies[2]->position);
    if( vector_dot(triangle_normal, vector_cross(v01, v0p)) >= -0.0000000000000001 &&
        vector_dot(triangle_normal, vector_cross(v12, v1p)) >= -0.0000000000000001 &&
        vector_dot(triangle_normal, vector_cross(v20, v2p)) >= -0.0000000000000001 ) {
      (*intersection)->t = time;
      (*intersection)->ray = ray;
      if(vector_dot(ray.direction, triangle_normal) > 0)
        triangle_normal = vector_scale(triangle_normal, -1);
      (*intersection)->normal = triangle_normal;
      return 1;
    }
  }

  return 0;
}

int raytracer_ray_is_intersecting_bounding_sphere(Ray r, Sphere bounding_sphere) {
  Vector difference, projection;

  difference = vector_subtract(bounding_sphere.center, r.initial_point);
  // ray_origin is inside sphere
  if(vector_norm(difference) <= bounding_sphere.radius)
    return 1;
  // sphere is not behind ray
  else if(vector_dot(difference, r.direction) > 0) {
    projection = vector_scale(r.direction, vector_dot(difference, r.direction));
    if(vector_norm(vector_subtract(difference, projection)) <= bounding_sphere.radius)
      return 1;
  }
  return 0;
}

Pixel raytracer_phong(Intersection *intersection, Scene *scene) {
  int i, j;
  double m_a, m_l, m_s, m_sp, m_sm;
  Vector vN, vI, vR, vU, intersection_point;
  Pixel pA, pS, pC, pI, ambient, diffuse, specular;

  /* initialize */
  m_a = intersection->material.ambient_coefficient;
  m_l = intersection->material.diffuse_coefficient;
  m_s = intersection->material.specular_coefficient;
  m_sp = intersection->material.material_smoothness;
  m_sm = intersection->material.material_metalness;
  vN = intersection->normal;
  pC = intersection->color;
  pA = scene->ambient_intensity;
  diffuse = create_pixel(0.0, 0.0, 0.0);
  specular = create_pixel(0.0, 0.0, 0.0);

  /* ambient light = m_a * pC * pA */
  ambient = pixel_multiply(pixel_scale(pC, m_a), pA);

  intersection_point = ray_get_point(intersection->ray, intersection->t);
  vU = vector_scale(intersection->ray.direction, -1.0);
  pS = pixel_add(pixel_scale(pC, m_sm), pixel_scale(create_pixel(1.0,1.0,1.0),(1-m_sm)));

  for(i = 0; i < scene->n_lights; i++) {
    pI = scene->lights[i]->color;
    vI = vector_normalize(vector_subtract(scene->lights[i]->position,
                          intersection_point));
    Intersection* inter = new_intersection();
    Ray r = create_ray(intersection_point, vI);
    r.initial_point = ray_get_point(r, 0.01);

    if(!raytracer_scene_intersection(r, scene, &inter) || vector_norm(vector_subtract(scene->lights[i]->position, r.initial_point)) < vector_norm(vector_subtract(ray_get_point(r, inter->t), r.initial_point))) {
      vR = vector_normalize(vector_add(vector_scale(vI, -1),
                            vector_scale(vN, vector_dot(vI, vN) * 2)));

      /* diffuse light =  m_l * MAX(vI * vN, 0) * pC * pI*/
      diffuse = pixel_add(diffuse, pixel_multiply(pixel_scale(pC,
                          m_l * MAX(vector_dot(vI, vN), 0)), pI));

      /* specular light = m_s * MAX(-vR * vU, 0) ^ m_sp * pI * pS */
      specular = pixel_add(specular, pixel_multiply(pS, pixel_scale(pI,
                           m_s * pow(MAX(vector_dot(vR, vU), 0), m_sp))));
    }
  }

  /* return ambient + diffuse + specular */
  return pixel_add(ambient, pixel_add(diffuse, specular));
}

Pixel raytracer_phongv2(Intersection *intersection, Scene *scene) {
  int i, j;
  double m_a, m_l, m_s, m_sp, m_sm;
  Vector vN, vI, vR, vU, intersection_point;
  Pixel pA, pS, pC, pI, ambient, diffuse, specular;

  /* initialize */
  m_a = intersection->material.ambient_coefficient;
  m_l = intersection->material.diffuse_coefficient;
  m_s = intersection->material.specular_coefficient;
  m_sp = intersection->material.material_smoothness;
  m_sm = intersection->material.material_metalness;
  vN = intersection->normal;
  pC = intersection->color;
  pA = scene->ambient_intensity;
  diffuse = create_pixel(0.0, 0.0, 0.0);
  specular = create_pixel(0.0, 0.0, 0.0);

  /* ambient light = m_a * pC * pA */
  ambient = pixel_multiply(pixel_scale(pC, m_a), pA);

  intersection_point = ray_get_point(intersection->ray, intersection->t);
  vU = vector_scale(intersection->ray.direction, -1.0);
  pS = pixel_add(pixel_scale(pC, m_sm), pixel_scale(create_pixel(1.0,1.0,1.0),(1-m_sm)));

  PointLight* sampled_lights;
  int n_sampled_lights;
  get_sampled_lights(*scene->lights, scene->n_lights, &sampled_lights, &n_sampled_lights);

  for(i = 0; i < n_sampled_lights; i++) {
    pI = sampled_lights[i].color;
    vI = vector_normalize(vector_subtract(sampled_lights[i].position,
                          intersection_point));
    Intersection* inter = new_intersection();
    Ray r = create_ray(intersection_point, vI);
    r.initial_point = ray_get_point(r, 0.01);

    if(!raytracer_scene_intersection(r, scene, &inter) || vector_norm(vector_subtract(sampled_lights[i].position, r.initial_point)) < vector_norm(vector_subtract(ray_get_point(r, inter->t), r.initial_point))) {
      vR = vector_normalize(vector_add(vector_scale(vI, -1),
                            vector_scale(vN, vector_dot(vI, vN) * 2)));

      /* diffuse light =  m_l * MAX(vI * vN, 0) * pC * pI*/
      diffuse = pixel_add(diffuse, pixel_multiply(pixel_scale(pC,
                          m_l * MAX(vector_dot(vI, vN), 0)), pI));

      /* specular light = m_s * MAX(-vR * vU, 0) ^ m_sp * pI * pS */
      specular = pixel_add(specular, pixel_multiply(pS, pixel_scale(pI,
                           m_s * pow(MAX(vector_dot(vR, vU), 0), m_sp))));
    }
  }

  /* return ambient + diffuse + specular */
  return pixel_add(ambient, pixel_add(diffuse, specular));
}

void get_sampled_lights(PointLight* lights, int n_lights, PointLight** light_out, int* n_sampled_lights) {
  int i;
  for (i = 0; i < n_lights; ++i) {
    *n_sampled_lights += lights->sampling_rate;
  }
  printf("GAH");
  *light_out = (PointLight*)malloc(sizeof(PointLight) * (*n_sampled_lights));
  printf("GAH2");
  Vector point;
  int sampled_points = 0;

  for (i = 0; i < n_lights; ++i) {
    if(lights[i].sampling_rate > 1) {
      while(sampled_points < lights[i].sampling_rate) {
        point.x = ((double)rand()/(double)RAND_MAX) * lights[i].radius;
        point.y = ((double)rand()/(double)RAND_MAX) * lights[i].radius;
        point.z = ((double)rand()/(double)RAND_MAX) * lights[i].radius;

        if(vector_norm(point) <= lights[i].radius) {
          (*light_out)[sampled_points] = lights[i];
          (*light_out)[sampled_points].intensity /= (*light_out)[sampled_points].sampling_rate;
          (*light_out)[sampled_points].position = point;
          sampled_points++;
        }
      }
    } else {
      (*light_out[sampled_points]) = lights[i];
      sampled_points++;
    }
  }
}

Intersection *new_intersection(void){
  Intersection* inter = (Intersection*)malloc(sizeof(Intersection));
  inter->t = -1;
  return inter;
}
