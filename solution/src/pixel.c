#include "pixel.h"

Pixel create_pixel(double red, double green, double blue) {
  return (Pixel){red, green, blue};
}

char pixel_component_to_byte(double component) {
  return (char)((double)(component*255 + 0.5));
}