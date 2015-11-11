#ifndef _VECTOR_
#define _VECTOR_

#include <stdlib.h>

typedef struct _vector {
	double x,y,z;
} Vector;

Vector *new_vector (void);
int free_vector (Vector *res);

Vector *vector_add(Vector *v1, const Vector *v2);
Vector *vector_subtract(Vector *v1, const Vector *v2);
Vector *vector_scale(Vector *v, const double s);
double vector_dot(Vector *v1, const Vector *v2);
double vector_norm(const Vector *v);
Vector *vector_normalize(Vector *v);
double *vector_angle_between(const Vector *v1, const Vector *v2);
Vector *vector_cross(Vector v1, const Vector v2);

#endif