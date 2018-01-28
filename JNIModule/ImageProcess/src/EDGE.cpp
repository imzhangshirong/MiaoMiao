#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../include/EDGE.h"

void set_label(unsigned int * label_data,int width,int height,int x,int y,unsigned int label);

void set_label(unsigned int * label_data,int width,int height,int x,int y,unsigned int label){
  int length=width*y+x;
  unsigned int * rw = label_data+length;
  *(rw)=label;
}

void EDGE::connected_mark(IMG_DATA image_data,IMG_DATA image_data_out){
  unsigned int * label_data = (unsigned int *)malloc(sizeof(int)*image_data.width*image_data.height);
  int d1,d2;
  int label_s=0;
  int label_c=0;
  set_label(label_data,image_data.width,image_data.height,0,0,0);
  for(int i=0;i<image_data.height-1;i++){
    for(int j=0;j<image_data.width-1;j++){
      label_c=*(label_data+i*image_data.width+j);
      unsigned char * rgb11 = IMG::get_rgb(image_data,j,i);
      unsigned char * rgb12 = IMG::get_rgb(image_data,j+1,i);
      unsigned char * rgb21 = IMG::get_rgb(image_data,j,i+1);
      unsigned char * rgb22 = IMG::get_rgb(image_data,j+1,i+1);
      d1 = abs(*rgb11-*rgb12);
      d2 = abs(*rgb11-*rgb21);
      if(d1>8){
        set_label(label_data,image_data.width,image_data.height,j+1,i,(label_c==0)?1:0);  
      }
      else{
        set_label(label_data,image_data.width,image_data.height,j+1,i,label_c);
      }
      if(d2>8){
        set_label(label_data,image_data.width,image_data.height,j,i+1,(label_c==0)?1:0);  
      }
      else{
        set_label(label_data,image_data.width,image_data.height,j,i+1,label_c);
      }
    }
  }
  for(int i=0;i<image_data.height-1;i++){
    for(int j=0;j<image_data.width-1;j++){
      label_c=*(label_data+i*image_data.width+j);
      if(label_c==0){
        IMG::set_rgb(image_data_out,j,i,IMG_COLOR::BLACK);
      }
      else{
        IMG::set_rgb(image_data_out,j,i,IMG_COLOR::WHITE); 
      }
    }
  }
}

void EDGE::prewitt_mark(IMG_DATA image_data,IMG_DATA image_data_out, MATRIX2 map_data){
  int row_length = image_data.width;
  for(int i=1;i<image_data.height-1;i+=1){
    for(int j=1;j<image_data.width-1;j+=1){
      unsigned char * rgb11 = IMG::get_rgb(image_data,j-1,i-1);
      unsigned char * rgb12 = IMG::get_rgb(image_data,j,i-1);
      unsigned char * rgb13 = IMG::get_rgb(image_data,j+1,i-1);
      unsigned char * rgb21 = IMG::get_rgb(image_data,j-1,i);
      unsigned char * rgb22 = IMG::get_rgb(image_data,j,i);
      unsigned char * rgb23 = IMG::get_rgb(image_data,j+1,i);
      unsigned char * rgb31 = IMG::get_rgb(image_data,j-1,i+1);
      unsigned char * rgb32 = IMG::get_rgb(image_data,j,i+1);
      unsigned char * rgb33 = IMG::get_rgb(image_data,j+1,i+1);
      int fy = (*rgb11 - *rgb31)+2*(*rgb12 - *rgb32)+(*rgb13 - *rgb33);
      int fx = (*rgb13 - *rgb11)+2*(*rgb23 - *rgb21)+(*rgb33 - *rgb31);
      double d=pow(fy*fy+fx*fx,0.5);
      if(d>0){
        double ky = (double)fy/d;
        double kx = (double)fx/d;
        int dx=(int)(kx*2);
        int dy=(int)(ky*2);
        if(dx>1){
          dx=1;
        }
        else if(dx<-1){
          dx=-1;
        }
        if(dy>1){
          dy=1;
        }
        else if(dy<-1){
          dy=-1;
        }
        unsigned char * rgbS = IMG::get_rgb(image_data,j+dx,i+dy);
        unsigned char * rgbE = IMG::get_rgb(image_data,j-dx,i-dy);
        int md = abs(*rgbE-*rgbS);
        //if(md<0)md=1;
        //if(md>100)md=100;
        //IMG::draw_line(image_data_out,j,i,j+kx*md,i+ky*md,red);
        if(md>20){
          IMG::set_rgb(image_data_out,j,i,IMG_COLOR::RED);
          map_data.set(j,i,1.0f);
        }     
      }
    }
    row_length += image_data.width;
  }
}