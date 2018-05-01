#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "src/m_processor.cpp"
#include "src/m_matrix.cpp"

using namespace cv;
using namespace std;


float filter_words(Mat mat, int r, int c, float value)
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
    //Read Image
    Mat img = imread("input/img01.jpg", CV_LOAD_IMAGE_COLOR);
    if (img.empty())
    {
        cout << "Error:LoadImageFailed" << endl;
        return -1;
    }
    //Split Single Channel
    Mat imgBGR[3];
    split(img,imgBGR);
    namedWindow("B Chanel",CV_WINDOW_FREERATIO);
    imshow("B Chanel",imgBGR[0]);
    namedWindow("G Chanel",CV_WINDOW_FREERATIO);
    imshow("G Chanel",imgBGR[1]);
    namedWindow("R Chanel",CV_WINDOW_FREERATIO);
    imshow("R Chanel",imgBGR[2]);
    //RGB to Gray
    Mat gray;
    cvtColor(img, gray, CV_RGB2GRAY);
    //Prewitt Edge Detect
    Mat edge = m_processor::prewitt(gray);
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
    //Words Edge Dilate
    Mat wordEdgeDilated;
    Mat structEle = getStructuringElement(MORPH_RECT,Size(32,32));
    dilate(wordEdge,wordEdgeDilated,structEle);
    //Mat structEle2 = getStructuringElement(MORPH_ELLIPSE,Size(16,16));
    //erode(wordEdgeDilated,wordEdgeDilated,structEle2);
    namedWindow("WordEdgeDilated", CV_WINDOW_FREERATIO);
    imshow("WordEdgeDilated", wordEdgeDilated);
    //Words Rect Detect
    Mat ttt = m_matrix::to8UC1(wordEdgeDilated,1);
    CvMemStorage* mem_storage = cvCreateMemStorage(0);      
    CvSeq *first_contour = NULL, *c = NULL;
    IplImage img_temp = IplImage(ttt);

    vector<vector<Point>> contours;  //定义轮廓集合  
    vector<Vec4i> hierarchy;
    Mat contourDetect = m_matrix::to8UC1(wordEdgeDilated,1);
    findContours(contourDetect, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//CV_RETR_EXTERNAL只检测外部轮廓
    // draw black contours on white image  
    Mat result(wordEdgeDilated.size(),CV_8UC3,Scalar(255,255,255));  
    int index = 0;  
    for (; index >= 0; index = hierarchy[index][0]) //hierarchy[index][0]表示后一个轮廓
    {
        drawContours(result, contours, index, Scalar(0), 1, 8, hierarchy);//描绘字符的外轮廓  
        Rect rect = boundingRect(contours[index]);//检测外轮廓
        if(rect.area()>2048){
        rectangle(result, rect, Scalar(0,0,255), 3);//对外轮廓加矩形框  

        }
    }
    namedWindow("WordEdgeDilatedContours", CV_WINDOW_FREERATIO);
    imshow("WordEdgeDilatedContours", result);

    /*Mat out;
    dft(wordEdge,out);
    namedWindow("WordEdgeDTF", CV_WINDOW_FREERATIO);
    imshow("WordEdgeDTF", out);*/
    waitKey(0);
    return 0;
}