#ifndef M_EDGE_H
#define M_EDGE_H
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;

class m_edge
{
public:
  static Mat prewitt(Mat &gray_img);
};
#endif