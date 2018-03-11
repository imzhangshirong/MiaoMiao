#include "src/COMMON.cpp"
#include "src/MATRIX.cpp"
#include "src/IMG.cpp"
#include "src/EDGE.cpp"
#include "src/PROCESSOR.cpp"
#include "src/VERT.cpp"
#include "math.h"

#ifndef VERT_PROC_H
#define VERT_PROC_H
namespace VERT_PROC{
  const float dTheta = 5;
  const int n_theta = (int)(180/dTheta)+1;
  const float D_PI = M_PI/180.0f;
  static VERT2 direct_verts[n_theta];
  static float direct_verts_r[n_theta];
  void init_vert()
  {
    float d = -90.0f;
    float rad;
    for(int i=0;i<n_theta;i++)
    {
      rad = d*D_PI;
      direct_verts[i]=VERT2(cosf(rad),sinf(rad));
      direct_verts_r[i]=d;
      d+=dTheta;
    }
  }
  void func_vert(IMG_DATA image_data,int x,int y,float value)
  {
    float rad = value*D_PI;
    float dx = cosf(rad)*10;
    float dy = sinf(rad)*10;
    if(rad!=0)IMG::draw_line(image_data,x,y,(int)(x+dx),(int)(y+dy),IMG_COLOR::GREEN);
  }
};
#endif


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
  unsigned char* dFunc_words(int x,int y,float value)
  {
    if(value > 0.0f)
    {
      return IMG_COLOR::GREEN;
    }
    else{
      return IMG_COLOR::BLACK;
    }
  }

  unsigned char* dFunc_binary(int x,int y,float value)
  {
    if(value > 0.0f)
    {
      return IMG_COLOR::WHITE;
    }
    else{
      return IMG_COLOR::BLACK;
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