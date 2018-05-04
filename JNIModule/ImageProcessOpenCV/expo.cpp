#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <algorithm>
#include <sys/time.h>
#include "src/m_processor.cpp"
#include "src/m_matrix.cpp"
#include "src/m_line.cpp"

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

float fft2HoughAngle(Mat &imgF)
{
    vector<Vec4i> lines;
    Mat dst = m_matrix::to8UC1(imgF);
    HoughLinesP(dst, lines, 1, CV_PI / 180, 10, 10, 10);
    for (size_t i = 0; i < lines.size() && i < 1; i++)
    {
        Vec4i l = lines[i];
        float theta = atan2(l[1] - l[3], l[0] - l[2]);
        return theta / CV_PI * 180.0 + 90.0;
    }
    return 0;
}
double computePolygonArea(const vector<Point2d> &points)
{
    int point_num = points.size();
    if (point_num < 3)
        return 0.0;
    double s = points[0].y * (points[point_num - 1].x - points[1].x);
    for (int i = 1; i < point_num; ++i)
        s += points[i].y * (points[i - 1].x - points[(i + 1) % point_num].x);
    return fabs(s / 2.0);
}

void hough(Mat &imgF, int threshold, int minLength, double gap, int paddingCross)
{
    vector<Vec4i> lines;
    Mat dst = m_matrix::to8UC1(imgF);
    Mat color_dst;
    cvtColor(dst, color_dst, CV_GRAY2BGR);
    HoughLinesP(dst, lines, 1, CV_PI / 180, threshold, minLength, gap);
    vector<m_line> validLines;
    cout << "Lines:" << lines.size() << endl;
    for (int i = 0; i < lines.size(); i++)
    {
        Vec4i l = lines[i];
        float dy = l[3] - l[1];
        float dx = l[2] - l[0];
        float length = sqrtf(dx * dx + dy * dy);
        float theta = -atan2(dy, dx);
        float degTheta = (float)((int)((theta / CV_PI * 180 + 360) * 100) % 36000) / 100.0;
        if (degTheta > 180)
            degTheta -= 180;
        m_line line(l[0], l[1], degTheta, length, i);
        validLines.push_back(line);
    }
    Scalar colors[] = {
        Scalar(0, 0, 255),
        Scalar(0, 255, 0),
        Scalar(0, 255, 255),
        Scalar(255, 255, 0),
        Scalar(255, 0, 255),
    };
    sort(validLines.begin(), validLines.end());
    int cindex = 0;
    vector<vector<m_line>> lineGroup;
    vector<m_line> group;
    double lastTheta = -1;
    for (int i = 0; i < validLines.size(); i++)
    {
        float theta = validLines[i].theta;
        if (theta == 0)
            continue;
        float dt = theta - lastTheta;
        if (lastTheta > 0 && (dt > 45 || theta - group[0].theta > 30))
        {
            lineGroup.push_back(group);
            vector<m_line> groupn;
            group = groupn;
            cindex++;
        }
        if (dt < 0.1f)
        {
            if (group[group.size() - 1].flag > validLines[i].flag)
            {
                group.pop_back();
                group.push_back(validLines[i]);
                lastTheta = theta;
            }
        }
        else
        {
            group.push_back(validLines[i]);
            lastTheta = theta;
        }
    }
    if (group.size() > 0)
        lineGroup.push_back(group);
    for (int i = 0; i < lineGroup.size(); i++)
    {
        for (int j = 0; j <= i - 1; j++)
        {
            float t1 = lineGroup[i][0].theta;
            float t2 = lineGroup[j][0].theta;
            float dt = t2 + 180 - t1;
            if (abs(dt) < 45)
            {
                for (int k = 0; k < lineGroup[i].size(); k++)
                {
                    lineGroup[j].push_back(lineGroup[i][k]);
                }
                lineGroup.erase(lineGroup.begin() + i, lineGroup.begin() + i + 1);
                i--;
                break;
            }
        }
    }

    for (int i = 0; i < lineGroup.size(); i++)
    {
        for (int j = 0; j < lineGroup[i].size(); j++)
        {
            for (int k = j + 1; k < lineGroup[i].size(); k++)
            {
                Point2d p = lineGroup[i][j].intersection(lineGroup[i][k]);
                if (p.x > -paddingCross && p.y > -paddingCross && p.x < dst.cols + paddingCross && p.y < dst.rows + paddingCross)
                {
                    //m_line line(p.x,p.y,(lineGroup[i][j].theta+lineGroup[i][k].theta)/2,(lineGroup[i][j].length+lineGroup[i][k].length)/2);
                    if (lineGroup[i][k].length / lineGroup[i][j].length < 0.6 || lineGroup[i][k].theta == 90)
                    {
                        lineGroup[i].erase(lineGroup[i].begin() + k, lineGroup[i].begin() + k + 1);
                    }
                    else if (lineGroup[i][k].flag > lineGroup[i][j].flag)
                    {
                        lineGroup[i].erase(lineGroup[i].begin() + k, lineGroup[i].begin() + k + 1);
                    }
                    else
                    {
                        lineGroup[i].erase(lineGroup[i].begin() + j, lineGroup[i].begin() + j + 1);
                    }
                    j--;
                    circle(color_dst, Point(p.x, p.y), 10, CV_RGB(0, 255, 255), 2, 8, 0);
                    break;
                }
            }
        }
    }
    for (int i = 0; i < lineGroup.size(); i++)
    {
        for (int j = 0; j < lineGroup[i].size(); j++)
        {
            lineGroup[i][j].drawToMat(color_dst, colors[i]);
            cout << lineGroup[i][j].theta << "|" << lineGroup[i][j].length << "|" << lineGroup[i][j].flag << endl;
        }
        cout << "-------" << endl;
    }
    //消失点检测
    vector<vector<m_line>> crossLines;
    for (int i = 0; i < lineGroup.size(); i++)
    {
        vector<m_line> cross;
        vector<Point2d> points;
        for (int j = 0; j < lineGroup[i].size(); j++)
        {
            for (int k = j + 1; k < lineGroup[i].size(); k++)
            {
                Point2d p = lineGroup[i][j].intersection(lineGroup[i][k]);
                if (p.x != 0 || p.y != 0)
                {
                    cout << "OOcross:" << p.x << "," << p.y << endl;
                    points.push_back(p);
                }
            }
        }
        double areaC[4] = {0};
        for (int j = 0; j < points.size(); j++)
        {
            int a = 0;
            if (points[j].x > 0 && points[j].y > 0)
            {
                a = 0;
            }
            else if (points[j].x < 0 && points[j].y > 0)
            {
                a = 1;
            }
            else if (points[j].x < 0 && points[j].y < 0)
            {
                a = 2;
            }
            else if (points[j].x > 0 && points[j].y < 0)
            {
                a = 3;
            }
            double dx = points[j].x / 10000;
            double dy = points[j].y / 10000;
            double length = sqrt(dx * dx + dy * dy);
            areaC[a] += length + 1;
        }
        int maxAC = 0;
        int areaCI = 0;
        for (int j = 0; j < 4; j++)
        {
            if (areaC[j] > maxAC)
            {
                areaCI = j;
                maxAC = areaC[j];
            }
        }
        //cout<<"////////"<<areaCI<<endl;
        int dx = (areaCI == 0 || areaCI == 3) ? 1 : -1;
        int dy = (areaCI == 0 || areaCI == 1) ? 1 : -1;
        int n = 0;
        double xv = 0;
        double yv = 0;
        Point2d vp;
        double vpd = 0;
        for (int j = 0; j < points.size(); j++)
        {
            if (dx * points[j].x >= 0 && points[j].y * dy >= 0)
            {
                double length = points[j].x * points[j].x + points[j].y * points[j].y;
                if (length > vpd)
                {
                    vpd = length;
                    vp = points[j];
                }
                xv += points[j].x;
                yv += points[j].y;
                cout << "cross:" << points[j].x << "," << points[j].y << endl;
                n++;
            }
        }
        //n--;
        if (n > 0)
        {
            //xv-=vp.x;
            //yv-=vp.y;
            xv /= n;
            yv /= n;
            //xv = 0.5*xv+0.5*vp.x;
            //yv = 0.5*yv+0.5*vp.y;
        }
        else
        {
            xv = vp.x;
            yv = vp.y;
        }
        if (xv != 0 && yv != 0)
        {
            cross.push_back(m_line(xv, yv, 0, 0));
            cross.push_back(m_line(xv, yv, 0, dst.rows));
            cross.push_back(m_line(xv, yv, dst.cols, dst.rows));
            cross.push_back(m_line(xv, yv, dst.cols, 0));
            crossLines.push_back(cross);
            cout << "AVERcross:" << xv << "," << yv << endl;
            cout << "---------" << endl;
        }
    }
    cout << crossLines.size() << endl;
    vector<vector<Point2d>> crossPoints;
    for (int i = 0; i < 5; i++)
    {
        vector<Point2d> ps;
        crossPoints.push_back(ps);
    }
    int cx = dst.cols / 2;
    int cy = dst.rows / 2;
    for (int i = 0; i < crossLines.size() && i < 2; i++)
    {
        for (int j = i + 1; j < crossLines.size() && j < 2; j++)
        {
            for (int k = 0; k < crossLines[i].size(); k++)
            {
                for (int l = 0; l < crossLines[j].size(); l++)
                {
                    Point2d p = crossLines[i][k].intersection(crossLines[j][l]);
                    int a = 0;
                    if (p.x > cx && p.y > cy)
                    {
                        a = 1;
                    }
                    else if (p.x < cx && p.y > cy)
                    {
                        a = 2;
                    }
                    else if (p.x < cx && p.y < cy)
                    {
                        a = 3;
                    }
                    else if (p.x > cx && p.y < cy)
                    {
                        a = 4;
                    }
                    crossPoints[a].push_back(p);
                }
            }
        }
    }
    Point2d corP[4];
    float areaSize = 0;
    vector<Point2d> apts;
    for (int i = 0; i < crossPoints[1].size(); i++)
    {
        apts.push_back(crossPoints[1][i]);
        for (int j = 0; j < crossPoints[2].size(); j++)
        {
            apts.push_back(crossPoints[2][j]);
            for (int k = 0; k < crossPoints[3].size(); k++)
            {
                apts.push_back(crossPoints[3][k]);
                for (int l = 0; l < crossPoints[4].size(); l++)
                {
                    apts.push_back(crossPoints[4][l]);

                    double sizeA = computePolygonArea(apts);
                    if (sizeA > areaSize)
                    {
                        areaSize = sizeA;
                        corP[0] = apts[0];
                        corP[1] = apts[1];
                        corP[2] = apts[2];
                        corP[3] = apts[3];
                    }
                    apts.pop_back();
                }
                apts.pop_back();
            }
            apts.pop_back();
        }
        apts.pop_back();
    }
    int left = 0;
    int right = dst.cols;
    int top = 0;
    int bottom = dst.rows;
    int zeroP = 0;
    for (int i = 0; i < 4; i++)
    {
        cout << "cor:" << corP[i].x << "," << corP[i].y << endl;
        if (left > corP[i].x)
            left = corP[i].x;
        if (right < corP[i].x)
            right = corP[i].x;
        if (top > corP[i].y)
            top = corP[i].y;
        if (bottom < corP[i].y)
            bottom = corP[i].y;
        if (corP[i].x == 0 && corP[i].y == 0)
            zeroP++;
    }
    if ((-top) + (-left) + right + bottom < 10000 && zeroP < 2)
    {
        Point2d offset(-left, -top);

        cout << "top:" << top << ",left:" << left << ",bottom:" << bottom << ",right:" << right << endl;
        Mat paddingDst = Mat(Size(right - left, bottom - top), CV_8UC3, Scalar(0));
        Mat qArea(paddingDst, Rect(-left, -top, dst.cols, dst.rows));
        color_dst.copyTo(qArea);
        for (int i = 0; i < 4; i++)
        {
            int next = (i + 1) % 4;
            line(paddingDst, corP[i] + offset, corP[next] + offset, Scalar(255, 255, 0), 2);
        }
        namedWindow("Detected Lines", CV_WINDOW_FREERATIO);
        imshow("Detected Lines", paddingDst);

        Point2f corSrc[4]{
            Point2f(corP[0] + offset),
            Point2f(corP[1] + offset),
            Point2f(corP[2] + offset),
            Point2f(corP[3] + offset)};
        Point2f corDst[4]{
            Point2f(paddingDst.cols, paddingDst.rows),
            Point2f(0, paddingDst.rows),
            Point2f(0, 0),
            Point2f(paddingDst.cols, 0)};
        Mat tMat = getPerspectiveTransform(corSrc, corDst);
        Mat warp;
        cv::warpPerspective(paddingDst, warp, tMat, paddingDst.size(), cv::INTER_LINEAR);
        namedWindow("PerspectiveTransform", CV_WINDOW_FREERATIO);
        imshow("PerspectiveTransform", warp);
    }
    else
    {
        cout << "Do not Perspective Trans" << endl;
        namedWindow("Detected Lines", CV_WINDOW_FREERATIO);
        imshow("Detected Lines", color_dst);
    }
}

Mat rotateImage(Mat &mat, float degree)
{

    degree = -degree;

    double angle = degree * CV_PI / 180.0;
    double a = sin(angle), b = cos(angle);
    int width = mat.cols;
    int height = mat.rows;
    int width_rotate = int(height * fabs(a) + width * fabs(b));
    int height_rotate = int(width * fabs(a) + height * fabs(b));

    // [ m0  m1  m2 ] ===>  [ A11  A12   b1 ]
    // [ m3  m4  m5 ] ===>  [ A21  A22   b2 ]

    float map[6];
    Mat map_matrix = Mat(2, 3, CV_32F, map);

    // 旋转中心
    CvPoint2D32f center = cvPoint2D32f(width / 2, height / 2);
    CvMat map_matrix2 = map_matrix;
    cv2DRotationMatrix(center, degree, 1.0, &map_matrix2);
    map[2] += (width_rotate - width) / 2;
    map[5] += (height_rotate - height) / 2;
    Mat img_rotate;

    //对图像做仿射变换

    //CV_WARP_FILL_OUTLIERS - 填充所有输出图像的象素。

    //如果部分象素落在输入图像的边界外，那么它们的值设定为 fillval.

    //CV_WARP_INVERSE_MAP - 指定 map_matrix 是输出图像到输入图像的反变换，

    warpAffine(mat, img_rotate, map_matrix, Size(width_rotate, height_rotate), 1, 0, 0);

    return img_rotate;
}

Mat equalizeHist3C(Mat &mat)
{
    Mat mergeImg;
    vector<Mat> splitBGR(mat.channels());
    split(mat, splitBGR);
    for (int i = 0; i < mat.channels(); i++)
        equalizeHist(splitBGR[i], splitBGR[i]);
    merge(splitBGR, mergeImg);
    return mergeImg;
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
    //dilate(wordEdgeDilated, wordEdgeDilated, kernelEle4);

    cout << "Spend Time:" << get_timestamp() - __t << "ms" << endl;
    /*erode(wordEdgeDilated, wordEdgeDilated, kernelEle);
    erode(wordEdgeDilated, wordEdgeDilated, kernelEle);
    erode(wordEdgeDilated, wordEdgeDilated, kernelEle2);   */
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
    //dilate(result, result, kernelEle2);
    //erode(result, result, kernelEle2);
    /*namedWindow("WordEdgeDilatedContours", CV_WINDOW_FREERATIO);
    imshow("WordEdgeDilatedContours", result);

    Mat wordEdge8UC3 = m_matrix::to8UC3(wordEdge);

    Mat maskImg(mask.size(), CV_8UC3, Scalar(0, 0, 0));
    img.copyTo(maskImg, mask);
    namedWindow("WordEdgeDilatedContoursMask", CV_WINDOW_FREERATIO);
    imshow("WordEdgeDilatedContoursMask", maskImg);*/

    //Mat fftMat = m_processor::fft2(wordEdge8UC3);
    //fftMat = m_matrix::filter(fftMat, filter_fft2);
    //hough(fftMat);
    //float fftAngle = fft2HoughAngle(fftMat);
    //cout << "FFTAngle:" << fftAngle << endl;
    //img = rotateImage(img,-fftAngle);
    //namedWindow("WordEdgeRotated", CV_WINDOW_FREERATIO);
    //imshow("WordEdgeRotated", img);
    hough(result, lineThreshold, minLength, gap, paddingCross);
    cout << "Spend Time:" << get_timestamp() - __t << "ms" << endl;
    waitKey(0);
    return 0;
}
