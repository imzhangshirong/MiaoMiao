#ifndef M_VERT2_H
#define M_VERT2_H

class m_vert2
{
private:

public:
  float x;
  float y;
  m_vert2();
  m_vert2(float x,float y);
  m_vert2 add(m_vert2 vert);
  m_vert2 sub(m_vert2 vert);
  float dot(m_vert2 vert);
  m_vert2 multiply(float value);
  float length();
  void unit();
};
#endif