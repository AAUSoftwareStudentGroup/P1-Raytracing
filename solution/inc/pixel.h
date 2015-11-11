#ifndef _PIXEL_
#define _PIXEL_

typedef struct _pixel {
  double red, green, blue;
} Pixel;

Pixel create_pixel(double red, double green, double blue);
char pixel_component_to_byte(double);

#endif