#ifndef _IMAGE_
#define _IMAGE_

#include <stdlib.h>

typedef struct _image {
  
} Image;

Image *new_image(void);

int image_write(Image *img, char *path);

#endif