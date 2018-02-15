
#ifndef VERT2_H
#define VERT2_H
class VERT2
{
private:

public:
  float x;
  float y;
  VERT2();
  VERT2(float x,float y);
  VERT2 add(VERT2 vert);
  VERT2 sub(VERT2 vert);
  float dot(VERT2 vert);
  VERT2 multiply(float value);
  float length();
  void unit();
};
#endif