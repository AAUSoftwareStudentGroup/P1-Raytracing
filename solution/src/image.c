#include "image.h"

Image *new_image(void) {
  return (Image*)malloc(sizeof(Image));
}

int image_write(Image *img, char *path) {
  return 1;
}