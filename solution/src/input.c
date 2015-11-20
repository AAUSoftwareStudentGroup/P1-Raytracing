#include "input.h"

int input_parse(int argc, char* argv[], Scene **scene, Camera **camera) {
  FILE *fp_model;
  *camera = new_camera(1000, 1000);
  if(ply_validate(argc, argv, &fp_model) == 0)
    return 0;
  if(ply_init(fp_model, scene) == 0)
    return 0;
  return ply_parse(fp_model, scene);

  /*
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
  
  */

  return 1;
}

int ply_validate(int argc, char* argv[], FILE** fp_model) {
  char str[256];
  
  
  if(argc != 2) {
    printf("Not exactly two arguments %d:%s", __LINE__, __FILE__);
    return 0;
  }

  *fp_model = fopen(argv[1], "r");
  
  if(*fp_model == NULL) {
    printf("file %s, could not be opened %d:%s", argv[1], __LINE__, __FILE__);
    return 0;
  }

  if(fscanf(*fp_model, "%s", str) != EOF && strcmp(str,"ply") != 0) {
    printf("%s is not a ply file %d:%s", argv[1], __LINE__, __FILE__);
    return 0;
  }

  return 1;
}

int ply_init(FILE *fp_model, Scene **scene) {
  int n_polygons, n_verticies, n_triangles;
  int i, j;

  // assumed valid as ply_validate() passed

  *scene = new_scene();
  
  (*scene)->n_objects = 1;
  (*scene)->objects = (Object**)malloc((*scene)->n_objects * sizeof(Object*));
  (*scene)->objects[0] = new_object();

  ply_scan_element(fp_model, "vertex", &n_verticies);
  ply_scan_element(fp_model, "face", &n_polygons);
  input_file_find_first(fp_model, "end_header");
  input_jump_lines(fp_model, n_verticies+1);

  n_triangles = 0;
  for(i = 0; i < n_polygons; i++) {
    input_read_int(fp_model, &j);
    n_triangles += j-2;
    input_jump_lines(fp_model, 1); // discard rest of line
  }

  (*scene)->objects[0]->n_verticies = n_verticies;
  (*scene)->objects[0]->verticies = (Vertex*)malloc(n_verticies*sizeof(Vertex));
  (*scene)->objects[0]->n_triangles = n_triangles;
  (*scene)->objects[0]->triangles = (Triangle*)malloc(n_triangles*sizeof(Triangle));
  return 1;
}

int ply_parse(FILE *fp_model, Scene **scene) {
  int i, j, triangle_index;
  int n_faces, verticies_in_polygon;
  int *vertex_index_list;
  Triangle t;
  Vertex v;


  ply_scan_element(fp_model, "face", &n_faces);

  input_file_find_first(fp_model, "end_header");
  for(i = 0; i < (*scene)->objects[0]->n_verticies; i++) {
    input_read_double(fp_model, &(v.position.x));
    input_read_double(fp_model, &(v.position.y));
    input_read_double(fp_model, &(v.position.z));
    input_read_double(fp_model, &(v.normal.x));
    input_read_double(fp_model, &(v.normal.y));
    input_read_double(fp_model, &(v.normal.z));
    (*scene)->objects[0]->verticies[i] = v;
  }


  triangle_index = 0;
  for(i = 0, j = 0; i < n_faces; i++) {
    input_read_int(fp_model, &verticies_in_polygon);
    vertex_index_list = (int*)malloc(verticies_in_polygon * sizeof(int));
    for(j = 0; j < verticies_in_polygon; j++) {
      input_read_int(fp_model, &(vertex_index_list[j]));
    }
    for(j = 0; j < verticies_in_polygon-2; j++) {
      Vertex *vertex_list = (*scene)->objects[0]->verticies;
      Vertex **foo = ((*scene)->objects[0]->triangles[triangle_index].verticies);

      foo[0] = &(vertex_list[ vertex_index_list[0]   ]);
      foo[1] = &(vertex_list[ vertex_index_list[j+1] ]);
      foo[2] = &(vertex_list[ vertex_index_list[j+2] ]);
      triangle_index++;
    }
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
  return fscanf(file, " %d", out);
}

int input_read_double(FILE *file, double *out) {
  return fscanf(file, " %lf", out);
}