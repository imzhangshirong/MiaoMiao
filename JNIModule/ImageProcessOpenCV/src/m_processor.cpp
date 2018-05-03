#include "../include/m_processor.h"

using namespace cv;
using namespace std;

Mat m_processor::prewitt(Mat &img)
{
  Mat re = Mat_<double>::zeros(img.rows, img.cols);
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
        if (md > 16)
        {
          re.at<double>(i, j) = 1.0f;
        }
      }
    }
  }
  return re;
}

Mat m_processor::fft2(Mat &img)
{
    Mat imgChannels[img.channels()];
    split(img,imgChannels);
    Mat I = imgChannels[0];
    
    //Mat I = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
    Mat padded;                            //expand input image to optimal size
    int m = getOptimalDFTSize( I.rows );
    int n = getOptimalDFTSize( I.cols ); // on the border add zero values
    copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));

    Mat planes[] = {Mat_<double>(padded), Mat::zeros(padded.size(), CV_64F)};
    Mat complexI;
    merge(planes, 2, complexI);         // Add to the expanded another plane with zeros

    dft(complexI, complexI);            // this way the result may fit in the source matrix

    // compute the magnitude and switch to logarithmic scale
    // => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
    split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude  
    Mat magI = planes[0];
    
    magI += Scalar::all(1);                    // switch to logarithmic scale
    log(magI, magI);

    // crop the spectrum, if it has an odd number of rows or columns
    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

    // rearrange the quadrants of Fourier image  so that the origin is at the image center        
    int cx = magI.cols/2;
    int cy = magI.rows/2;

    Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant 
    Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
    Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
    Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right

    Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);

    normalize(magI, magI, 1, 0, CV_MINMAX); // Transform the matrix with double values into a 
                                            // viewable image form (double between values 0 and 1).
    return magI;
}