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
  VERT_PROC::init_vert();//初始化向量
  IMG_DATA jpeg_data = input_image("2_.jpg");

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
  f_m_m1.filter(MATRIX_PROC::filter_words);
  PROCESSOR::draw_matrix(f_m_m1,feature_m1,MATRIX_PROC::dFunc_words);
  //draw_histogram(f_m_m1,feature_m1);
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
    float max_k = f_m_area64.maxf();
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
  //
  float min_x = f_m_area64.width;
  float max_x = 0;
  float min_y = f_m_area64.height;
  float max_y = 0;
  for(int i=0;i<f_m_area64.height;i++)
  {
    for(int j=0;j<f_m_area64.width;j++)
    {
      if(f_m_area64.get(j,i)>0){
        if(j<min_x)min_x=j;
        if(j>max_x)max_x=j;
        if(i<min_y)min_y=i;
        if(i>max_y)max_y=i;
      }
    }
  }
  max_y+=2;
  float scale_f = f_m_m1.width/f_m_area64.width;
  min_x*=scale_f;
  max_x*=scale_f;
  scale_f = f_m_m1.height/f_m_area64.height;
  min_y*=scale_f;
  max_y*=scale_f;

  int a_width = (int)(max_x-min_x);
  int a_height = (int)(max_y-min_y);


  IMG_DATA edge_proc = IMG::create(a_width,a_height);
  MATRIX2 a_map_data = map_data.subset(min_x,min_y,a_width,a_height);
  PROCESSOR::draw_matrix(a_map_data,edge_proc,MATRIX_PROC::dFunc_binary);
  output_image("e1.jpg",edge_proc);
  int dtf_img_size = 128;
  int dtf_size = 128;
  int dtf_offset_x = 0;
  int dtf_offset_y = 0;
  IMG_DATA dtf_img = IMG::create(dtf_img_size,dtf_img_size);
  MATRIX2 dtf_map_data = map_data.subset(min_x+dtf_offset_x,min_y+dtf_offset_y,dtf_img_size,dtf_img_size);
  PROCESSOR::draw_matrix(dtf_map_data,dtf_img,MATRIX_PROC::dFunc_binary);
  output_image("dfto.jpg",dtf_img);

  dtf_img = IMG::create(dtf_size,dtf_size);
  dtf_map_data = map_data.subset(min_x+dtf_offset_x,min_y+dtf_offset_y,dtf_img_size,dtf_img_size);

  
  MATRIX2 dtf_data = PROCESSOR::FFT2(dtf_map_data,dtf_size);
  dtf_data.filter(MATRIX_PROC::mapper_DFT);
  //dtf_data.print();
  float dtf_max = dtf_data.maxf();
  dtf_data.mul(1.0f/dtf_max*255.0f);
  PROCESSOR::draw_matrix(dtf_data,dtf_img,MATRIX_PROC::dFunc_gray);
  output_image("dft.jpg",dtf_img);

  dtf_data.filter(MATRIX_PROC::filter_DFT);  
  PROCESSOR::draw_matrix(dtf_data,dtf_img,MATRIX_PROC::dFunc_binary);
  output_image("dft2.jpg",dtf_img);

  int hough_stepsThelta = 256;
  int hough_stepsLength = 256;
  IMG_DATA hough_img = IMG::create(hough_stepsThelta,hough_stepsLength);
  MATRIX2 hough_map = PROCESSOR::houghLineTrans(dtf_data,hough_stepsThelta,hough_stepsLength);
  MATRIX2_INDEXF hough_max = hough_map.max();

  std::cout<<hough_max.value<<":"<<hough_max.x<<","<<hough_max.y<<std::endl;
  hough_map.mul(1.0f/hough_max.value*255.0f);
  PROCESSOR::draw_matrix(hough_map,hough_img,MATRIX_PROC::dFunc_gray);
  output_image("hough.jpg",hough_img);
  //MATRIX2 a_map_verts = PROCESSOR::vert_mark(a_map_data,7,VERT_PROC::direct_verts,VERT_PROC::direct_verts_r,VERT_PROC::n_theta);

  //PROCESSOR::draw_verts(a_map_verts,edge_proc,VERT_PROC::func_vert);
  //IMG::draw_rect(edge_proc,IMG_RECT{(int)min_x,(int)min_y,(int)max_x,(int)max_y},IMG_COLOR::WHITE);
  


  std::cout<<f_m_area64.width<<","<<f_m_area64.height<<std::endl;
  output_image("fa1.jpg",feature_area);



  std::cout<<get_timestamp()-_t<<" ms"<<std::endl;
  return 0;
}