#ifndef M_PROCESSOR_H
#define M_PROCESSOR_H

#include "m_image.h"
#include "m_matrix.h"
#include "m_vert2.h"

class m_processor
{
public:
  static void binary(m_image_data &image_data,m_image_data &image_data_out,int threshold,int range);
  static void binary2(m_image_data &image_data,m_image_data &image_data_out,int threshold,int range,int whiteDirect);
  static void block_binary(m_image_data &image_data,int block_width,int block_height,bool (*bFunc)(int));
  static void gray_scale(m_image_data &image_data,m_image_data &image_data_out);
  static m_matrix gray2matrix(m_image_data &image_data,int* grayMap,int* averageGray);
  static void draw_matrix(m_matrix &matrix,m_image_data &image_data_out,unsigned char* (*func)(int x,int y,float value));
  static m_matrix draw_vert_mark(m_matrix &matrix,int block_size,m_vert2* verts_d,float* verts_d_r,int verts_d_length);
  static void draw_verts(m_matrix &matrix,m_image_data &image_data_out,void (*func)(m_image_data image_data,int x,int y,float value));
  static m_matrix fft2(m_matrix &matrix,int size);
  static m_matrix dft2(m_matrix &matrix,int size);
  static m_matrix ht_line(m_matrix &matrix,int theltaStep,int lengthStep);//Hought Tranform
  static vector<m_domain> bwlabel(m_matrix &matrix,float threshold);
  static void draw_labeled_domain(vector<m_domain> &domains,m_image_data &image_data_out);
};
#endif