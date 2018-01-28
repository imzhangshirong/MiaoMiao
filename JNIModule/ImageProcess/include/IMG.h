#include <stdlib.h>
#include <setjmp.h>

extern "C" {
  #include "jpeglib.h"
  #include "jerror.h"
}
#ifndef IMG_H
#define IMG_H
struct IMG_DATA
{                     
  int width;
  int height;
  int components;
  unsigned char * data = NULL;
};

struct IMG_DATA_BLOCK
{
  int index;
  int x;
  int y;
  IMG_DATA data;
};

struct IMG_DATA_CLUSTER
{
  int length;
  int width;
  int height;
  IMG_DATA_BLOCK* blocks;
};

struct IMG_RECT{
  int left;
  int top;
  int right;
  int bottom;
};

struct IMG_POINT{
  int x;
  int y;
};

namespace IMG_COLOR{
  static unsigned char YELLOW[] = {255,255,0};
  static unsigned char WHITE[] = {255,255,255};
  static unsigned char BLACK[] = {0,0,0};
  static unsigned char GREEN[] = {0,255,0};
  static unsigned char RED[] = {255,0,0};
  static unsigned char BLUE[] = {0,0,255};
  static unsigned char CYAN[] = {0,255,255};
  static unsigned char MEGENTA[] = {255,0,255};
}

class IMG
{
private:

public:
  static IMG_DATA create(int width, int height);
    
  static void write_JPEG(char * filename,IMG_DATA image_data, int quality);
  static IMG_DATA read_JPEG (char * filename);
  //static void write_BMP(char * filename,IMG_DATA image_data, int quality);
  static IMG_DATA read_BMP (char * filename);
  static IMG_DATA copy_data(IMG_DATA image_data);
  static IMG_DATA_CLUSTER slice_block(IMG_DATA image_data,int size_width,int size_height);

  static unsigned char *  get_rgb(IMG_DATA image_data,int x,int y);
  static void set_rgb(IMG_DATA image_data,int x,int y,unsigned char * rgb);
  static void set_rgb(IMG_DATA image_data,int x,int y,unsigned char r,unsigned char g,unsigned char b);
  static void draw_line(IMG_DATA image_data,int x1,int y1,int x2,int y2,unsigned char * rgb);
  static void draw_rect(IMG_DATA image_data,IMG_RECT rect,unsigned char * rgb);
  static void fill_rect(IMG_DATA image_data,IMG_RECT rect,unsigned char * rgb);
  static void clear(IMG_DATA image_data,unsigned char* color);
    
};
#endif