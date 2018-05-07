#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <algorithm>
#include <sys/time.h>
#include "src/m_matrix.cpp"
#include "src/m_line.cpp"
#include "src/m_processor.cpp"


using namespace cv;
using namespace std;

long get_timestamp()
{
    struct timeval start; //s
    struct timeval end;   //us

    gettimeofday(&start, NULL);
    return start.tv_sec * 1000 + start.tv_usec / 1000;
}

double filter_words(Mat mat, int r, int c, double value)
{
    if (value > 3)
    {
        return 1.0;
    }
    else
    {
        return 0.0;
    }
}

double filter_reverse(Mat mat, int r, int c, double value)
{
    if (value > 0)
    {
        return 0;
    }
    else
    {
        return 1.0;
    }
}

double filter_fft2(Mat mat, int r, int c, double value)
{
    if (value > 0.75)
    {
        return 1.0;
    }
    else
    {
        return 0.0;
    }
}

int main(int argc, const char **argv)
{
    float scale = 0.8f;
    int kernelSize = 48 * scale;
    int kernelSize2 = 33 * scale;
    int kernelSize3 = 13 * (1.2 + scale) / 2;
    int kernelSize4 = 7;
    int lineThreshold = 30 * scale;
    int minLength = 120 * scale;
    double gap = 200 * scale;
    int paddingCross = 1000 * scale;
    double contourArea = 2048 * scale;
    double contourAreaSize = 80 * scale;
    double wordConvA[] = {
        1.1, -1.0, 1.0, -1.0, 1.1,
        -1.0, 1.1, -1.1, 1.1, -1.0,
        1.0, -1.1, 1.0, -1.1, 1.0,
        -1.0, 1.1, -1.1, 1.1, -1.0,
        1.1, -1.0, 1.0, -1.0, 1.1};
    Mat kernelEle = getStructuringElement(MORPH_RECT, Size(kernelSize, kernelSize));
    Mat kernelEle2 = getStructuringElement(MORPH_ELLIPSE, Size(kernelSize2, kernelSize2));
    Mat kernelEle3 = getStructuringElement(MORPH_ELLIPSE, Size(kernelSize3, kernelSize3));
    Mat kernelEle4 = getStructuringElement(MORPH_ELLIPSE, Size(kernelSize4, kernelSize4));
    long __t = get_timestamp();
    //Read Image
    Mat img = imread("input/img07.jpg", CV_LOAD_IMAGE_COLOR);
    if (img.empty())
    {
        cout << "Error:LoadImageFailed" << endl;
        return -1;
    }
    //blur(img,img, Size(3, 3));
    //img = equalizeHist3C(img);
    Mat resizeImg;
    resize(img, resizeImg, Size(img.cols * scale, img.rows * scale), 0, 0, INTER_LINEAR);
    //Split Single Channel
    Mat imgBGR[3];
    split(resizeImg, imgBGR);
    namedWindow("B Chanel", CV_WINDOW_FREERATIO);
    imshow("B Chanel", imgBGR[0]);
    namedWindow("G Chanel", CV_WINDOW_FREERATIO);
    imshow("G Chanel", imgBGR[1]);
    namedWindow("R Chanel", CV_WINDOW_FREERATIO);
    imshow("R Chanel", imgBGR[2]);
    //RGB to Gray
    Mat gray;
    cvtColor(resizeImg, gray, CV_RGB2GRAY);
    //blur(gray, gray, Size(3, 3));
    /*Mat canny;
    
    //运行canny算子
    Canny(canny, canny, 3, 9, 3);
    namedWindow("CannyEdge", CV_WINDOW_FREERATIO);
    imshow("CannyEdge", canny);*/
    //Prewitt Edge Detect
    Mat edge = m_processor::prewitt(gray);
    namedWindow("PrewittEdge", CV_WINDOW_FREERATIO);
    imshow("PrewittEdge", edge);
    //Words Edge Detect
    Mat wordConv = Mat_<double>(5, 5, wordConvA);
    Mat wordEdge = m_matrix::conv(edge, wordConv);
    wordEdge = m_matrix::filter(wordEdge, filter_words);
    namedWindow("WordEdge", CV_WINDOW_FREERATIO);
    imshow("WordEdge", wordEdge);
    cout << "Spend Time:" << get_timestamp() - __t << "ms" << endl;
    //Words Edge Dilate
    Mat wordEdgeDilated;
    dilate(wordEdge, wordEdgeDilated, kernelEle);
    erode(wordEdgeDilated, wordEdgeDilated, kernelEle);
    dilate(wordEdgeDilated, wordEdgeDilated, kernelEle2);
    erode(wordEdgeDilated, wordEdgeDilated, kernelEle2);
    dilate(wordEdgeDilated, wordEdgeDilated, kernelEle3);
    erode(wordEdgeDilated, wordEdgeDilated, kernelEle3);
    dilate(wordEdgeDilated, wordEdgeDilated, kernelEle4);
    erode(wordEdgeDilated, wordEdgeDilated, kernelEle4);
    cout << "Spend Time:" << get_timestamp() - __t << "ms" << endl;
    //namedWindow("WordEdgeDilated", CV_WINDOW_FREERATIO);
    //imshow("WordEdgeDilated", wordEdgeDilated);
    //Words Rect Detect //https://docs.opencv.org/2.4/doc/tutorials/imgproc/shapedescriptors/bounding_rotated_ellipses/bounding_rotated_ellipses.html
    vector<vector<Point>> contours; //定义轮廓集合
    vector<Vec4i> hierarchy;
    Mat contourDetect = m_matrix::to8UC1(wordEdgeDilated);
    findContours(contourDetect, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE); //CV_RETR_EXTERNAL只检测外部轮廓
    vector<vector<Point>> contours_poly(contours.size());
    // draw black contours on white image
    Mat mask(wordEdgeDilated.size(), CV_8UC3, Scalar(0));
    Mat result(wordEdgeDilated.size(), CV_64F, Scalar(0));
    int index = 0;
    for (; index >= 0; index = hierarchy[index][0]) //hierarchy[index][0]表示后一个轮廓
    {
        //approxPolyDP(contours[index], contours_poly[index], 3, false );
        RotatedRect rect = minAreaRect(contours[index]);
        if (rect.size.area() > contourArea && rect.size.height > contourAreaSize && rect.size.width > contourAreaSize)
        {
            drawContours(result, contours, index, Scalar(1), 1, 8, hierarchy);
        }
        /*RotatedRect rect = minAreaRect(contours[index]);
        if (rect.size.area() > 2048)
        {
            Point2f pts[4];
            rect.points(pts);
            Point points[1][4];
            int npt[] = {4};
            for (int j = 0; j < 4; j++)
            {
                points[0][j] = Point(pts[j]);
            }
            const Point *ppt[1] = {points[0]};
            //polylines(result, ppt, npt, 1, 1, Scalar(1), 3);
            fillPoly(mask, ppt, npt, 1, Scalar(255, 255, 255));
            cout << rect.angle << "|" << rect.size.area() << endl;
        }*/
        //drawCvBox2D(result,box,Scalar(0,0,255), 3);
        /*Rect rect = boundingRect(contours[index]);//检测外轮廓
        if(rect.area()>2048){
            rectangle(result, rect, Scalar(0,0,255), 3);//对外轮廓加矩形框
        }*/
    }
    Point2f poffset(0,0);
    vector<Point2f> pcors;
    vector<Point2f> pcorsTrans;
    Mat prevMat = m_processor::getPerspectiveTransformMat(result, lineThreshold, minLength, gap, paddingCross,pcors,pcorsTrans,&poffset);
    Mat prevScaled;
    if(pcorsTrans.size()==4){
        Size paddingSize = Size((int)pcorsTrans[0].x,(int)pcorsTrans[0].y);
        prevScaled = m_processor::transPrevMat(wordEdge,prevMat,poffset,paddingSize);
        imshow("prevScaled",prevScaled);
    }
    else{
        prevScaled = wordEdge;
    }
    Mat pixelDistrt = Mat(Size(prevScaled.cols,prevScaled.rows),CV_8UC3,Scalar(0));
    int xDistrt[prevScaled.cols] = {0};
    int yDistrt[prevScaled.rows] = {0};
    for(int i=0;i<prevScaled.cols;i++){
        for(int j=0;j<prevScaled.rows;j++){
            if(prevScaled.at<double>(j,i)>0)xDistrt[i]++;
        }
        line(pixelDistrt,Point(i,0),Point(i,xDistrt[i]),Scalar(255,255,0),1);
    }
    for(int i=0;i<prevScaled.rows;i++){
        for(int j=0;j<prevScaled.cols;j++){
            if(prevScaled.at<double>(i,j)>0)yDistrt[i]++;
        }
        line(pixelDistrt,Point(0,i),Point(yDistrt[i],i),Scalar(0,255,255),1);
    }
    imshow("pixelDistrt",pixelDistrt);
    cout << "Spend Time:" << get_timestamp() - __t << "ms" << endl;
    waitKey(0);
    return 0;
}
