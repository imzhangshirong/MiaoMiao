#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <cfloat>
#include "../include/VERT.h"

VERT2::VERT2()
{
  this->x=0.0f;
  this->y=0.0f;
}

VERT2::VERT2(float x,float y)
{
  this->x = x;
  this->y = y;
}

VERT2 VERT2::add(VERT2 vert)
{
  return VERT2(this->x+vert.x,this->y+vert.y);
}

VERT2 VERT2::sub(VERT2 vert)
{
  return VERT2(this->x-vert.x,this->y-vert.y);
}

VERT2 VERT2::multiply(float value)
{
  return VERT2(this->x*value,this->y*value);
}

float VERT2::length()
{
  return powf(powf(this->x,2)+powf(this->y,2),0.5f);
}

float VERT2::dot(VERT2 vert)
{
  return this->x*vert.x+this->y*vert.y;
}

void VERT2::unit()
{
  float length = this->length();
  this->x/=length;
  this->y/=length;
}

