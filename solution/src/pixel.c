#include "pixel.h"

Pixel create_pixel(double red, double green, double blue) {
  return (Pixel){red, green, blue};
}

Pixel create_from_color_temperature(unsigned int kelvin) {
  
/* 
 * Copyright(c)<17.9.2012>,<Tanner Helland>
 * All rights reserved.
 * Website: www.tannerhelland.com
 * URL: http://www.tannerhelland.com/4435/convert-temperature-rgb-algorithm-code/
 * Unless otherwise noted, all text, audio, and video content on this site
 * is licensed under a Creative Commons license permitting 
 * sharing with attribution (CC-BY-3.0). All source code samples and/or 
 * downloads are licensed under a simplified BSD license.
 * The programming code (section: 'The Algorithm') is modified (translated to C).
 */

  Pixel color;
  /* Calculate red */
  if(kelvin <= 6600){
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
  if(kelvin <= 6600){
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
  if(kelvin >= 6600){
    color.blue = 255;
  }
  else {
    if(kelvin <= 1900){
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
  return color;
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