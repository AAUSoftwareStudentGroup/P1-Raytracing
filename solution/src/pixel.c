#include "pixel.h"

Pixel create_pixel(double red, double green, double blue) {
  return (Pixel){red, green, blue};
}

char pixel_component_to_byte(double component) {
  return (char)((double)(component*255 + 0.5));
}

Pixel pixel_scale(Pixel color, double scalar) {
  return (Pixel){color.red * scalar, color.green * scalar, color.blue * scalar};
}

Pixel pixel_multiply(Pixel color1, Pixel color2) {
  return (Pixel){color1.red * color2.red, color1.green * color2.green, color1.blue * color2.blue};
}