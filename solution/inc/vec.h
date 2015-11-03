#ifndef _VEC_
#define _VEC_

#include <stdlib.h>

typedef struct _vec {
	double x,y,z;
} vec;

vec* new_vec    (void);
int  free_vec   (vec *res);

int  vec_add_to (vec *res, const vec *b);
int  vec_add    (vec *res, const vec *a, const vec *b);

#endif