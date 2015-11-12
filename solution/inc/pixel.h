#ifndef _PIXEL_
#define _PIXEL_

typedef struct _pixel {
  double red, green, blue;
} Pixel;

Pixel create_pixel(double red, double green, double blue);
Pixel create_from_color_temperature(unsigned int kelvin);
char pixel_component_to_byte(double);
Pixel pixel_scale(Pixel color, double scalar);
Pixel pixel_multiply(Pixel color1, Pixel color2);

#endif