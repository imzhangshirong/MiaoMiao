#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <cfloat>
#include "../include/IMG.h"
#include "../include/PROCESSOR.h"
#include "../include/VERT.h"

void PROCESSOR::binary(IMG_DATA image_data,IMG_DATA image_data_out,int threshold,int range){
  for(int i=0;i<image_data.height;i++){
    for(int j=0;j<image_data.width;j++){
      unsigned char * rgb = IMG::get_rgb(image_data,j,i);
      if(abs(*rgb - threshold)>range){
        IMG::set_rgb(image_data_out,j,i,IMG_COLOR::BLACK);
      }
      else{
        IMG::set_rgb(image_data_out,j,i,IMG_COLOR::WHITE);        
      }
    }
  }
}

void PROCESSOR::binary2(IMG_DATA image_data,IMG_DATA image_data_out,int threshold,int range,int whiteDirect){
  for(int i=0;i<image_data.height;i++){
    for(int j=0;j<image_data.width;j++){
      unsigned char * rgb = IMG::get_rgb(image_data,j,i);
      int d = *rgb - threshold;
      if(abs(d)>range && d*whiteDirect<0){
        IMG::set_rgb(image_data_out,j,i,IMG_COLOR::BLACK);
      }
      else{
        IMG::set_rgb(image_data_out,j,i,IMG_COLOR::WHITE);        
      }
    }
    
  }
  
}

void PROCESSOR::gray_scale(IMG_DATA image_data,IMG_DATA image_data_out){
  for(int i=0;i<image_data.height;i++){
    for(int j=0;j<image_data.width;j++){
      unsigned char * rgb = IMG::get_rgb(image_data,j,i);
      unsigned char gray = (*rgb+*(rgb+1)+*(rgb+2))/3;
      IMG::set_rgb(image_data_out,j,i,gray,gray,gray);
    }
  }
}

MATRIX2 PROCESSOR::gray2matrix(IMG_DATA image_data,int* grayMap,int* averageGray)
{
  MATRIX2 re(image_data.width,image_data.height);
  int grayAverage = 0;
  for(int i=0;i<image_data.height;i++){
    for(int j=0;j<image_data.width;j++){
      unsigned char gray = *(IMG::get_rgb(image_data,j,i));
      re.set(j,i,gray);
      grayAverage+=gray;
    }
  }
  grayAverage/=(image_data.height*image_data.width);
  *averageGray = grayAverage;
  return re;
}

void block_binary(IMG_DATA image_data,int block_width,int block_height,bool (*bFunc)(int)){
  int components = image_data.components;
  int x = 0;
  int y = 0;
  while(x>=0 && y>=0){
    int copyWidth = block_width;
    int copyHeight = block_height;
    if(x*block_width >= image_data.width){
      x=0;
      y++;
    }
    if(y*block_height >= image_data.height)break;
    int start_p_x = x*block_width;
    int start_p_y = y*block_height;
    if(start_p_x+copyWidth>image_data.width)copyWidth = image_data.width-start_p_x;
    if(start_p_y+copyHeight>image_data.height)copyHeight = image_data.height-start_p_y;
    for(int i=0;i<copyHeight;i++){
      for(int j=0;j<copyWidth;j++){
        int p_x = start_p_x + j;
        int p_y = start_p_y + i;
        unsigned char * rgb = IMG::get_rgb(image_data,p_x,p_y);
        if((*bFunc)(*rgb)){
          IMG::set_rgb(image_data,p_x,p_y,IMG_COLOR::BLACK);
        }
        else{
          IMG::set_rgb(image_data,p_x,p_y,IMG_COLOR::WHITE);        
        }
      }
    }
    x++;
  }
}

void PROCESSOR::draw_matrix(MATRIX2 matrix,IMG_DATA image_data_out,unsigned char* (*func)(int x,int y,float value))
{
  for(int i=0;i<matrix.height;i++)
  {
    for(int j=0;j<matrix.width;j++)
    {
      unsigned char* color = func(j,i,matrix.get(j,i));
      IMG::set_rgb(image_data_out,j,i,color);
    }
  }
}

void PROCESSOR::draw_verts(MATRIX2 matrix,IMG_DATA image_data_out,void (*func)(IMG_DATA image_data,int x,int y,float value))
{
  for(int i=0;i<matrix.height;i++)
  {
    for(int j=0;j<matrix.width;j++)
    {
      func(image_data_out,j,i,matrix.get(j,i));
    }
  }
}

MATRIX2 PROCESSOR::vert_mark(MATRIX2 matrix,int block_size,VERT2* verts_d,float* verts_d_r,int verts_d_length)
{
  MATRIX2 re(matrix.width,matrix.height);
  int size = block_size;
  int valid = size*size/2;
  int half=size/2;
  int x=0,y=0,dx=0,dy=0;
  int v_size = size*size;
  for(int i=0;i<matrix.height;i++)
  {
    for(int j=0;j<matrix.width;j++)
    {
      VERT2 verts[v_size];
      int v_n=0;
      for(int k=0;k<size;k++)
      {
        for(int l=0;l<size;l++)
        {
          dx = k-half;
          dy = l-half;
          x=j+dx;
          y=i+dy;
          if(x>=0 && x<matrix.width && y>=0 && y<matrix.height)
          {
            float value = matrix.get(x,y);
            if(value>0)
            {
              verts[v_n]=VERT2(dx,dy);
              v_n++;
            }
          }
        }
      }
      if(v_n<valid)continue;
      VERT2 vert_c(0.0f,0.0f);
      for(int k=0;k<v_n;k++)
      {
        vert_c = vert_c.add(verts[k]);
      }
      vert_c = vert_c.multiply(1.0f/v_n);
      for(int k=0;k<v_n;k++)
      {
        verts[k] = verts[k].sub(vert_c);
      }
      float min_d = FLT_MAX;
      int min_index = -1;
      for(int k=0;k<verts_d_length;k++)
      {
        VERT2 base_vert = verts_d[k];
        float cur_d = 0.0f;
        for(int l=0;l<v_n;l++)
        {
          cur_d+=verts[l].dot(base_vert);
        }
        if(cur_d<min_d){
          min_d = cur_d;
          min_index = k;
        }
      }
      re.set(j,i,verts_d_r[min_index]);
    }
  }
  return re;
}


//FFT2///////////////////////////////////////////////////////////
COMPLEX2 _W_u_L(float u,float L)
{
  static float pi2=-M_PI*2;
  float W_uL_d = pi2*(float)u/(float)L;
  COMPLEX2 W_uL(cosf(W_uL_d),sinf(W_uL_d));
  return W_uL;
}

COMPLEX2 _fft2(float* sum_d,int u,int L)
{
  int n = L;
  int n_r = n/2;
  COMPLEX2 W_uL = _W_u_L((float)u,(float)L);
  COMPLEX2 F1;
  COMPLEX2 F2;
  if(n_r>1){
    COMPLEX2 fb(0.0f,0.0f);
    float rd1[n_r];
    float rd2[n_r];
    for(int r=0;r<n_r;r++)
    {
      rd1[r]=sum_d[2*r];
      rd2[r]=sum_d[2*r+1];
    }
    F1 = _fft2(rd1,u,n_r);
    F2 = _fft2(rd2,u,n_r);
  }
  else
  {
    //r=0;
    F1 = COMPLEX2(sum_d[0],0.0f);
    F2 = COMPLEX2(sum_d[1],0.0f);
  }
  F2.mul(W_uL);
  F1.add(F2);
  return F1;
}

COMPLEX2 _fft2(COMPLEX2* sum_d,int u,int L)
{
  int n = L;
  int n_r = n/2;
  COMPLEX2 W_uL = _W_u_L((float)u,(float)L);
  COMPLEX2 F1;
  COMPLEX2 F2;
  if(n_r>1){
    COMPLEX2 fb(0.0f,0.0f);
    COMPLEX2 rd1[n_r];
    COMPLEX2 rd2[n_r];
    for(int r=0;r<n_r;r++)
    {
      rd1[r]=sum_d[2*r];
      rd2[r]=sum_d[2*r+1];
    }
    F1 = _fft2(rd1,u,n_r);
    F2 = _fft2(rd2,u,n_r);
  }
  else
  {
    //r=0;
    F1 = COMPLEX2(1.0f,0.0f);
    F2 = COMPLEX2(1.0f,0.0f);
    F1.mul(sum_d[0]);
    F2.mul(sum_d[1]);
  }
  F2.mul(W_uL);
  F1.add(F2);
  return F1;
}

void _fftA(MATRIX_C2 rec,MATRIX2 matrix)
{
  for(int m=0;m<rec.height;m++)
  {
    float line[matrix.width];
    for(int i=0;i<matrix.width;i++)
    {
      line[i]=matrix.get(i,m);
    }
    for(int k=0;k<rec.width;k++)
    {
      COMPLEX2 fb=_fft2(line,k,matrix.width);
      rec.set(k,m,fb);
    }
  }
}

void _fftB(MATRIX_C2 rec,MATRIX_C2 matrix)
{
  for(int m=0;m<rec.width;m++)
  {
    COMPLEX2 line[matrix.height];
    for(int i=0;i<matrix.height;i++)
    {
      line[i]=matrix.get(m,i);
    }
    for(int k=0;k<rec.height;k++)
    {
      COMPLEX2 fb=_fft2(line,k,matrix.height);
      rec.set(m,k,fb);
    }
  }
}

MATRIX2 PROCESSOR::FFT2(MATRIX2 matrix,int size)
{
  MATRIX_C2 recA(size,size);
  _fftA(recA,matrix);
  MATRIX_C2 recB(size,size);
  _fftB(recB,recA);
  MATRIX_C2 rec(size,size);
  int half_x = size/2;
  int half_y = size/2;
  
  for(int l=0;l<size;l++)
  {
    for(int k=0;k<size;k++)
    {
      COMPLEX2 fb=recB.get(k,l);
      
      if (l<half_y && k<half_x)
      {
        rec.set(k+half_x,l+half_y,fb);
      }
      else if(l<half_y && k>=half_x)
      {
        rec.set(k-half_x,l+half_y,fb);
      }
      else if(l>=half_y && k>=half_x)
      {
        rec.set(k-half_x,l-half_y,fb);
      }
      else if(l>=half_y && k<half_x)
      {
        rec.set(k+half_x,l-half_y,fb);
      }
    }
  }
  return rec.to_float();
}
//DFT///////////////////////////////////////////////////////////
MATRIX2 PROCESSOR::DFT(MATRIX2 matrix,int size)
{
  MATRIX_C2 rec(size,size);
  int half_x = size/2;
  int half_y = size/2;
  float pi2=-M_PI*2;
  
  for(int l=0;l<rec.height;l++)
  {
    for(int k=0;k<rec.width;k++)
    {
      COMPLEX2 fb(0.0f,0.0f);
      for(int j=0;j<matrix.height;j++)
      {
        for(int i=0;i<matrix.width;i++)
        {
          float v=matrix.get(i,j);
          if(v!=0.0f)
          {
            float dd=pi2*((float)(k*i)/(float)matrix.width+(float)(l*j)/(float)matrix.height);
            COMPLEX2 fbt(cosf(dd)*v,sinf(dd)*v);
            fb.add(fbt);
          }  
        }
      }
      if (l<half_y && k<half_x)
      {
        rec.set(k+half_x,l+half_y,fb);
      }
      else if(l<half_y && k>=half_x)
      {
        rec.set(k-half_x,l+half_y,fb);
      }
      else if(l>=half_y && k>=half_x)
      {
        rec.set(k-half_x,l-half_y,fb);
      }
      else if(l>=half_y && k<half_x)
      {
        rec.set(k+half_x,l-half_y,fb);
      }
    }
  }
  return rec.to_float();
}

void PROCESSOR::draw_DFT(MATRIX2 matrix,IMG_DATA image_data_out)
{
  for(int i=0;i<matrix.height;i++)
  {
    for(int j=0;j<matrix.width;j++)
    {
      unsigned char gray = (unsigned char)matrix.get(j,i);
      //std::cout<<gray<<std::endl;
      unsigned char color[] = {gray,gray,gray};
      IMG::set_rgb(image_data_out,j,i,color);
    }
  }
}
/*void PROCESSOR::Blur(IMG_DATA image_data,IMG_DATA image_data_out,int r){
  
}*/