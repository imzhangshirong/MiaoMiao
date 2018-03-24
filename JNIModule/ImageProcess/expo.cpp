#include <iostream>
#include <stdio.h>
#include <string>
#include <sys/time.h>
#include "expo.h"

using namespace std;
using namespace COMM_FUNC;


int main(){
  long _t = get_timestamp();
  
  /*float arrr[] = {
    0,0,1,1,0,0,
    1,0,0,1,0,1,
    1,0,1,1,1,0,
    0,1,1,1,0,0
  };
  m_matrix test(6,4); 
  test.set_array(arrr,24);
  m_processor::bwlabel(test,0);*/

  MATRIX_PROC::init_mtrx();//初始化算子矩阵
  
  m_image_data jpeg_data = input_image("1.jpg");

  //灰度化
  m_image_data jpeg_data_gray = m_image::copy_data(jpeg_data);
  int grayMap[256]={0};
  int grayAverage = 0;
  m_processor::gray_scale(jpeg_data_gray,jpeg_data_gray);
  output_image("g.jpg",jpeg_data_gray);
  //灰度矩阵，灰度分布
  m_matrix mtrx_gray = m_processor::gray2matrix(jpeg_data_gray,grayMap,&grayAverage);


  m_image_data jpeg_data_gray_out = m_image::copy_data(jpeg_data_gray);
  m_matrix map_data(jpeg_data_gray.width,jpeg_data_gray.height);

  EDGE::prewitt_mark(jpeg_data_gray,jpeg_data_gray_out,map_data);
  
  output_image("1.jpg",jpeg_data_gray_out);

  vector<m_domain> domains = m_processor::bwlabel(mtrx_gray,4);
  m_image_data gray_labeled = m_image::create(map_data.width,map_data.height);
  m_processor::draw_labeled_domain(domains,gray_labeled);
  output_image("labeled.jpg",gray_labeled);

  m_image_data feature_m1 = m_image::create(map_data.width,map_data.height);
  m_matrix f_m_m1 = map_data.conv(MATRIX_PROC::m1);
  f_m_m1.filter(MATRIX_PROC::filter_words);
  m_processor::draw_matrix(f_m_m1,feature_m1,MATRIX_PROC::dFunc_words);
  //draw_histogram(f_m_m1,feature_m1);
  output_image("f1.jpg",feature_m1);

  m_image_data feature_area = m_image::create(map_data.width,map_data.height);
  m_matrix_rect* areas = NULL;
  int area_size=64;
  int n = 0;
  m_matrix f_m_area64= f_m_m1;
  while(area_size>=8)
  {
    m_matrix m1(area_size,area_size,1.0f);
    //m1.print();
    f_m_area64 = f_m_m1.conv_scale(m1,areas,n);
    //mt.print();
    
    n = f_m_area64.width*f_m_area64.height;
    float max_k = f_m_area64.maxf();
    int m=0;
    if(areas!=NULL)free(areas);
    areas=(m_matrix_rect *)malloc(n*sizeof(m_matrix_rect));
    m_image::clear(feature_area,m_image_color::BLACK);
    
    for(int i=0;i<f_m_area64.height;i++)
    {
      for(int j=0;j<f_m_area64.width;j++)
      {
        int x=j*area_size;
        int y=i*area_size;
        int flag = 1;
        float value = f_m_area64.get(j,i);
        if(value<=0.1f*max_k)flag=0;
        m_matrix_rect rect{x,y,area_size,area_size,flag};
        *(areas+m)=rect;
        if(flag==1){
          m_image_rect img_rect{x,y,x+area_size,y+area_size};
          unsigned char c = (unsigned char)(255.0f*value/max_k);
          unsigned char color[]={c,c,c};
          m_image::fill_rect(feature_area,img_rect,color);
        }
        m++;
      }
    }
    area_size/=2;
  }
  output_image("fa1.jpg",feature_area);
  
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

  cout<<a_width<<",/////,"<<a_height<<endl;
  
  m_image_data edge_proc = m_image::create(a_width,a_height);
  m_matrix a_map_data = map_data.subset(min_x,min_y,a_width,a_height);
  m_processor::draw_matrix(a_map_data,edge_proc,MATRIX_PROC::dFunc_binary);
  output_image("e1.jpg",edge_proc);
  
  int dtf_img_size = 128;
  int dtf_size = 128;
  int dtf_offset_x = 0;
  int dtf_offset_y = 0;
  m_image_data dtf_img = m_image::create(dtf_img_size,dtf_img_size);
  m_matrix dtf_map_data = map_data.subset(min_x+dtf_offset_x,min_y+dtf_offset_y,dtf_img_size,dtf_img_size);
  m_processor::draw_matrix(dtf_map_data,dtf_img,MATRIX_PROC::dFunc_binary);
  output_image("dfto.jpg",dtf_img);

  dtf_img = m_image::create(dtf_size,dtf_size);
  dtf_map_data = map_data.subset(min_x+dtf_offset_x,min_y+dtf_offset_y,dtf_img_size,dtf_img_size);

  
  m_matrix dtf_data = m_processor::fft2(dtf_map_data,dtf_size);
  //dtf_data.print();
  dtf_data.filter(MATRIX_PROC::mapper_DFT);
  
  float dtf_max = dtf_data.maxf();
  dtf_data.mul(1.0f/dtf_max*255.0f);
  m_processor::draw_matrix(dtf_data,dtf_img,MATRIX_PROC::dFunc_gray);
  output_image("dft.jpg",dtf_img);

  dtf_data.filter(MATRIX_PROC::filter_DFT);  
  m_processor::draw_matrix(dtf_data,dtf_img,MATRIX_PROC::dFunc_binary);
  output_image("dft2.jpg",dtf_img);

  int hough_stepsThelta = 256;
  int hough_stepsLength = 256;
  m_image_data hough_img = m_image::create(hough_stepsThelta,hough_stepsLength);
  m_matrix hough_map = m_processor::ht_line(dtf_data,hough_stepsThelta,hough_stepsLength);
  m_matrix_index_info hough_max = hough_map.max();

  std::cout<<hough_max.value<<":"<<hough_max.x<<","<<hough_max.y<<std::endl;
  hough_map.mul(1.0f/hough_max.value*255.0f);
  m_processor::draw_matrix(hough_map,hough_img,MATRIX_PROC::dFunc_gray);
  output_image("hough.jpg",hough_img);
  //m_matrix a_map_verts = m_processor::vert_mark(a_map_data,7,VERT_PROC::direct_verts,VERT_PROC::direct_verts_r,VERT_PROC::n_theta);

  //m_processor::draw_verts(a_map_verts,edge_proc,VERT_PROC::func_vert);
  //m_image::draw_rect(edge_proc,m_image_rect{(int)min_x,(int)min_y,(int)max_x,(int)max_y},m_image_color::WHITE);
  


  std::cout<<f_m_area64.width<<","<<f_m_area64.height<<std::endl;
  output_image("fa1.jpg",feature_area);

  

  std::cout<<get_timestamp()-_t<<" ms"<<std::endl;
  return 0;
}