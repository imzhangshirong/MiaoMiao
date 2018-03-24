#ifndef M_COMPLEX_H
#define M_COMPLEX_H

class m_complex
{
public:
  float rl;
  float im;
  m_complex();
  m_complex(float rl,float im);
  float modulo();
  void add(m_complex cop);
  void mul(m_complex cop);
  void print();
};

#endif