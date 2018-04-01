#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <vector>
#include <sys/time.h>
#include "../include/m_matrix.h"


using namespace std;
m_matrix::m_matrix()
{
  this->width = 0;
  this->height = 0;
  this->m_map = NULL;
}

m_matrix::m_matrix(int width, int height, float value)
{
  this->width = width;
  this->height = height;
  int size = width*height;
  this->m_map = new float[size];
  for(int i=0;i<size;i++)
  {
    this->m_map[i]=value;
  }
}

m_matrix::m_matrix(int width, int height, initializer_list<float> array)
{
  this->m_map = new float[1];
  this->width = width;
  this->height = height;
  int size = width*height;
  this->set_array(array.begin(),size);
  
}

m_matrix::m_matrix(const m_matrix &m)
{
  *this = m;
}

void m_matrix::random()
{
  struct timeval start;//s
  struct timeval end;//us
  gettimeofday(&start,NULL);
  srand(start.tv_usec+start.tv_sec);

  for(int i=0;i<this->height;i++)
  {
    for(int j=0;j<this->width;j++)
    {
      
      float num = (rand()%1000)*0.001;
      this->set(j,i,num);
    }
  }
}

m_matrix &m_matrix::operator= (const m_matrix &m)
{
  width = m.width;
  height = m.height;
  int size = width*height;
  this->m_map = new float[size];
  memcpy(this->m_map,m.m_map,size*sizeof(float));
  return *this;
}

m_matrix m_matrix::subset(int x,int y,int width,int height)
{
  m_matrix re(width,height);
  int cur=0;
  float* pR = re.data();
  for(int i=0;i<height;i++)
  {
    for(int j=0;j<width;j++)
    {
      pR[cur]=this->get(x+j,y+i);
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
  return this->m_map[y*this->width+x];
}

void m_matrix::set(int x, int y, float value)
{
  this->m_map[y*this->width+x] = value;
}

void m_matrix::set_array(const float* data,int n)
{
  delete [] this->m_map;
  this->m_map =new float[n];
  memcpy(this->m_map,data,n*sizeof(float));
}

void m_matrix::set_vector(vector<float> &data)
{
  if (!data.empty())
  {
    int size = data.size();
    delete [] this->m_map;
    this->m_map =new float[size];
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
  float * pRe = re.data();  
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
            float curValue = this->m_map[cur+y+x+l];
            temp+=matrix.get(l,k)*curValue;
          }
          y+=this->width;
        }
        x+=matrix.width;
        pRe[row*max_width+col]=temp;
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
  float * pRe = re.data();  
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
          float curValue = this->m_map[cur+y+x+l];
          temp+=matrix.get(l,k)*curValue;
        }
        y+=this->width;
      }
      x+=matrix.width;
      pRe[col]=temp;
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
      if(j>0)std::cout<<",\t";
      cout<<this->get(j,i);
    }
    cout<<endl;
  }
}

void m_matrix::print_row_col()
{
  for(int i=0;i<this->width;i++)
  {
    for(int j=0;j<this->height;j++)
    {
      if(j>0)std::cout<<",\t";
      cout<<this->get(i,j);
    }
    cout<<endl;
  }
}


void m_matrix::add(float value)
{
  int n = this->width*this->height;
  for(int i=0;i<n;i++)
  {
    this->m_map[i] += value;
  }
}

void m_matrix::mul(float value)
{
  int n = this->width*this->height;
  for(int i=0;i<n;i++)
  {
    this->m_map[i] *= value;
  }
}

m_matrix m_matrix::mul_v(m_matrix &matrix)
{
  if(this->width==matrix.width && this->height==matrix.height)
  {
    m_matrix re(this->width,this->height);
    for(int i=0;i<this->height;i++)
    {
      for(int j=0;j<this->width;j++)
      {
        float v1 = this->get(j,i);
        float v2 = matrix.get(j,i);
        re.set(j,i,v1*v2);
      }
    }
    return re;
  }
  m_matrix null(0,0);
  cout<<"Error:"<<this->height<<"x"<<this->width<<" mulValue "<<matrix.height<<"x"<<matrix.width<<endl;
  return null;
}

void m_matrix::filter(float (*filter)(float))
{
  for(int i=0;i<this->height;i++)
  {
    for(int j=0;j<this->width;j++)
    {
      this->set(j,i,filter(this->get(j,i)));
    }
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
    float v = this->m_map[i];
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
    float v = this->m_map[i];
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
    float v = this->m_map[i];
    all+=v;
  }
  return all/n;
}

m_matrix::~m_matrix()
{
  if(this->height>0 && this->width>0 && this->m_map != NULL)
  {
    delete [] this->m_map;
    this->m_map = NULL;
    this->height = 0;
    this->width = 0;
  }
}

void m_matrix::release()
{
  this->~m_matrix();
}


m_matrix m_matrix::mul(m_matrix &matrix)
{
  if(this->width == matrix.height)
  {
    m_matrix re(matrix.width,this->height);
    for(int i=0;i<this->height;i++)
    {
      for(int j=0;j<matrix.width;j++)
      {
        float v = 0;
        for(int k=0;k<matrix.height;k++)
        {
          float v1 = this->get(k,i);
          float v2 = matrix.get(j,k);
          v+=v1*v2;
        }
        re.set(j,i,v);
      }
    }
    return re;
  }
  else{
    cout<<"Error:"<<this->height<<"x"<<this->width<<" mul "<<matrix.height<<"x"<<matrix.width<<endl;
    m_matrix null(0,0);
    return null;
  }
}

m_matrix m_matrix::transpose()
{
  m_matrix re(this->height,this->width);
  for(int i=0;i<this->width;i++)
  {
    for(int j=0;j<this->height;j++)
    {
      re.set(j,i,this->get(i,j));
    }
  }
  return re;
}

m_matrix m_matrix::add(m_matrix &matrix)
{
  if(this->width==matrix.width && this->height==matrix.height)
  {
    m_matrix re(this->width,this->height);
    for(int i=0;i<this->height;i++)
    {
      for(int j=0;j<this->width;j++)
      {
        float v1 = this->get(j,i);
        float v2 = matrix.get(j,i);
        re.set(j,i,v1+v2);
      }
    }
    return re;
  }
  m_matrix null(0,0);
  cout<<"Error:"<<this->height<<"x"<<this->width<<" add "<<matrix.height<<"x"<<matrix.width<<endl;
  return null;
}