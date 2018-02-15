#include "IMG.h"
#include "MATRIX.h"
#include "VERT.h"
#ifndef PROCESSOR_H
#define PROCESSOR_H


class PROCESSOR
{
public:
  static void binary(IMG_DATA image_data,IMG_DATA image_data_out,int threshold,int range);
  static void binary2(IMG_DATA image_data,IMG_DATA image_data_out,int threshold,int range,int whiteDirect);
  static void block_binary(IMG_DATA image_data,int block_width,int block_height,bool (*bFunc)(int));
  static void gray_scale(IMG_DATA image_data,IMG_DATA image_data_out);
  static MATRIX2 gray2matrix(IMG_DATA image_data,int* grayMap,int* averageGray);
  static void draw_matrix(MATRIX2 matrix,IMG_DATA image_data_out,unsigned char* (*func)(int x,int y,float value));
  static MATRIX2 vert_mark(MATRIX2 matrix,int block_size,VERT2* verts_d,float* verts_d_r,int verts_d_length);
  static void draw_verts(MATRIX2 matrix,IMG_DATA image_data_out,void (*func)(IMG_DATA image_data,int x,int y,float value));
  static MATRIX2 FFT2(MATRIX2 matrix,int size);
  static MATRIX2 DFT(MATRIX2 matrix,int size);
  static void draw_DFT(MATRIX2 matrix,IMG_DATA image_data_out);
};
#endif