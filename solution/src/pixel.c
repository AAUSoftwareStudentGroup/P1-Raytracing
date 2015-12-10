#include "pixel.h"

 /* 
  * The following code is implemented from 
  * http://www.tannerhelland.com/4435/convert-temperature-rgb-algorithm-code/
  * (visited: 19-11-2015)
  * where an algorithm, that converts colortemperature in kelvin to an 
  * RGB-value, is made in pseudocode. The code is 'translated' to C. 
  * The algorithm is not precise enough to be used in a scientific matter, but 
  * is precise enough in our context. The algorithm is only ment to be used with
  * colortemperatures between 1000K and 40000K. The algorithm is made by 
  * plotting Charity's Original Blackbody Values, which can be found here:
  * http://www.vendian.org/mncharity/dir3/blackbody/UnstableURLs/bbr_color.html
  * (visited 19-11-2015).
  */
Pixel create_from_color_temperature(unsigned int kelvin) {

  Pixel color;
  kelvin = kelvin / 100;
  /* Calculate red */
  if(kelvin <= 66){
    color.red = 255;
  }
  else{
    color.red = kelvin - 60;
    color.red = 329.698727446 * (pow(color.red, -0.1332047592));
    if(color.red < 0){
      color.red = 0;
    }
    if(color.red > 255){
      color.red = 255;
    }
  }

  /* Calculate green */
  if(kelvin <= 66){
    color.green = kelvin;
    color.green = 99.4708025861 * log(color.green) - 161.1195681661;
    if(color.green < 0){
      color.green = 0;
    }
    if(color.green > 255){
      color.green = 255;
    }
  }
  else{
    color.green = kelvin - 60;
    color.green = 288.1221695283 * (pow(color.green, -0.0755148492));
    if(color.green < 0){
      color.green = 0;
    }
    if(color.green > 255){
      color.green = 255;
    }
  }

  /* Calculate blue */
  if(kelvin >= 66){
    color.blue = 255;
  }
  else {
    if(kelvin <= 19){
      color.blue = 0;
      }
    else{
      color.blue = kelvin - 10;
      color.blue = 138.5177312231 * log(color.blue) - 305.0447927307;
      if(color.blue < 0){
        color.blue = 0;
      }
      if(color.blue > 255){
        color.blue = 255;
      }
    }
  }
  color = pixel_scale(color, 1.0/255.0);
  return color;
}

/* Makes the pixel-value, that was previously between 0 and 1 into a normal
   RGB-value, that, in this case, is between 0 and 255 */
char pixel_component_to_byte(double component) {
  return (char)((double)(component*255 + 0.5));
}

Pixel pixel_scale(Pixel color, double scalar) {
  return (Pixel){color.red * scalar, color.green * scalar, color.blue * scalar};
}

Pixel pixel_multiply(Pixel color1, Pixel color2) {
  return (Pixel){color1.red * color2.red, color1.green * color2.green, 
    color1.blue * color2.blue};
}

Pixel pixel_add(Pixel color1, Pixel color2){
  return (Pixel){MIN(color1.red + color2.red,1.0), 
    MIN(color1.green + color2.green, 1.0), MIN(color1.blue + color2.blue,1.0)};
}

Pixel create_pixel(double red, double green, double blue) {
  return (Pixel){red, green, blue};
}