#ifndef M_MATRIX_H
#define M_MATRIX_H
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;

class m_matrix
{
  public:
    static Mat filter(Mat &mat, float (*func)(Mat,int, int, float));
    static Mat conv(Mat &mat, Mat &matConv);
    static Mat to8UC1(Mat &mat,float max);
    static Mat to8UC3(Mat &mat,float max);
};

#endif