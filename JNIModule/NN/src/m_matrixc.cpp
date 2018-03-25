#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <vector>
#include "../include/m_matrixc.h"
#include "../include/m_complex.h"

using namespace std;

m_matrixc::m_matrixc(int width, int height)
{
  new (this) m_matrixc(width,height,m_complex{0.0f,0.0f});
}
m_matrixc::m_matrixc(int width, int height, m_complex value)
{
  this->width = width;
  this->height = height;
  int size = width*height;
  this->m_map = (m_complex*)malloc(size*sizeof(m_complex));
  for(int i=0;i<size;i++)
  {
    *(this->m_map+i)=value;
  }
}

m_matrixc::m_matrixc(const m_matrixc &m)
{
  *this = m;
}

m_matrixc &m_matrixc::operator= (const m_matrixc &m)
{
  width = m.width;
  height = m.height;
  int size = width*height;
  this->m_map = (m_complex*)malloc(size*sizeof(m_complex));
  memcpy(this->m_map,m.m_map,size*sizeof(m_complex));
  return *this;
}

m_complex m_matrixc::get(int x, int y)
{
  return *(this->m_map+y*this->width+x);
}

void m_matrixc::set(int x, int y, m_complex value)
{
  *(this->m_map+y*this->width+x) = value;
}


void m_matrixc::print()
{
  for(int i=0;i<this->height;i++)
  {
    for(int j=0;j<this->width;j++)
    {
      if(j>0)cout<<",";
      m_complex cop = this->get(j,i);
      cout<<"("<<cop.rl<<","<<cop.im<<")";
    }
    cout<<endl;
  }
}

void m_matrixc::add(m_complex value)
{
  int n = this->width*this->height;
  for(int i=0;i<n;i++)
  {
    (*(this->m_map+i)).add(value);
  }
}

m_complex* m_matrixc::data()
{
  return this->m_map;
}

m_matrix m_matrixc::to_float()
{
  m_matrix* re = new m_matrix(this->width,this->height);
  int n = re->width*re->height;
  float* m_cur=re->data();
  m_complex* cop_cur=this->data();
  for(int i=0;i<n;i++)
  {
    *(m_cur+i)=(*(cop_cur+i)).modulo();
  }
  return *re;
}

m_matrixc::~m_matrixc()
{
  if(this->height>0 && this->width>0 && this->m_map != NULL)
  {
    free(this->m_map);
    this->m_map = NULL;
    this->height = 0;
    this->width = 0;
  }
}