#include "plane.h"

Plane create_plane(Vector point, Vector normal) {
  return (Plane){point, normal};
}