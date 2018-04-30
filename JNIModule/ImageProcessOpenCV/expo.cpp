#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "src/m_edge.cpp"
#include "src/m_matrix.cpp"

using namespace cv;
using namespace std;


float filter_words(int r, int c, float value)
{
    if (value > 3.0f)
    {
        return 1.0f;
    }
    else
    {
        return 0.0f;
    }
}

int main(int argc, const char **argv)
{
    //read Image
    Mat img = imread("input/img01.jpg", CV_LOAD_IMAGE_COLOR);
    if (img.empty())
    {
        cout << "Error:LoadImageFailed" << endl;
        return -1;
    }
    //RGB to Gray
    Mat gray;
    cvtColor(img, gray, CV_RGB2GRAY);
    //Prewitt Edge Detect
    Mat edge = m_edge::prewitt(gray);
    namedWindow("PrewittEdge", CV_WINDOW_FREERATIO);
    imshow("PrewittEdge", edge);
    //Words Edge Detect
    float wordConvA[] = {
        1.0f, -1.0f, 1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, 1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, 1.0f, -1.0f, 1.0f};
    Mat wordConv = Mat_<float>(5, 5, wordConvA);
    Mat wordEdge = m_matrix::conv(edge, wordConv);
    wordEdge = m_matrix::filter(wordEdge, filter_words);
    namedWindow("WordEdge", CV_WINDOW_FREERATIO);
    imshow("WordEdge", wordEdge);

    waitKey(0);
    return 0;
}