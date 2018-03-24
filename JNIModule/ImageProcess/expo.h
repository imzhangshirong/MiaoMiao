#include "math.h"
#include "src/m_matrix.cpp"
#include "src/m_complex.cpp"
#include "src/m_matrixc.cpp"
#include "src/m_image.cpp"
#include "src/m_vert2.cpp"
#include "src/m_processor.cpp"
#include "src/EDGE.cpp"
#include "src/COMMON.cpp"

#ifndef MATRIX_PROC_H
#define MATRIX_PROC_H
namespace MATRIX_PROC{
  static m_matrix m1(5,5);
  void init_mtrx()
  {
    static float m1_data[] = {
      1.0f,	-1.0f,	1.0f, -1.0f,  1.0f,
      -1.0f,	1.0f,	-1.0f,  1.0f, -1.0f,
      1.0f,	-1.0f,	1.0f, -1.0f,  1.0f,
      -1.0f,	1.0f,	-1.0f,  1.0f, -1.0f,
      1.0f,	-1.0f,	1.0f, -1.0f,  1.0f
    };
    m1.set_array(m1_data,25);

    std::cout<<"Init Matrix Completed."<<std::endl;
  }
  unsigned char* dFunc_words(int x,int y,float value)
  {
    if(value > 0.0f)
    {
      return m_image_color::GREEN;
    }
    else{
      return m_image_color::BLACK;
    }
  }

  unsigned char* dFunc_binary(int x,int y,float value)
  {
    if(value > 0.0f)
    {
      return m_image_color::WHITE;
    }
    else{
      return m_image_color::BLACK;
    }
  }

  unsigned char* dFunc_gray(int x,int y,float value)
  {
    unsigned char gray = (unsigned char)value;

    unsigned char* color = new unsigned char[3];
    color[0] = gray;
    color[1] = gray;
    color[2] = gray;
    return color;
  }

  float filter_words(int x,int y,float value)
  {
    if(value > 3.0f)
    {
      return 1.0f;
    }
    else{
      return 0.0f;
    }
  }

  float mapper_DFT(int x,int y,float value)
  {
    float k = (abs(value)+1);
    return k;
  }

  float filter_DFT(int x,int y,float value)
  {
    return (value<60)?0:255;
  }
}

#endif