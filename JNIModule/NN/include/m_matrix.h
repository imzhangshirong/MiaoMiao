#ifndef M_MATRIX_H
#define M_MATRIX_H

#include <vector>

#define FLT_MAX 3.402823466e+38F 

using namespace std;


struct m_matrix_rect
{
  int x;
  int y;
  int width;
  int height;
  int flag;
};

struct m_matrix_index_info
{
  int x;
  int y;
  float value;
};

struct m_matrix_index
{
  int x;
  int y;
};

class m_matrix{
private:
  float* m_map;
public:
  int width;
  int height;

  m_matrix();
  m_matrix(int width, int height);
  m_matrix(int width, int height, float value);
  m_matrix(int width, int height,initializer_list<float> array);
  m_matrix(const m_matrix &m);
  m_matrix &operator= (const m_matrix &m);
  m_matrix conv(m_matrix &matrix);
  m_matrix conv_scale(m_matrix &matrix);
  m_matrix conv_scale(m_matrix &matrix,m_matrix_rect* areas,int n);
  float get(int x, int y);
  void set(int x, int y, float value);
  void set_array(const float* data,int n);
  void set_vector(vector<float> &data);
  void print();
  void filter(float (*filter)(float));
  void filter(float (*filter)(int,int,float));
  void filter(float (*filter)(int,int,float,float*),float* params);
  float maxf();
  m_matrix_index_info max(); 
  float min();
  float average();
  void random();
  void add(float value);
  m_matrix add(m_matrix &matrix);
  float* data();
  m_matrix subset(int x,int y,int width,int height);
  void mul(float value);
  m_matrix mul(m_matrix &matrix);
  m_matrix mul_v(m_matrix &matrix);
  void print_row_col();
  m_matrix transpose();
  void release();
  ~m_matrix();
};

#endif