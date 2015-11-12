#include "vector.h"

Vector vector_add(Vector v1, Vector v2) {
  return (Vector){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

Vector vector_subtract(Vector v1, Vector v2) {
  return (Vector){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

Vector vector_scale (Vector v, double s) {
  return (Vector){v.x * s, v.y * s, v.z * s};
}

double vector_dot (Vector v1, Vector v2) {
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

double vector_norm(Vector v) {
  return sqrt(vector_dot(v, v));
}

Vector vector_normalize(Vector v) {
  return vector_scale(v, 1/vector_norm(v));
}

double vector_angle_between(Vector v1, Vector v2) {
  return acos(vector_dot(v1, v2)/(vector_norm(v1) * vector_norm(v2))); // Hvorfor ikke bare vector_norm(v)?
}

Vector vector_cross(Vector v1, Vector v2) {
  return (Vector){(v1.y * v2.z) - (v1.z * v2.y), (v1.z * v2.x) - (v1.x * v2.x), (v1.x * v2.y) - (v1.y * v1.x)}; // Går ud fra det er denne fremgangsmåde?
}