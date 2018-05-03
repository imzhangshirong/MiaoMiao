#ifndef M_LINE_H
#define M_LINE_H
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

class m_line
{
  public:
    double x;
    double y;
    double x_;
    double y_;
    float theta;
    float length;
    int flag;

    m_line(double x, double y, float theta,float length,int flag);
    m_line(double x, double y, double x_,double y_);
    bool operator<(const m_line &a);
    void drawToMat(Mat &mat,Scalar &color,int thickness);
    Point2d intersection(m_line &line);
};

#endif