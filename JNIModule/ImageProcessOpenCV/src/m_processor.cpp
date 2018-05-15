#include "../include/m_matrix.h"
#include "../include/m_line.h"
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

Mat m_processor::getPerspectiveTransformMat(Mat &imgF, int threshold, int minLength, double gap, int paddingCross,vector<Point2f> &pcors,vector<Point2f> &pcorsTrans,Point2f * poffset)
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
        double maxLength = 0;
        for (int j = 0; j < points.size(); j++)
        {
            double dx = points[j].x / 100;
            double dy = points[j].y / 100;
            double length = sqrt(dx * dx + dy * dy)/10;
            if(length>maxLength)maxLength = length;
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
            double dx = points[j].x / 100;
            double dy = points[j].y / 100;
            double length = sqrt(dx * dx + dy * dy)/10;
            areaC[a] += length/maxLength + 1;
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
        n--;
        if (n > 0)
        {
            xv-=vp.x;
            yv-=vp.y;
            xv /= n;
            yv /= n;
            xv = 0.5*xv+0.5*vp.x;
            yv = 0.5*yv+0.5*vp.y;
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

                    double sizeA = m_processor::computePolygonArea(apts);
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
    Mat tMat = Mat::eye(3,3,CV_32F);
    if ((-top) + (-left) + right + bottom < 10000 && zeroP < 2)
    {
        Point2d offset(-left, -top);

        cout << "top:" << top << ",left:" << left << ",bottom:" << bottom << ",right:" << right << endl;
        int padding_width = right - left;
        int padding_height = bottom - top;
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
            Point2f(padding_width, padding_height),
            Point2f(0, padding_height),
            Point2f(0, 0),
            Point2f(padding_width, 0)};
        for(int i=0;i<4;i++){
            pcors.push_back(corSrc[i]);
            pcorsTrans.push_back(corDst[i]);
        }
        poffset->x = offset.x;
        poffset->y = offset.y;
        tMat = getPerspectiveTransform(corSrc, corDst);
        Mat warp;
        cv::warpPerspective(paddingDst, warp, tMat, paddingDst.size(), cv::INTER_LINEAR);
        namedWindow("PerspectiveTransform", CV_WINDOW_FREERATIO);
        imshow("PerspectiveTransform", warp);
    }
    else
    {
        cout << "Do not Perspective Trans" << endl;
        /*namedWindow("Detected Lines", CV_WINDOW_FREERATIO);
        imshow("Detected Lines", color_dst);*/
    }
    return tMat;
}
Mat m_processor::transPrevMat(Mat &src,Mat &mat,Point2f &offset,Size &size){
    Mat paddingDst = Mat(size, src.type(), Scalar(0));
    Mat qArea(paddingDst, Rect(offset.x, offset.y, src.cols, src.rows));
    src.copyTo(qArea);
    warpPerspective(paddingDst, paddingDst, mat, paddingDst.size(), cv::INTER_LINEAR);
    return paddingDst;
}

Mat m_processor::rotateImage(Mat &mat, float degree)
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

float m_processor::fft2HoughAngle(Mat &imgF)
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

double m_processor::computePolygonArea(const vector<Point2d> &points)
{
    int point_num = points.size();
    if (point_num < 3)
        return 0.0;
    double s = points[0].y * (points[point_num - 1].x - points[1].x);
    for (int i = 1; i < point_num; ++i)
        s += points[i].y * (points[i - 1].x - points[(i + 1) % point_num].x);
    return fabs(s / 2.0);
}

Mat m_processor::equalizeHist3C(Mat &mat)
{
    Mat mergeImg;
    vector<Mat> splitBGR(mat.channels());
    split(mat, splitBGR);
    for (int i = 0; i < mat.channels(); i++)
        equalizeHist(splitBGR[i], splitBGR[i]);
    merge(splitBGR, mergeImg);
    return mergeImg;
}

vector<Vec3i> m_processor::getCrestsData(vector<int> &dist){
    int distrtN = dist.size();
    vector<int> distrtDefualt(dist);
    vector<int> distrt(distrtN,0);
    int kernelSizeD = 5;
    int halfSize = kernelSizeD/2;
    for(int k=0;k<5;k++){
        for(int i=0;i<distrtN;i++){
            int all = distrtDefualt[i];
            int allN = 1;
            for(int j=0;j<kernelSizeD;j++){
                int l = i-halfSize+j;
                if(l>=0 && l<distrtN && i!=l){
                    all+=distrt[l];
                    allN++;
                }
            }
            distrt[i]=all/allN;
        }
        distrtDefualt = vector<int>(distrt);
    }
    
    int lastMoutnV = 0;
    int lastMoutnD = 0;
    vector<Vec3i> features;
    for(int i=0;i<distrtN;i++){
        int value = distrt[i];
        int moutD = (value-lastMoutnV);
        if(moutD == 0){ //no
            if(lastMoutnD >0){//crest
                int j = i-1;
                if(j>=0){
                    Vec3i v3i;
                    v3i[0] = j;
                    v3i[1] = distrt[j];
                    v3i[2] = 1;
                    features.push_back(v3i);
                }
            }
            else if(lastMoutnD<0){//trough
                int j = i-1;
                if(j>=0){
                    Vec3i v3i;
                    v3i[0] = j;
                    v3i[1] = distrt[j];
                    v3i[2] = -1;
                    features.push_back(v3i);
                }
            }
            else{

            }
        }
        else if(moutD>0){ //up
            if(lastMoutnD > 0){
                
            }
            else if(lastMoutnD<0){//trough
                int j = i-1;
                if(j>=0){
                    Vec3i v3i;
                    v3i[0] = j;
                    v3i[1] = distrt[j];
                    v3i[2] = -1;
                    features.push_back(v3i);
                }
            }
            else{//trough
                int j = i-1;
                if(j>=0){
                    Vec3i v3i;
                    v3i[0] = j;
                    v3i[1] = distrt[j];
                    v3i[2] = -1;
                    features.push_back(v3i);
                }
            }
        }
        else if(moutD<0){ //down
            if(lastMoutnD > 0){//crest
                int j = i-1;
                if(j>=0){
                    Vec3i v3i;
                    v3i[0] = j;
                    v3i[1] = distrt[j];
                    v3i[2] = 1;
                    features.push_back(v3i);     
                }
            }
            else if(lastMoutnD<0){
            }
            else{//crest

            }
        }
        lastMoutnV = value;
        lastMoutnD = moutD;
    }
    
    for(int i=0;i<features.size()-1;i++){
        Vec3i v3i = features[i];
        Vec3i v3i2 = features[i+1];
        if(v3i[2]<0 && v3i2[2]<0){
            if(v3i[1]>v3i2[1]){
                features.erase(features.begin()+i,features.begin()+i+1);
                i--;
            }
            else if(v3i[1]<v3i2[1])
            {
                features.erase(features.begin()+i+1,features.begin()+i+2);
            }
        }
        if(v3i[2]>0 && v3i2[2]<0){
            if(v3i[1]==v3i2[1])
            {
                features.erase(features.begin()+i,features.begin()+i+2);
                i--;
            }
        }
    }
    Vec3i lastCrest;
    int lastCrestI = -1;
    for(int i=0;i<features.size();i++){
        Vec3i v3i = features[i];
        
        if(v3i[2]>0){
            if(v3i[0]-lastCrest[0]<5 && lastCrest[1]>0){
                if(v3i[1]>lastCrest[1]){
                    features.erase(features.begin()+lastCrestI,features.begin()+i);
                }
                else if(v3i[1]<lastCrest[1])
                {
                    features.erase(features.begin()+lastCrestI+1,features.begin()+i+1);
                    i-=(i-lastCrestI);
                }
            }
            lastCrestI = i;
            lastCrest = features[i];
        }
    }
    return features;
}
