#ifndef MATRIX_H
#define MATRIX_H

class COMPLEX2
{
public:
  float rl;
  float im;
  COMPLEX2();
  COMPLEX2(float rl,float im);
  float modulo();
  void add(COMPLEX2 cop);
  void mul(COMPLEX2 cop);
  void print();
};

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
  void add(float value);
  float* data();
  MATRIX2 subset(int x,int y,int width,int height);
  void mul(float value);
};

class MATRIX_C2
{
private:
  COMPLEX2* m_map;
public:
  int width;
  int height;

  MATRIX_C2(int width, int height);
  MATRIX_C2(int width, int height, COMPLEX2 value);
  COMPLEX2 get(int x, int y);
  void set(int x, int y, COMPLEX2 value);
  void set_array(COMPLEX2* data);
  void print();
  void add(COMPLEX2 value);
  COMPLEX2* data();
  MATRIX2 to_float();
};
#endif