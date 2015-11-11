#ifndef _IMAGE_
#define _IMAGE_

#include <stdio.h>
#include <stdlib.h>

#include "pixel.h"

typedef struct _image {
  unsigned int width, height;
  Pixel **pixels;
} Image;

Image *new_image(unsigned int width, unsigned int height);

int image_write(Image *img, char *path);

#endif