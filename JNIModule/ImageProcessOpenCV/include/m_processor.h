#ifndef M_PROCESSOR_H
#define M_PROCESSOR_H
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;

class m_processor
{
  public:
    static Mat prewitt(Mat &gray_img);
    static Mat fft2(Mat &img);
    static Mat transPrevMat(Mat &src, Mat &mat, Point2f &offset, Size &size);
    static Mat rotateImage(Mat &mat, float degree);
    static Mat getPerspectiveTransformMat(Mat &imgF, int threshold, int minLength, double gap, int paddingCross, vector<Point2f> &pcors, vector<Point2f> &pcorsTrans, Point2f *poffset);
    static float fft2HoughAngle(Mat &imgF);
    static double computePolygonArea(const vector<Point2d> &points);
    static Mat equalizeHist3C(Mat &mat);
};
#endif