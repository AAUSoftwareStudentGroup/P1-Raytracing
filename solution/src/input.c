#include "input.h"

#ifndef MAX
#define MAX(a,b) (((a) > (b) ? (a) : (b)))
#endif

#ifndef MIN
#define MIN(a,b) (((a) < (b) ? (a) : (b)))
#endif

int input_parse(int argc, char* argv[], Scene **scene, Camera **camera, Configuration *conf) {
  FILE *fp_model;
  int i;
  if(input_read_command_args(argc, argv, conf) == 0)
    return 0;

  *camera = new_camera(conf->width, conf->height);
  
  if(ply_validate(conf, &fp_model) == 0)
    return 0;
  
  if(ply_init(fp_model, scene) == 0)
    return 0;

  if(ply_parse(fp_model, conf, scene, camera) == 0)
    return 0;

  for(i = 0; i < (*scene)->n_objects; i++) {
    input_build_root_node((*scene)->objects[i]);
  }

  (*scene)->ambient_intensity = create_pixel(0.1,0.1,0.1);

  return 1;
}

int input_read_command_args(int argc, char* argv[], Configuration *conf) {
  int i, int_argument;
  double double_argument;
  char string_argument[256];

  if(argc < 2) {
    printf("Usage: %s [FILE]\n\n  FILE: Path to input-file in ply format\n\n", argv[0]);
    return 0;
  }
  else {
    strcpy(conf->in_file, argv[1]);
  }

  for(i = 2; i < argc - 1; i++) {
    if(argv[i][0] == '-') {
      switch(argv[i][1]) {
        case 'h':
        case 'w':
        case 't':
          sscanf(argv[i+1], "%d", &int_argument);
          break;
        case 'H':
        case 'V':
          sscanf(argv[i+1], "%lf", &double_argument);
          break;
        case 'o':
          sscanf(argv[i+1], "%s", string_argument);
          break;
      }
      switch(argv[i++][1]) {
        case 'h':
          conf->height = int_argument;
          break;
        case 'w':
          conf->width = int_argument;
          break;
        case 't':
          conf->color_temperature = int_argument;
          break;
        case 'H':
          conf->horizontal_angle = double_argument;
          break;
        case 'V':
          conf->vertical_angle = double_argument;
          break;
        case 'o':
          strcpy(conf->out_file, string_argument);
          break;
      }
    }
  }
  return 1;
}

int ply_validate(Configuration *conf, FILE** fp_model) {
  char str[256];

  *fp_model = fopen(conf->in_file, "r");

  if(*fp_model == NULL) {
    printf("ERROR: file %s, could not be opened! %s:%d\n", conf->in_file, __FILE__, __LINE__);
    return 0;
  }

  if(fscanf(*fp_model, "%s", str) != EOF && strcmp(str,"ply") != 0) {
    printf("ERROR: %s is not a ply file! %s:%d\n", conf->in_file, __FILE__, __LINE__);
    return 0;
  }

  return 1;
}

int ply_init(FILE *fp_model, Scene **scene) {
  int n_polygons, n_verticies, n_objects, n_lights, n_triangles;
  int i, j;

  *scene = new_scene();

  ply_scan_element(fp_model, "vertex", &n_verticies);
  ply_scan_element(fp_model, "face", &n_polygons);
  ply_scan_element(fp_model, "object", &n_objects);
  ply_scan_element(fp_model, "light", &n_lights);
  input_file_find_first(fp_model, "end_header");
  input_jump_lines(fp_model, n_verticies+1);

  n_triangles = 0;
  for(i = 0; i < n_polygons; i++) {
    input_read_int(fp_model, &j);
    n_triangles += j-2;
    input_jump_lines(fp_model, 1); // discard rest of line
  }


  (*scene)->n_objects = n_objects;
  (*scene)->objects = (Object**)malloc((*scene)->n_objects * sizeof(Object*));

  // assuming atleast one object
  (*scene)->objects[0] = new_object();
  input_read_int(fp_model, &((*scene)->objects[0]->n_verticies));
  input_read_int(fp_model, &((*scene)->objects[0]->n_triangles));
  input_jump_lines(fp_model, 1); // discard rest of line
  (*scene)->objects[0]->verticies = (Vertex*)malloc(n_verticies*sizeof(Vertex));
  (*scene)->objects[0]->triangles = (Triangle*)malloc(n_triangles*sizeof(Triangle));

  for(i = 1; i < n_objects; i++) {
    (*scene)->objects[i] = new_object();
    input_read_int(fp_model, &((*scene)->objects[i]->n_verticies));
    input_read_int(fp_model, &((*scene)->objects[i]->n_triangles));
    input_jump_lines(fp_model, 1); // discard rest of line
    (*scene)->objects[i]->verticies = (*scene)->objects[i-1]->verticies + (*scene)->objects[i-1]->n_verticies;
    (*scene)->objects[i]->triangles = (*scene)->objects[i-1]->triangles + (*scene)->objects[i-1]->n_triangles;
  }


  (*scene)->n_lights = n_lights;
  (*scene)->lights = (PointLight**)malloc(n_lights*sizeof(PointLight*));
  for(i = 0; i < n_lights; i++) {
    (*scene)->lights[i] = (PointLight*)malloc(sizeof(PointLight));
  }

  return 1;
}

int ply_parse(FILE *fp_model, Configuration *conf, Scene **scene, Camera **camera) {
  PointLight *lamp_source = NULL;
  int i, j, k, triangle_index;
  int n_faces, verticies_in_polygon;
  int *vertex_index_list;
  Triangle t;
  Vertex v;

  ply_scan_element(fp_model, "face", &n_faces);

  input_file_find_first(fp_model, "end_header");
  for(j = 0; j < (*scene)->n_objects; j++) {
    for(i = 0; i < (*scene)->objects[j]->n_verticies; i++) {
      input_read_double(fp_model, &(v.position.x));
      input_read_double(fp_model, &(v.position.y));
      input_read_double(fp_model, &(v.position.z));
      input_read_double(fp_model, &(v.normal.x));
      input_read_double(fp_model, &(v.normal.y));
      input_read_double(fp_model, &(v.normal.z));
      (*scene)->objects[j]->verticies[i] = v;
    }
  }

  triangle_index = 0;
  // foreach polygon in file
  for(i = 0, j = 0; i < n_faces; i++) {
    // read number of vertices
    input_read_int(fp_model, &verticies_in_polygon);
    vertex_index_list = (int*)malloc(verticies_in_polygon * sizeof(int));
    // read each vertex index into temporary storage
    for(j = 0; j < verticies_in_polygon; j++) {
      input_read_int(fp_model, &(vertex_index_list[j]));
    }
    // split into triangles and store in objects list
    for(j = 0; j < verticies_in_polygon-2; j++) {
      Vertex *vertex_list = (*scene)->objects[0]->verticies;
      Vertex **triangle = ((*scene)->objects[0]->triangles[triangle_index].verticies);

      triangle[0] = &(vertex_list[ vertex_index_list[0]   ]);
      triangle[1] = &(vertex_list[ vertex_index_list[j+1] ]);
      triangle[2] = &(vertex_list[ vertex_index_list[j+2] ]);

      (*scene)->objects[0]->triangles[triangle_index].edges[0] = vector_subtract(triangle[1]->position, triangle[0]->position);
      (*scene)->objects[0]->triangles[triangle_index].edges[1] = vector_subtract(triangle[2]->position, triangle[1]->position);
      (*scene)->objects[0]->triangles[triangle_index].edges[2] = vector_subtract(triangle[0]->position, triangle[2]->position);

      triangle_index++;
    }
  }

  for(i = 0; i < (*scene)->n_objects; i++) {
    // skip two first numbers of each row
    input_read_int(fp_model, &j);
    input_read_int(fp_model, &j);
    // read color
    input_read_int(fp_model, &j);
    (*scene)->objects[i]->color.red = (double)j / 255.0;
    input_read_int(fp_model, &j);
    (*scene)->objects[i]->color.green = (double)j / 255.0;
    input_read_int(fp_model, &j);
    (*scene)->objects[i]->color.blue = (double)j / 255.0;
    /*
    property float diffuse_coefficient
    property float specular_coefficient
    property float specular_hardness
    */
    (*scene)->objects[i]->material.ambient_coefficient = 1;
    input_read_double(fp_model, &((*scene)->objects[i]->material.diffuse_coefficient));
    input_read_double(fp_model, &((*scene)->objects[i]->material.specular_coefficient));
    input_read_int(fp_model, &((*scene)->objects[i]->material.smoothness));
    (*scene)->objects[i]->material.metalness = 0.5;
  }

  for(i = 0; i < (*scene)->n_lights; i++) {
    Vector light_position;
    input_read_double(fp_model, &(light_position.x));
    input_read_double(fp_model, &(light_position.y));
    input_read_double(fp_model, &(light_position.z));
    (*scene)->lights[i]->position = light_position;

    input_read_double(fp_model, &((*scene)->lights[i]->intensity));

    input_read_int(fp_model, &j);
    (*scene)->lights[i]->color.red = ((double)j*((*scene)->lights[i]->intensity)) / 255;
    input_read_int(fp_model, &j);
    (*scene)->lights[i]->color.green = ((double)j*((*scene)->lights[i]->intensity)) / 255;
    input_read_int(fp_model, &j);
    (*scene)->lights[i]->color.blue = ((double)j*((*scene)->lights[i]->intensity)) / 255;

    if((*scene)->lights[i]->color.red   == 0 && 
       (*scene)->lights[i]->color.green == 0 &&
       (*scene)->lights[i]->color.blue  == 0) {
      (*scene)->lights[i]->color = create_from_color_temperature(conf->color_temperature);
      lamp_source = (*scene)->lights[i];
    }

    input_read_double(fp_model, &((*scene)->lights[i]->radius) ); // Radius
    input_read_int(fp_model, &((*scene)->lights[i]->sampling_rate)); // sample_size
    if((*scene)->lights[i]->sampling_rate == 1)
      (*scene)->lights[i]->radius = 0;
  }

  input_read_double(fp_model, &((*camera)->position.x));
  input_read_double(fp_model, &((*camera)->position.y));
  input_read_double(fp_model, &((*camera)->position.z));

  if(lamp_source != NULL)
    camera_look_at_point(*camera, lamp_source->position, 
                         vector_norm(vector_subtract(lamp_source->position, (*camera)->position)), 
                         conf->vertical_angle, conf->horizontal_angle);
  else {
    camera_look_at_point(*camera, (Vector){0,0,0}, 
                         vector_norm((*camera)->position), 
                         conf->vertical_angle, conf->horizontal_angle);
  }

  return 1;
}

int ply_scan_element(FILE *file, const char *element_name, int *out) {
  int result;
  int fscan_result;

  fseek(file, 0, SEEK_SET);
  char *search_string = (char*)malloc(12+strlen(element_name));
  strcpy(search_string, "element ");
  strcpy(search_string+8, element_name);
  strcpy(search_string+8+strlen(element_name), " %d");
  // "element vertex %d"

  while((fscan_result = fscanf(file, search_string, &result)) != EOF) {
    if(fscan_result == 1) {
      *out = result;
      return 1;
    }
    else {
        input_jump_lines(file, 1);
    }
  }
  return 0;
}

int input_file_find_first(FILE *file, char *search_str) {
  fseek(file, 0, SEEK_SET);

  input_file_find_next(file, search_str);

  return 0;
}

int input_file_find_next(FILE *file, char *search_str) {
  char str[256];

  while(fscanf(file, "%s", str) != EOF) {
    if(strcmp(str, search_str) == 0)
      return 1;
  }

  return 0;
}

int input_jump_lines(FILE *file, int lines) {
  char c;
  while(lines--) {
    while((c = fgetc(file)) != '\n') {
      if(c == EOF)
        return 0;
    }
  }
  return 1;
}

int input_read_int(FILE *file, int *out) {
  int res = fscanf(file, " %d", out);
  return res;
}

int input_read_double(FILE *file, double *out) {
  int res = fscanf(file, " %lf", out);
  return res;
}

int input_build_root_node(Object *object) {
  int i;
  KDNode *root;

  root = &(object->root);

  root->low = NULL;
  root->high = NULL;

  root->box.low = root->box.high = object->verticies[0].position;

  for(i = 0; i < object->n_verticies; i++){
    root->box.low.x = MIN(root->box.low.x, object->verticies[i].position.x);
    root->box.low.y = MIN(root->box.low.y, object->verticies[i].position.y);
    root->box.low.z = MIN(root->box.low.z, object->verticies[i].position.z);
    root->box.high.x = MAX(root->box.high.x, object->verticies[i].position.x);
    root->box.high.y = MAX(root->box.high.y, object->verticies[i].position.y);
    root->box.high.z = MAX(root->box.high.z, object->verticies[i].position.z);
  }

  root->n_triangles = object->n_triangles;
  root->triangles = (Triangle**)malloc(object->n_triangles*sizeof(Triangle*));

  for(i = 0; i < object->n_triangles; i++) {
    root->triangles[i] = &(object->triangles[i]);
  }

  if(root->n_triangles > 2)
    kdnode_build_subnodes(root, 0);

  return 1;
}

Configuration create_configuration() {
  Configuration conf = {500, 500, 6600, 0, 0, "", ""};
  conf.out_file = (char*)malloc(sizeof(char)*256);
  conf.in_file = (char*)malloc(sizeof(char)*256);
  strcpy(conf.out_file, "out.ppm");
  return conf;
}