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
    static Mat filter(Mat &mat, float (*func)(int, int, float));
    static Mat conv(Mat &mat, Mat &matConv);
};

#endif