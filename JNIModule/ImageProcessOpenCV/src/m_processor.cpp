#include "../include/m_processor.h"

using namespace cv;
using namespace std;

Mat m_processor::prewitt(Mat &img)
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


vector<m_domain> m_processor::bwlabel(Mat &matrix, float threshold)
{
  Mat labelM = Mat_<int>::zeros(matrix.rows,matrix.cols);
  int label = 1;
  for (int i = 0; i < matrix.rows; i++)
  {
    float last = matrix.at<float>(i, 0);
    for (int j = 0; j < matrix.cols; j++)
    {
      float value = matrix.at<float>(i, j);
      if (i == 0)
      {
        if ((abs(last - value) > threshold))
        {
          label++;
        }
        labelM.at<int>(i, j)=label;
      }
      else
      {
        float v1 = -1;
        float v2 = matrix.at<float>(i - 1,j);
        float v3 = -1;
        if (j - 1 >=0)
          v1 = matrix.at<float>(i - 1, j - 1);
        if (j + 1 < matrix.cols)
          v3 = matrix.at<float>(i - 1, j + 1);
        int labeln = -1;
        if (v1>=0 && abs(v1 - value) <= threshold)
        {
          int t = labelM.at<int>(i-1, j - 1);
          if(labeln<0 || t<labeln)labeln = t;
        }
        if (v2>=0 && abs(v2 - value) <= threshold)
        {
          int t = labelM.at<int>(i-1, j);
          if(labeln<0 || t<labeln)labeln = t;
        }
        if (v3>=0 && abs(v3 - value) <= threshold)
        {
          int t = labelM.at<int>( i - 1,j+1);
          if(labeln<0 || t<labeln)labeln = t;
        }
        if (j>0 && (abs(last - value) <= threshold))
        {
          int t = labelM.at<int>(i,j-1);
          if(labeln<0 || t<labeln)labeln = t;
        }
        if(labeln<0){
          label++;
          labeln = label;
        }
        labelM.at<int>(i, j)= labeln;
      }
      last = value;
    }
  }
  m_domain *labels = new m_domain[label];
  for (int i = matrix.rows-2; i >=0; i--)
  {
    float last = matrix.at<float>(i, 0);
    for (int j = matrix.cols-1; j >=0; j--)
    {
      float value = matrix.at<float>(i, j);
      {
        float v1 = -1;
        float v2 = matrix.at<float>(i + 1,j);
        float v3 = -1;
        if (j - 1 >=0)
          v1 = matrix.at<float>(i + 1,j - 1);
        if (j + 1 < matrix.cols)
          v3 = matrix.at<float>( i + 1,j + 1);
        int labeln = -1;
        if (v1>=0 && abs(v1 - value) <= threshold)
        {
          int t = labelM.at<int>(i +1,j-1);
          if(labeln<0 || t<labeln)labeln = t;
        }
        if (v2>=0 && abs(v2 - value) <= threshold)
        {
          int t = labelM.at<int>(i+1, j);
          if(labeln<0 || t<labeln)labeln = t;
        }
        if (v3>=0 && abs(v3 - value) <= threshold)
        {
          int t = labelM.at<int>(i+1, j + 1);
          if(labeln<0 || t<labeln)labeln = t;
        }
        int old = labelM.at<int>(i, j);
        if(labeln>0 && labeln<old){
          labels[old-1].flag=-1;
          labelM.at<int>(i, j)= labeln;
        }
      }
      last = value;
    }
  }
  vector<m_domain> domains;
  //labelM.print();
  for (int i = 0; i < matrix.rows; i++)
  {
    for (int j = 0; j < matrix.cols; j++)
    {
      int labelc = (int)labelM.at<int>(i,j);
      m_point index;
      index.col = j;
      index.row = i;
      labels[labelc-1].indeces.push_back(index);
    }
  }

  for(int i=0;i<label;i++)
  {
    if(labels[i].indeces.size()>0)
    {
      domains.push_back(labels[i]);
    }
  }
  delete [] labels;
  /*for(int i=0;i<domains.size();i++)
  {
    cout<<"////"<<domains[i].indeces.size()<<endl;    
  }*/
  return domains;
}