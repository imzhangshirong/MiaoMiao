#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../include/m_vert2.h"

m_vert2::m_vert2()
{
  this->x=0.0f;
  this->y=0.0f;
}

m_vert2::m_vert2(float x,float y)
{
  this->x = x;
  this->y = y;
}

m_vert2 m_vert2::add(m_vert2 vert)
{
  return m_vert2(this->x+vert.x,this->y+vert.y);
}

m_vert2 m_vert2::sub(m_vert2 vert)
{
  return m_vert2(this->x-vert.x,this->y-vert.y);
}

m_vert2 m_vert2::multiply(float value)
{
  return m_vert2(this->x*value,this->y*value);
}

float m_vert2::length()
{
  return powf(powf(this->x,2)+powf(this->y,2),0.5f);
}

float m_vert2::dot(m_vert2 vert)
{
  return this->x*vert.x+this->y*vert.y;
}

void m_vert2::unit()
{
  float length = this->length();
  this->x/=length;
  this->y/=length;
}

