#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <vector>
#include "../include/m_matrix.h"

using namespace std;

m_matrix::m_matrix(int width, int height, float value)
{
  this->width = width;
  this->height = height;
  int size = width*height;
  this->m_map = (float*)malloc(size*sizeof(float));
  for(int i=0;i<size;i++)
  {
    *(this->m_map+i)=value;
  }
}

m_matrix::m_matrix(const m_matrix &m)
{
  *this = m;
}

m_matrix &m_matrix::operator= (const m_matrix &m)
{
  width = m.width;
  height = m.height;
  int size = width*height;
  this->m_map = (float*)malloc(size*sizeof(float));
  memcpy(this->m_map,m.m_map,size*sizeof(float));
  return *this;
}

m_matrix m_matrix::subset(int x,int y,int width,int height)
{
  m_matrix re(width,height);
  int cur=0;
  for(int i=0;i<height;i++)
  {
    for(int j=0;j<width;j++)
    {
      *(re.data()+cur)=this->get(x+j,y+i);
      cur++;
    }
  }
  return re;
}

float* m_matrix::data()
{
  return this->m_map;
}

m_matrix::m_matrix(int width, int height)
{
  new (this) m_matrix(width,height,0.0f);
}

float m_matrix::get(int x, int y)
{
  return *(this->m_map+y*this->width+x);
}

void m_matrix::set(int x, int y, float value)
{
  *(this->m_map+y*this->width+x) = value;
}

void m_matrix::set_array(float* data,int n)
{
  int size = width*height;  
  this->m_map = (float*)malloc(size*sizeof(float));
  memcpy(this->m_map,data,size*sizeof(float));
}

void m_matrix::set_vector(vector<float> &data)
{
  if (!data.empty())
  {
    int size = width*height;  
    this->m_map = (float*)malloc(size*sizeof(float));
    memcpy(this->m_map,&data[0],size*sizeof(float));
  }
}

m_matrix m_matrix::conv_scale(m_matrix &matrix,m_matrix_rect* areas,int n)
{
  if(areas==NULL)return this->conv_scale(matrix);
  int max_width = this->width/matrix.width;
  int max_height = this->height/matrix.height;
  int cur = 0;
  int row = 0;
  int col = 0;
  int x = 0;
  int y = 0;
  m_matrix re(max_width, max_height);
  for(int a=0;a<n;a++)
  {
    m_matrix_rect rect = *(areas+a);
    if(rect.flag==0)continue;
    cur=rect.y*this->width+rect.x;
    int max_a_h = rect.height/matrix.height;
    int max_a_w = rect.width/matrix.width;
    col=rect.x/matrix.width;
    row=rect.y/matrix.height;
    for(int i=0;i<max_a_h;i++)
    {
      x = 0;
      for(int j=0;j<max_a_w;j++)
      {
        float temp = 0.0f;
        y = 0;
        for(int k=0;k<matrix.height;k++)
        {
          for(int l=0;l<matrix.width;l++)
          {
            float curValue = *(this->m_map+cur+y+x+l);
            temp+=matrix.get(l,k)*curValue;
          }
          y+=this->width;
        }
        x+=matrix.width;
        *(re.data()+row*max_width+col)=temp;
        col++;
      }
      row++;
      cur+=this->width*matrix.height;
    }
  }
  
  return re;
}

m_matrix m_matrix::conv_scale(m_matrix &matrix)
{
  int max_width = this->width/matrix.width;
  int max_height = this->height/matrix.height;
  int cur = 0;
  int row = 0;
  int col = 0;
  int x = 0;
  int y = 0;
  m_matrix re(max_width, max_height);
  for(int i=0;i<re.height;i++)
  {
    x = 0;
    for(int j=0;j<re.width;j++)
    {
      float temp = 0.0f;
      y = 0;
      for(int k=0;k<matrix.height;k++)
      {
        for(int l=0;l<matrix.width;l++)
        {
          float curValue = *(this->m_map+cur+y+x+l);
          temp+=matrix.get(l,k)*curValue;
        }
        y+=this->width;
      }
      x+=matrix.width;
      *(re.data()+col)=temp;
      col++;
    }
    cur+=this->width*matrix.height;
  }
  
  return re;
}

m_matrix m_matrix::conv(m_matrix &matrix)
{
  int half = matrix.width/2;
  m_matrix re(this->width,this->height);
  int base_x = 0;
  int base_y = 0;
  int x = 0;
  int y = 0;
  for(int i=0;i<this->height;i++)
  {
    for(int j=0;j<this->width;j++)
    {
      float temp = 0.0f;
      for(int k=0;k<matrix.height;k++)
      {
        for(int l=0;l<matrix.width;l++)
        {
          x = base_x + l - half;
          y = base_y + k - half;
          if(x>=0 && y>=0 && x<this->width && y<this->height)
          {
            temp+=this->get(x,y)*matrix.get(l,k);
          }
        }
      }
      re.set(j,i,temp);
      base_x++;
    }
    base_x=0;
    base_y++;
  }
  return re;
}

void m_matrix::print()
{
  for(int i=0;i<this->height;i++)
  {
    for(int j=0;j<this->width;j++)
    {
      if(j>0)std::cout<<",";
      cout<<this->get(j,i);
    }
    cout<<endl;
  }
}

void m_matrix::add(float value)
{
  int n = this->width*this->height;
  for(int i=0;i<n;i++)
  {
    *(this->m_map+i) = *(this->m_map+i)+value;
  }
}

void m_matrix::mul(float value)
{
  int n = this->width*this->height;
  for(int i=0;i<n;i++)
  {
    *(this->m_map+i) = *(this->m_map+i)*value;
  }
}

void m_matrix::filter(float (*filter)(int,int,float))
{
  for(int i=0;i<this->height;i++)
  {
    for(int j=0;j<this->width;j++)
    {
      this->set(j,i,filter(j,i,this->get(j,i)));
    }
  }
}

void m_matrix::filter(float (*filter)(int,int,float,float*),float* params)
{
  for(int i=0;i<this->height;i++)
  {
    for(int j=0;j<this->width;j++)
    {
      this->set(j,i,filter(j,i,this->get(j,i),params));
    }
  }
}


float m_matrix::min()
{
  float min = FLT_MAX;
  int n = this->width*this->height;
  for(int i=0;i<n;i++)
  {
    float v = *(this->m_map+i);
    if(v<min)min = v;
  }
  return min;
}

float m_matrix::maxf()
{
  float max = -FLT_MAX;
  int n = this->width*this->height;
  for(int i=0;i<n;i++)
  {
    float v = *(this->m_map+i);
    if(v>max)
    {
      max = v;
    }
  }
  return max;
}

m_matrix_index_info m_matrix::max()
{
  m_matrix_index_info index;
  index.value = -FLT_MAX;
  int n = this->width*this->height;
  for(int i=0;i<this->height;i++)
  {
    for(int j=0;j<this->width;j++)
    {
      float v = this->get(j,i);
      if(v>index.value)
      {
        index.x=j;
        index.y=i;
        index.value = v;
      }
    }
  }
  return index;
}

float m_matrix::average()
{
  float all = 0.0f;
  int n = this->width*this->height;
  for(int i=0;i<n;i++)
  {
    float v = *(this->m_map+i);
    all+=v;
  }
  return all/n;
}

m_matrix::~m_matrix()
{
  if(this->height>0 && this->width>0 && this->m_map != NULL)
  {
    free(this->m_map);
    this->m_map = NULL;
    this->height = 0;
    this->width = 0;
  }
}