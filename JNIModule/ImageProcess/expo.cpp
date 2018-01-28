#include <iostream>
#include <stdio.h>
#include <string>
#include <sys/time.h>
#include "expo.h"

using namespace std;
using namespace COMM_FUNC;

void draw_histogram(MATRIX2 map_data,IMG_DATA image_data)
{
  int map_row[map_data.height] = {0};
  int map_col[map_data.width] = {0};
  for(int i=0;i<map_data.height;i++)
  {
    for(int j=0;j<map_data.width;j++){
      if(map_data.get(j,i)>0.0f){
        map_row[i]++;
        map_col[j]++;
      }
    }
  }
  int n = sizeof(map_row)/sizeof(map_row[0]);
  for(int i=0;i<n;i++)
  {
    IMG::draw_line(image_data,0,i,map_row[i],i,IMG_COLOR::CYAN);
  }
  n = sizeof(map_col)/sizeof(map_col[0]);
  for(int i=0;i<n;i++)
  {
    IMG::draw_line(image_data,i,0,i,map_col[i],IMG_COLOR::MEGENTA);
  }
}

int main(){
  long _t = get_timestamp();
  MATRIX_PROC::init_mtrx();//初始化算子矩阵

  IMG_DATA jpeg_data = input_image("1.jpg");

  //灰度化
  IMG_DATA jpeg_data_gray = IMG::copy_data(jpeg_data);
  int grayMap[256]={0};
  int grayAverage = 0;
  PROCESSOR::gray_scale(jpeg_data_gray,jpeg_data_gray);
  //灰度矩阵，灰度分布
  MATRIX2 mtrx_gray = PROCESSOR::gray2matrix(jpeg_data_gray,grayMap,&grayAverage);

  IMG_DATA jpeg_data_gray_out = IMG::copy_data(jpeg_data_gray);
  MATRIX2 map_data(jpeg_data_gray.width,jpeg_data_gray.height);

  EDGE::prewitt_mark(jpeg_data_gray,jpeg_data_gray_out,map_data);
  
  output_image("1.jpg",jpeg_data_gray_out);

  


  IMG_DATA feature_m1 = IMG::create(map_data.width,map_data.height);
  MATRIX2 f_m_m1 = map_data.conv(MATRIX_PROC::m1);
  f_m_m1.filter(MATRIX_PROC::filter1);
  PROCESSOR::draw_matrix(f_m_m1,feature_m1,MATRIX_PROC::func1);
  draw_histogram(f_m_m1,feature_m1);
  output_image("f1.jpg",feature_m1);

  IMG_DATA feature_area = IMG::create(map_data.width,map_data.height);
  MATRIX2_RECT* areas = NULL;
  int area_size=64;
  int n = 0;
  MATRIX2 f_m_area64=f_m_m1;
  while(area_size>=8)
  {
    f_m_area64 = f_m_m1.conv_scale(MATRIX2(area_size,area_size,1.0f),areas,n);
    n = f_m_area64.width*f_m_area64.height;
    float max_k = f_m_area64.max();
    int m=0;
    areas=(MATRIX2_RECT *)malloc(n*sizeof(MATRIX2_RECT));
    IMG::clear(feature_area,IMG_COLOR::BLACK);
    for(int i=0;i<f_m_area64.height;i++)
    {
      for(int j=0;j<f_m_area64.width;j++)
      {
        int x=j*area_size;
        int y=i*area_size;
        int flag = 1;
        float value = f_m_area64.get(j,i);
        if(value<=0.1f*max_k)flag=0;
        MATRIX2_RECT rect{x,y,area_size,area_size,flag};
        *(areas+m)=rect;
        if(flag==1){
          IMG_RECT img_rect{x,y,x+area_size,y+area_size};
          unsigned char c = (unsigned char)(255.0f*value/max_k);
          unsigned char color[]={c,c,c};
          IMG::fill_rect(feature_area,img_rect,color);
        }
        m++;
      }
    }
    area_size/=2;
  }
  float min_x = f_m_area64.width;
  float max_x = 0;
  float min_y = f_m_area64.height;
  float max_y = 0;
  for(int i=0;i<f_m_area64.height;i++)
  {
    for(int j=0;j<f_m_area64.width;j++)
    {
      if(f_m_area64.get(j,i)>0){
        if(j<min_x)min_x=j-1;
        if(j>max_x)max_x=j+1;
        if(i<min_y)min_y=i-1;
        if(i>max_y)max_y=i+1;
      }
    }
  }
  min_x*=f_m_m1.width/f_m_area64.width;
  max_x*=f_m_m1.width/f_m_area64.width;
  min_y*=f_m_m1.height/f_m_area64.height;
  max_y*=f_m_m1.height/f_m_area64.height;
  IMG_DATA edge_proc = IMG::create(map_data.width,map_data.height);
  PROCESSOR::draw_matrix(map_data,edge_proc,MATRIX_PROC::func_edge);
  IMG::draw_rect(edge_proc,IMG_RECT{(int)min_x,(int)min_y,(int)max_x,(int)max_y},IMG_COLOR::WHITE);
  output_image("e1.jpg",edge_proc);


  std::cout<<f_m_area64.width<<","<<f_m_area64.height<<std::endl;
  output_image("fa1.jpg",feature_area);



  std::cout<<get_timestamp()-_t<<" ms"<<std::endl;
  return 0;
}