#ifndef MATRIX_H
#define MATRIX_H

struct MATRIX2_RECT
{
  int x;
  int y;
  int width;
  int height;
  int flag;
};

class MATRIX2{
private:
  float* m_map;
public:
  int width;
  int height;

  MATRIX2(int width, int height);
  MATRIX2(int width, int height, float value);
  MATRIX2 conv(MATRIX2 matrix);
  MATRIX2 conv_scale(MATRIX2 matrix);
  MATRIX2 conv_scale(MATRIX2 matrix,MATRIX2_RECT* areas,int n);
  float get(int x, int y);
  void set(int x, int y, float value);
  void set_array(float* data);
  void print();
  void filter(float (*filter)(int,int,float));
  void filter(float (*filter)(int,int,float,float*),float* params);
  float max();
  float min();
  float average();
};
#endif