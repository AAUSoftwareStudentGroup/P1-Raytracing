#include "image.h"

int image_write(Image *image, char *path) {
  int x, y;
  FILE *image_file;

  image_file = fopen(path, "wb");
  
  fprintf(image_file, "P6 %d %d 255 ", image->width, image->height);
  /* For all pixels put pixel-color */
  for(y = 0; y < image->height; y++) {
    for(x = 0; x < image->width; x++) {
      fputc(pixel_component_to_byte(image->pixels[x][y].red), image_file);
      fputc(pixel_component_to_byte(image->pixels[x][y].green), image_file);
      fputc(pixel_component_to_byte(image->pixels[x][y].blue), image_file);
    }
  }
  fclose(image_file);
  return 1;
}

Image *new_image(unsigned int width, unsigned int height) {
  int x, y;
  Image *image = (Image*)malloc(sizeof(Image));
  
  image->width = width;
  image->height = height;

  image->pixels = (Pixel**)malloc(width * sizeof(Pixel*));
  for(x = 0; x < width; x++) { /* Looping over all columns */
    image->pixels[x] = (Pixel*)malloc(height * sizeof(Pixel));
    for(y = 0; y < height; y++) { /* Looping over all rows */
      image->pixels[x][y] = create_pixel(0.0, 0.0, 0.0);
    }
  }
  return image;
}