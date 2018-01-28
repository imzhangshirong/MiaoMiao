#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <cfloat>
#include "../include/MATRIX.h"


MATRIX2::MATRIX2(int width, int height, float value)
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

MATRIX2::MATRIX2(int width, int height)
{
  new (this) MATRIX2(width,height,0.0f);
}

float MATRIX2::get(int x, int y)
{
  return *(this->m_map+y*this->width+x);
}

void MATRIX2::set(int x, int y, float value)
{
  *(this->m_map+y*this->width+x) = value;
}

void MATRIX2::set_array(float* data)
{
  this->m_map = data;
}

MATRIX2 MATRIX2::conv_scale(MATRIX2 matrix,MATRIX2_RECT* areas,int n)
{
  if(areas == NULL)return this->conv_scale(matrix);
  int max_width = this->width/matrix.width;
  int max_height = this->height/matrix.height;
  int cur = 0;
  int row = 0;
  int col = 0;
  int x = 0;
  int y = 0;
  MATRIX2 re(max_width, max_height);
  for(int a=0;a<n;a++)
  {
    MATRIX2_RECT rect = areas[a];
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
        *(re.m_map+row*max_width+col)=temp;
        col++;
      }
      row++;
      cur+=this->width*matrix.height;
    }
  }
  return re;
}

MATRIX2 MATRIX2::conv_scale(MATRIX2 matrix)
{
  int max_width = this->width/matrix.width;
  int max_height = this->height/matrix.height;
  int cur = 0;
  int row = 0;
  int col = 0;
  int x = 0;
  int y = 0;
  MATRIX2 re(max_width, max_height);
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
      *(re.m_map+col)=temp;
      col++;
    }
    cur+=this->width*matrix.height;
  }
  return re;
}

MATRIX2 MATRIX2::conv(MATRIX2 matrix)
{
  int half = matrix.width/2;
  MATRIX2 re(this->width,this->height);
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

void MATRIX2::print()
{
  for(int i=0;i<this->height;i++)
  {
    for(int j=0;j<this->width;j++)
    {
      if(j>0)std::cout<<",";
      std::cout<<this->get(j,i);
    }
    std::cout<<std::endl;
  }
}

void MATRIX2::filter(float (*filter)(int,int,float))
{
  for(int i=0;i<this->height;i++)
  {
    for(int j=0;j<this->width;j++)
    {
      this->set(j,i,filter(j,i,this->get(j,i)));
    }
  }
}

void MATRIX2::filter(float (*filter)(int,int,float,float*),float* params)
{
  for(int i=0;i<this->height;i++)
  {
    for(int j=0;j<this->width;j++)
    {
      this->set(j,i,filter(j,i,this->get(j,i),params));
    }
  }
}


float MATRIX2::min()
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

float MATRIX2::max()
{
  float max = FLT_MIN;
  int n = this->width*this->height;
  for(int i=0;i<n;i++)
  {
    float v = *(this->m_map+i);
    if(v>max)max = v;
  }
  return max;
}

float MATRIX2::average()
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
