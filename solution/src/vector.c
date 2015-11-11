#include "vector.h"

Vector vector_add(Vector v1, Vector v2) {
  return (Vector){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

Vector vector_subtract (Vector v1, Vector v2) {
  return v1;
}

Vector vector_scale (Vector v, double s) {
  return v;
}

double vector_dot (Vector v1, Vector v2) {
  return 0;
}

double vector_norm(Vector v) {
  return 0;
}

Vector vector_normalize(Vector v) {
  return v;
}

double vector_angle_between(Vector v1, Vector v2) {
  return 0;
}

Vector vector_cross(Vector v1, Vector v2) {
  return v1;
}