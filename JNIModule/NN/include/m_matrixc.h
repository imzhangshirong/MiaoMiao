#ifndef M_MATRIX_C_H
#define M_MATRIX_C_H

#include "m_complex.h"
#include "m_matrix.h"
#include <vector>


using namespace std;

class m_matrixc
{
private:
  m_complex* m_map;
public:
  int width;
  int height;

  m_matrixc(int width, int height);
  m_matrixc(int width, int height, m_complex value);
  m_matrixc(const m_matrixc &m);
  m_matrixc &operator= (const m_matrixc &m);
  m_complex get(int x, int y);
  void set(int x, int y, m_complex value);
  void print();
  void add(m_complex value);
  m_complex* data();
  m_matrix to_float();
  ~m_matrixc();
};
#endif