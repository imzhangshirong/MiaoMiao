#include "src/COMMON.cpp"
#include "src/MATRIX.cpp"
#include "src/IMG.cpp"
#include "src/EDGE.cpp"
#include "src/PROCESSOR.cpp"

#ifndef MATRIX_PROC_H
#define MATRIX_PROC_H
namespace MATRIX_PROC{
  static MATRIX2 m1(5,5);
  void init_mtrx()
  {
    static float m1_data[] = {
      1.0f,	-1.0f,	1.0f, -1.0f,  1.0f,
      -1.0f,	1.0f,	-1.0f,  1.0f, -1.0f,
      1.0f,	-1.0f,	1.0f, -1.0f,  1.0f,
      -1.0f,	1.0f,	-1.0f,  1.0f, -1.0f,
      1.0f,	-1.0f,	1.0f, -1.0f,  1.0f
    };
    m1.set_array(m1_data);

    std::cout<<"Init Matrix Completed."<<std::endl;
  }
  unsigned char* func1(int x,int y,float value)
  {
    if(value > 0.0f)
    {
      return IMG_COLOR::GREEN;
    }
    else{
      return IMG_COLOR::BLACK;
    }
  }

  unsigned char* func_edge(int x,int y,float value)
  {
    if(value > 0.0f)
    {
      return IMG_COLOR::GREEN;
    }
    else{
      return IMG_COLOR::BLACK;
    }
  }

  float filter1(int x,int y,float value)
  {
    if(value > 3.0f)
    {
      return 1.0f;
    }
    else{
      return 0.0f;
    }
  }
}

#endif