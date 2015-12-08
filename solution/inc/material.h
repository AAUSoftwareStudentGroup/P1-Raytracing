#ifndef _MATERIAL_
#define _MATERIAL_

typedef struct _material {
  double ambient_coefficient;
  double diffuse_coefficient;
  double specular_coefficient;
  int smoothness;
  double metalness; 
} Material;

#endif