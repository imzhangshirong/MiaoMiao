
#ifndef M_IMAGE_H
#define M_IMAGE_H

#include <stdlib.h>
#include <setjmp.h>

extern "C" {
  #include "jpeglib.h"
  #include "jerror.h"
}

struct m_image_data
{                     
  int width;
  int height;
  int components;
  unsigned char * data = NULL;
};

struct m_image_data_block
{
  int index;
  int x;
  int y;
  m_image_data data;
};

struct m_image_data_cluster
{
  int length;
  int width;
  int height;
  m_image_data_block* blocks;
};

struct m_image_rect{
  int left;
  int top;
  int right;
  int bottom;
};

struct m_image_point{
  int x;
  int y;
};

namespace m_image_color{
  static unsigned char YELLOW[] = {255,255,0};
  static unsigned char WHITE[] = {255,255,255};
  static unsigned char BLACK[] = {0,0,0};
  static unsigned char GREEN[] = {0,255,0};
  static unsigned char RED[] = {255,0,0};
  static unsigned char BLUE[] = {0,0,255};
  static unsigned char CYAN[] = {0,255,255};
  static unsigned char MEGENTA[] = {255,0,255};
}

class m_image
{
private:

public:
  static m_image_data create(int width, int height);
    
  static void write_JPEG(char * filename,m_image_data image_data, int quality);
  static m_image_data read_JPEG (char * filename);
  //static void write_BMP(char * filename,m_image_data image_data, int quality);
  static m_image_data read_BMP (char * filename);
  static m_image_data copy_data(m_image_data image_data);
  static m_image_data_cluster slice_block(m_image_data image_data,int size_width,int size_height);

  static unsigned char *  get_rgb(m_image_data image_data,int x,int y);
  static void set_rgb(m_image_data image_data,int x,int y,unsigned char * rgb);
  static void set_rgb(m_image_data image_data,int x,int y,unsigned char r,unsigned char g,unsigned char b);
  static void draw_line(m_image_data image_data,int x1,int y1,int x2,int y2,unsigned char * rgb);
  static void draw_rect(m_image_data image_data,m_image_rect rect,unsigned char * rgb);
  static void fill_rect(m_image_data image_data,m_image_rect rect,unsigned char * rgb);
  static void clear(m_image_data image_data,unsigned char* color);
    
};
#endif