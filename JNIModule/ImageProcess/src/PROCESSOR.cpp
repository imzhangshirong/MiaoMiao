#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../include/PROCESSOR.h"

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

/*void PROCESSOR::Blur(IMG_DATA image_data,IMG_DATA image_data_out,int r){
  
}*/