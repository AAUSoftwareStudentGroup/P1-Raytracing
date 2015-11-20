#ifndef _MATERIAL_
#define _MATERIAL_

typedef struct _material {
  double ambient_coefficient;
  double diffuse_coefficient;
  double specular_coefficient;
  int material_smoothness;
  double material_metalness; 
} Material;

#endif