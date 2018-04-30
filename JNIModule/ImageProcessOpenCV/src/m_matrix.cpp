#include "../include/m_matrix.h"

using namespace cv;
using namespace std;

Mat m_matrix::filter(Mat &mat, float (*func)(int, int, float))
{
    Mat re = Mat_<float>::zeros(mat.rows, mat.cols);
    for (int i = 0; i < mat.rows; i++)
    {
        for (int j = 0; j < mat.cols; j++)
        {
            re.at<float>(i, j) = func(i, j, mat.at<float>(i, j));
        }
    }
    return re;
}

Mat m_matrix::conv(Mat &mat, Mat &matConv)
{
    int half = matConv.rows / 2;
    Mat re = Mat_<float>::zeros(mat.rows, mat.cols);
    int base_x = 0;
    int base_y = 0;
    int x = 0;
    int y = 0;
    for (int i = 0; i < mat.rows; i++)
    {
        for (int j = 0; j < mat.cols; j++)
        {
            float temp = 0.0f;
            for (int k = 0; k < matConv.rows; k++)
            {
                for (int l = 0; l < matConv.cols; l++)
                {
                    x = base_x + l - half;
                    y = base_y + k - half;
                    if (x >= 0 && y >= 0 && x < mat.cols && y < mat.rows)
                    {
                        temp += mat.at<float>(y, x) * matConv.at<float>(k, l);
                    }
                }
            }
            re.at<float>(i, j) = temp;
            base_x++;
        }
        base_x = 0;
        base_y++;
    }
    return re;
}
/*
m_matrix m_matrix::conv_scale(m_matrix &matrix)
{
    int max_width = this->width / matrix.width;
    int max_height = this->height / matrix.height;
    int cur = 0;
    int row = 0;
    int col = 0;
    int x = 0;
    int y = 0;
    m_matrix re(max_width, max_height);
    for (int i = 0; i < re.height; i++)
    {
        x = 0;
        for (int j = 0; j < re.width; j++)
        {
            float temp = 0.0f;
            y = 0;
            for (int k = 0; k < matrix.height; k++)
            {
                for (int l = 0; l < matrix.width; l++)
                {
                    float curValue = *(this->m_map + cur + y + x + l);
                    temp += matrix.get(l, k) * curValue;
                }
                y += this->width;
            }
            x += matrix.width;
            *(re.data() + col) = temp;
            col++;
        }
        cur += this->width * matrix.height;
    }

    return re;
}*/