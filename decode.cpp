
/*
LodePNG Examples

Copyright (c) 2005-2012 Lode Vandevenne

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

    3. This notice may not be removed or altered from any source
    distribution.
*/

#include "lodepng.h"
#include <iostream>
#include <cstdio>

/*
3 ways to decode a PNG from a file to RGBA pixel data (and 2 in-memory ways).
*/

//g++ lodepng.cpp example_decode.cpp -ansi -pedantic -Wall -Wextra -O3


//Example 1
//Decode from disk to raw pixels with a single function call
void decodeOneStep(const char* filename) {
  std::vector<unsigned char> image; //the raw pixels
  unsigned width, height;

  //decode
  unsigned error = lodepng::decode(image, width, height, filename);

  //if there's an error, display it
  if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

  //the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...
}

//Example 2
//Load PNG file from disk to memory first, then decode to raw pixels in memory.
void decodeTwoSteps(const char* filename) {
  std::vector<unsigned char> png;
  std::vector<unsigned char> image; //the raw pixels
  unsigned width, height;

  //load and decode
  unsigned error = lodepng::load_file(png, filename);
  if(!error) error = lodepng::decode(image, width, height, png);

  //if there's an error, display it
  if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

  //the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...
}

//Example 3
//Load PNG file from disk using a State, normally needed for more advanced usage.
void decodeWithState(const char* filename) {
  std::vector<unsigned char> png;
  std::vector<unsigned char> image; //the raw pixels
  unsigned width, height;
  lodepng::State state; //optionally customize this one

  unsigned error = lodepng::load_file(png, filename); //load the image file with given filename
  if(!error) error = lodepng::decode(image, width, height, state, png);

  //if there's an error, display it
  if(error) std::cout << "decoder error " << error << ": "<< lodepng_error_text(error) << std::endl;

  //the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...
  //State state contains extra information about the PNG such as text chunks, ...
  std::cout << image.size()  << std::endl;
  std::cout << "width " << width << std::endl ;
  std::cout << "height " << height << std::endl ;
  int cl  = 100; //换行
  int id = 1;
  auto p = image.data();
  for(int i=0;i<=image.size()-1;++i){
      //printf("%x\n",reinterpret_cast<unsigned long long>(p));
      
      int x = *(p+i) & 0xff;

      //printf("0x%02x,",*(p+i));
      printf("0x%02x,",x);
      id++;
      if( id == cl){
          id = 1;
          printf("\n");
      }
  }
}

int main(int argc, char *argv[]) {
  const char* filename = argc > 1 ? argv[1] : "test.png";

  decodeWithState(filename);
}

