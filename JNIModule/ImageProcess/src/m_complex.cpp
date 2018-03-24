#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <vector>
#include "../include/m_complex.h"

using namespace std;

m_complex::m_complex()
{
  this->rl=0.0f;
  this->im=0.0f;
}
m_complex::m_complex(float rl,float im)
{
  this->rl=rl;
  this->im=im;
}
float m_complex::modulo()
{
  return powf(powf(this->rl,2.0f)+powf(this->im,2.0f),0.5f);
}

void m_complex::add(m_complex cop)
{
  this->rl+=cop.rl;
  this->im+=cop.im;
}

void m_complex::mul(m_complex cop)
{
  float rl=this->rl;
  this->rl=this->rl*cop.rl-this->im*cop.im;
  this->im=rl*cop.im+this->im*cop.rl;
}

void m_complex::print()
{
  cout<<"("<<this->rl<<","<<this->im<<"i)"<<endl;
}