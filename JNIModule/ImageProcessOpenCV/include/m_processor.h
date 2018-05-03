#ifndef M_PROCESSOR_H
#define M_PROCESSOR_H
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>


using namespace cv;

class m_processor
{
public:
  static Mat prewitt(Mat &gray_img);
  static Mat fft2(Mat &img);
};
#endif