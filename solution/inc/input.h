#ifndef _INPUT_
#define _INPUT_

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scene.h"
#include "camera.h"

typedef struct {
  int width;
  int height;
  int color_temperature;
  double horizontal_angle;
  double vertical_angle;
  char *in_file;
  char *out_file;
} Configuration;

int input_parse(int argc, char* argv[], Scene **scene, Camera **camera, Configuration *conf);
int input_read_command_args(int argc, char* argv[], Configuration *conf);
int ply_validate(Configuration *conf, FILE** fp_model);
int ply_init(FILE *fp_model, Scene **scene);
int ply_parse(FILE *fp_model, Configuration *conf, Scene **scene, Camera **camera);
int ply_scan_element(FILE *file, const char *element_name, int *out);
int input_file_find_first(FILE *file, char *search_str);
int input_file_find_next(FILE *file, char *search_str);
int input_jump_lines(FILE *file, int lines);
int input_read_int(FILE *file, int *out);
int input_read_double(FILE *file, double *out);
int input_build_root_node(Object *objects);
Configuration create_configuration();

#endif