#include "../include/m_line.h"

using namespace cv;
using namespace std;

m_line::m_line(double x, double y, float theta,float length,int flag)
{
    double t = theta/180*CV_PI;
    double a = cos(t), b = sin(t);
    this->x = x;
    this->y = y;
    this->x_ = x+a*10;
    this->y_ = y-b*10;
    this->flag = flag;
    this->theta = theta;
    this->length = length;
}

m_line::m_line(double x, double y, double x_,double y_)
{
    float dx = x_-x;
    float dy = y_-y;
    float theta = -atan2(dy, dx);
    float degTheta = (float)((int)((theta / CV_PI * 180 + 360) * 100) % 36000) / 100.0;
    this->x = x;
    this->y = y;
    this->x_ = x_;
    this->y_ = y_;
    this->flag = -1;
    this->theta = degTheta;
    this->length = -1;
}

bool m_line::operator<(const m_line &a)
{
    return this->theta < a.theta;
}
void m_line::drawToMat(Mat &mat, Scalar &color, int thickness = 1)
{
    double t = theta/180*CV_PI;
    double a = cos(t), b = sin(t);
    double x0 = this->x, y0 = this->y;
    Point pt1(cvRound(x0 - 2000 * a), cvRound(y0 + 2000 * b));
    Point pt2(cvRound(x0 + 2000 * a), cvRound(y0 - 2000 * b));
    line(mat, pt1, pt2, color, thickness);
}

Point2d m_line::intersection(m_line &line){
    Point2d ret(0,0);
    // Store the values for fast access and easy
    // equations-to-code conversion
    double x1 = x, x2 = x_, x3 = line.x, x4 = line.x_;
    double y1 = y, y2 = y_, y3 = line.y, y4 = line.y_;

    double d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    // If d is zero, there is no intersection
    if (d == 0)
        return ret;

    // Get the x and y
    double pre = (x1 * y2 - y1 * x2), post = (x3 * y4 - y3 * x4);
    double x = (pre * (x3 - x4) - (x1 - x2) * post) / d;
    double y = (pre * (y3 - y4) - (y1 - y2) * post) / d;

    // Return the point of intersection
    ret.x = x;
    ret.y = y;
    return ret;
}