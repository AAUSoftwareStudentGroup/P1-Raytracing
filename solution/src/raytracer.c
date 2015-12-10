#include "raytracer.h"

/* Render an image of scene with perspective of camera */
Image *raytracer_render(Scene *scene, Camera *camera) {
  int x, y;
  Image *image;
  Ray ray;

  image = new_image(camera->width, camera->height);
  
  /* For each pixel */
  for(x = 0; x < camera->width; x++) {
    for(y = 0; y < camera->height; y++) {
      ray = raytracer_calculate_ray(x, y, camera);

      /* Trace ray and assign result to pixel */
      image->pixels[x][y] = raytracer_trace(ray, scene);
    }
    /* print progress percentage */
    printf("%.1f\n", ((double)x + 1) / camera->width * 100);
  }

  return image;
}

/* Calculate ray intersecting pixel (x, y) on camera's image plane */
Ray raytracer_calculate_ray(int x, int y, Camera *camera){
  Vector direction;
  direction = vector_scale(camera->forward, camera->distance);
  direction = vector_add(direction, vector_scale(camera->up, 
                                                 -y + camera->height / 2.0));
  direction = vector_add(direction, vector_scale(camera->right, 
                                                 x - camera->width / 2.0));
                                                 
  return create_ray(camera->position, direction);
}


Pixel raytracer_trace(Ray ray, Scene *scene) {
  Intersection intersection = create_intersection();
  Pixel pixel = {0, 0, 0};
  
  /* If ray intersects with scene: */
  if(raytracer_scene_intersection(ray, scene, &intersection)) {
    /* Shade pixel */
    pixel = raytracer_phong(intersection, scene);
  }
  
  return pixel;
}

int raytracer_scene_intersection(Ray ray, Scene *scene, 
                                 Intersection *intersection) {
  int i;
  Intersection temporary_intersection;

  temporary_intersection = create_intersection();

  /* For each object in scene: */
  for(i = 0; i < scene->n_objects; i++) {
    /* If ray intersects with object: */
    if(raytracer_object_intersection(ray, scene->objects[i], 
       &temporary_intersection))
      /* Reassign intersection if current intersection is closer */
      if(temporary_intersection.t < intersection->t || intersection->t == -1)
        *intersection = temporary_intersection;
  }
  return intersection->t > 0;
}

int raytracer_object_intersection(Ray ray, Object *object, Intersection *intersection) {
  double i, j;
  
  /* if ray intersects with object's aabb: */
  if(intersection_ray_aabb(ray, object->root.box, &i, &j) && 
     raytracer_kdtree_intersection(ray, &(object->root), intersection)) {
    intersection->color = object->color;
    intersection->material = object->material;
  }
  return intersection->t > 0;
}

int raytracer_kdtree_intersection(Ray ray, KDNode *node, Intersection *intersection) {
  int i;
  double t_minl, t_maxl;
  double t_minh, t_maxh;
  double tmin, tmax;
  int retl, reth;
  Intersection temporary_intersection;

  temporary_intersection = create_intersection();
  
  /* if leaf */
  if(kdnode_is_leaf(node)) {
    /* test intersection with geometry */
    for(i = 0; i < node->n_triangles; i++) {
      if(raytracer_triangle_intersection(ray, node->triangles[i], &temporary_intersection)) {
        if(temporary_intersection.t < intersection->t || intersection->t == -1) {
          *intersection = temporary_intersection;
          intersection->triangle = node->triangles[i];
        }
      }
    }
  } else {
    /* test intersection recursively */
    retl = intersection_ray_aabb(ray, node->low->box, &t_minl, &t_maxl);
    reth = intersection_ray_aabb(ray, node->high->box, &t_minh, &t_maxh);

    if(retl && reth) { /* Intersecting both sub-nodes */
      if(t_minh < t_minl) { /* low node is hit first */
        if(!raytracer_kdtree_intersection(ray, node->high, intersection)) {
          raytracer_kdtree_intersection(ray, node->low, intersection);
        }
      } else if(t_minh == t_minl) {
        raytracer_kdtree_intersection(ray, node->low, intersection);
        raytracer_kdtree_intersection(ray, node->high, intersection);
      } else { /* high node is hit first */
        if(!raytracer_kdtree_intersection(ray, node->low, intersection)) {
          raytracer_kdtree_intersection(ray, node->high, intersection);
        }
      }
    } else if(retl) { /* only low node is hit */
        raytracer_kdtree_intersection(ray, node->low, intersection);
    } else { /* only high node is hit */
        raytracer_kdtree_intersection(ray, node->high, intersection);
    }
  }
  return intersection->t > 0;
}

int raytracer_triangle_intersection(Ray ray, Triangle *triangle, Intersection *intersection) {
  double denominator, time;
  Plane plane;
  Vector triangle_normal;
  time = -1;

  triangle_normal = vector_normalize(vector_cross(triangle->edges[0], 
                                                  triangle->edges[1]));

  plane = create_plane(triangle->verticies[0]->position, triangle_normal);

  /* If triangle on front of camera: check if point is inside triangle */
  if(intersection_ray_plane(ray, plane, &time) && time > 0) {
    int i;
    Vector p = ray_get_point(ray, time);
    for(i = 0; i < VERTICES_IN_TRIANGLE; i++)
      if(vector_dot(triangle_normal, vector_cross(triangle->edges[i], 
         vector_subtract(p, triangle->verticies[i]->position))) < 0)
        return 0;
    intersection->t = time;
    intersection->ray = ray;
    if(vector_dot(ray.direction, triangle_normal) > 0)
      triangle_normal = vector_scale(triangle_normal, -1);
    intersection->normal = triangle_normal;
    return 1;
  }

  return 0;
}

Pixel raytracer_phong(Intersection intersection, Scene *scene) {
  int i, j, samples_reached_light;
  double m_a, m_l, m_s, m_sp, m_sm, sampled_light_intensity, distance_light;
  Vector vN, vI, vR, vU, intersection_point;
  Pixel pA, pS, pC, pI, ambient, diffuse, specular;
  Vector light_sample_position;
  Ray r;

  /* initialize */
  m_a = intersection.material.ambient_coefficient;
  m_l = intersection.material.diffuse_coefficient;
  m_s = intersection.material.specular_coefficient;
  m_sp = intersection.material.smoothness;
  m_sm = intersection.material.metalness;
  vN = intersection.normal;
  pC = intersection.color;
  pA = scene->ambient_intensity;
  diffuse = create_pixel(0.0, 0.0, 0.0);
  specular = create_pixel(0.0, 0.0, 0.0);

  /* ambient light = m_a * pC * pA */
  ambient = pixel_multiply(pixel_scale(pC, m_a), pA);

  intersection_point = ray_get_point(intersection.ray, intersection.t);
  vU = vector_scale(intersection.ray.direction, -1.0);
  pS = pixel_add(pixel_scale(pC, m_sm), pixel_scale(create_pixel(1.0,1.0,1.0), (1-m_sm)));

  for(i = 0; i < scene->n_lights; i++) {
    pI = scene->lights[i]->color;
    
    samples_reached_light = 0;

    for(j = 0; j < scene->lights[i]->sampling_rate; j++) {
      light_sample_position = point_light_get_sample(scene->lights[i]);

      vI = vector_normalize(vector_subtract(light_sample_position,
                    intersection_point));
      
      r = create_ray(intersection_point, vI);
      r.initial_point = ray_get_point(r, 0.01);

      if(!raytracer_in_shadow(light_sample_position, r, scene)) {
        samples_reached_light++;
      }
    }
    sampled_light_intensity = (double)(samples_reached_light*
                              scene->lights[i]->intensity) / 
                              scene->lights[i]->sampling_rate;

    // distance_light = vector_norm(vector_subtract(scene->lights[i]->position,
                          // intersection_point));
    // sampled_light_intensity /= distance_light*distance_light;
    
    pI = pixel_scale(pI, sampled_light_intensity);
    vI = vector_normalize(vector_subtract(scene->lights[i]->position,
                          intersection_point));

    vR = vector_normalize(vector_add(vector_scale(vI, -1),
                          vector_scale(vN, vector_dot(vI, vN) * 2)));

    /* diffuse light =  m_l * MAX(vI * vN, 0) * pC * pI*/
    diffuse = pixel_add(diffuse, pixel_multiply(pixel_scale(pC,
                        m_l * MAX(vector_dot(vI, vN), 0)), pI));

    // /* specular light = m_s * MAX(-vR * vU, 0) ^ m_sp * pI * pS */
    specular = pixel_add(specular, pixel_multiply(pS, pixel_scale(pI,
                         m_s * pow(MAX(vector_dot(vR, vU), 0), m_sp))));
  }

  /* return ambient + diffuse + specular */
  return pixel_add(ambient, pixel_add(diffuse, specular));
}

int raytracer_in_shadow(Vector point, Ray r, Scene *scene) {
  Intersection inter;

  inter = create_intersection();

  if(raytracer_scene_intersection(r, scene, &inter)) {
    if(vector_norm(vector_subtract(point, r.initial_point)) > vector_norm(vector_subtract(ray_get_point(r, inter.t), r.initial_point))) {
      return 1;
    }
  }
  return 0;
}

Intersection *new_intersection(void){
  Intersection* inter = (Intersection*)malloc(sizeof(Intersection));
  inter->t = -1;
  return inter;
}

Intersection create_intersection(void){
  Intersection inter;
  inter.t = -1;
  return inter;
}
