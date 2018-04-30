#include "../include/m_edge.h"

using namespace cv;
using namespace std;

Mat m_edge::prewitt(Mat &img)
{
  Mat re = Mat_<float>::zeros(img.rows, img.cols);
  for (int i = 1; i < img.rows - 1; i += 1)
  {
    for (int j = 1; j < img.cols - 1; j += 1)
    {
      int rgb11 = img.at<unsigned char>(i - 1, j - 1);
      int rgb12 = img.at<unsigned char>(i - 1, j);
      int rgb13 = img.at<unsigned char>(i - 1, j + 1);
      int rgb21 = img.at<unsigned char>(i, j - 1);
      int rgb22 = img.at<unsigned char>(i, j);
      int rgb23 = img.at<unsigned char>(i, j + 1);
      int rgb31 = img.at<unsigned char>(i + 1, j - 1);
      int rgb32 = img.at<unsigned char>(i + 1, j);
      int rgb33 = img.at<unsigned char>(i + 1, j + 1);
      int fy = (rgb11 - rgb31) + 2 * (rgb12 - rgb32) + (rgb13 - rgb33);
      int fx = (rgb13 - rgb11) + 2 * (rgb23 - rgb21) + (rgb33 - rgb31);
      double d = pow(fy * fy + fx * fx, 0.5);
      if (d > 0)
      {
        double ky = (double)fy / d;
        double kx = (double)fx / d;
        int dx = (int)(kx * 2);
        int dy = (int)(ky * 2);
        if (dx > 1)
        {
          dx = 1;
        }
        else if (dx < -1)
        {
          dx = -1;
        }
        if (dy > 1)
        {
          dy = 1;
        }
        else if (dy < -1)
        {
          dy = -1;
        }
        int rgbS = img.at<unsigned char>(i + dy, j + dx);
        int rgbE = img.at<unsigned char>(i - dy, j - dx);
        int md = abs(rgbE - rgbS);
        if (md > 20)
        {
          re.at<float>(i, j) = 1.0f;
        }
      }
    }
  }
  return re;
}