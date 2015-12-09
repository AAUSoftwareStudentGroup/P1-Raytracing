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
  return acos(vector_dot(v1, v2)/(vector_norm(v1) * vector_norm(v2))); 
}

Vector vector_cross(Vector v1, Vector v2) {
  Vector v;
  v.x = (v1.y*v2.z) - (v1.z*v2.y);
  v.y = (v1.z*v2.x) - (v1.x*v2.z);
  v.z = (v1.x*v2.y) - (v1.y*v2.x);
  return v;
}

Vector vector_rotate_around_z(Vector v, double angle){
  return (Vector){cos(angle) * v.x - sin(angle) * v.y, sin(angle) * v.x + 
    cos(angle) * v.y, v.z};
}

Vector vector_rotate_around_x(Vector v, double angle){
  return (Vector){v.x, cos(angle) * v.y - sin(angle) * v.z, sin(angle) * v.y + 
    cos(angle) * v.z};  
}

Vector vector_rotate_around_xz(Vector v, double vertical, double horizontal) {
  v = vector_rotate_around_x(v, vertical);
  v = vector_rotate_around_z(v, horizontal);
  return v;
}