#include "IMG.h"
#include "MATRIX.h"
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
  
};
#endif