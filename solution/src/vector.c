#include "vector.h"

Vector *new_vector(void) {
  return (Vector*)malloc(sizeof(Vector));
}

int free_vector(Vector *result) {
  free(result);
  return 1;
}

Vector *vector_add(Vector *v1, const Vector *v2) {
  v1->x += v2->x;
  v1->y += v2->y;
	v1->z += v2->z;
  return v1;
}

Vector *vector_subtract (Vector *v1, const Vector *v2) {
  return v1;
}

Vector *vector_scale (Vector *v, const double s) {
  return v;
}

double vector_dot (Vector *v1, const Vector *v2) {
  return 0;
}

double vector_norm(const Vector *v) {
  return 0;
}

Vector *vector_normalize(Vector *v) {
  return v;
}

double *vector_angle_between(const Vector *v1, const Vector *v2) {
  return 0;
}

Vector *vector_cross(Vector v1, const Vector v2) {
  return v1;
}