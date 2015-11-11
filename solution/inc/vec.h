#ifndef _VEC_
#define _VEC_

#include <stdlib.h>

typedef struct _vec {
	double x,y,z;
} vec;

vec* new_vec    (void);
int  free_vec   (vec *res);

vec* vec_add_to (vec *res, const vec *b);
vec* vec_add    (const vec *a, const vec *b);

#endif