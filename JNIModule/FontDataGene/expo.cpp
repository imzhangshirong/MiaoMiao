#include <iostream>
#include <fstream>
#include <string>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <algorithm>
#include <sys/time.h>
#include <iconv.h>  
#include <stdlib.h>  
#include <stdio.h>
#include "CvxText.cpp"

using namespace std;
using namespace cv;


wstring UTF8toUnicode(const string& s)
{
    wstring ws;
    wchar_t wc;
    for( int i = 0;i < s.length(); )
    {
        char c = s[i];
        if ( (c & 0x80) == 0 )
        {
            wc = c;
            ++i;
        }
        else if ( (c & 0xE0) == 0xC0 )
        {
            wc = (s[i] & 0x1F) << 6;
            wc |= (s[i+1] & 0x3F);
            i += 2;
        }
        else if ( (c & 0xF0) == 0xE0 )
        {
            wc = (s[i] & 0xF) << 12;
            wc |= (s[i+1] & 0x3F) << 6;
            wc |= (s[i+2] & 0x3F);
            i += 3;
        }
        else if ( (c & 0xF8) == 0xF0 )
        {
            wc = (s[i] & 0x7) << 18;
            wc |= (s[i+1] & 0x3F) << 12;
            wc |= (s[i+2] & 0x3F) << 6;
            wc |= (s[i+3] & 0x3F);
            i += 4;
        }
        else if ( (c & 0xFC) == 0xF8 )
        {
            wc = (s[i] & 0x3) << 24;
            wc |= (s[i] & 0x3F) << 18;
            wc |= (s[i] & 0x3F) << 12;
            wc |= (s[i] & 0x3F) << 6;
            wc |= (s[i] & 0x3F);
            i += 5;
        }
        else if ( (c & 0xFE) == 0xFC )
        {
            wc = (s[i] & 0x1) << 30;
            wc |= (s[i] & 0x3F) << 24;
            wc |= (s[i] & 0x3F) << 18;
            wc |= (s[i] & 0x3F) << 12;
            wc |= (s[i] & 0x3F) << 6;
            wc |= (s[i] & 0x3F);
            i += 6;
        }
        ws += wc;
    }
    return ws;
}

string UnicodeToUTF8( const wstring& ws )
{
    string s;
    for( int i = 0;i < ws.size(); ++i ) 
    {
        wchar_t wc = ws[i];
        if ( 0 <= wc && wc <= 0x7f )
        {
            s += (char)wc;
        }
        else if ( 0x80 <= wc && wc <= 0x7ff )
        {
            s += ( 0xc0 | (wc >> 6) );
            s += ( 0x80 | (wc & 0x3f) );
        }
        else if ( 0x800 <= wc && wc <= 0xffff )
        {
            s += ( 0xe0 | (wc >> 12) );
            s += ( 0x80 | ((wc >> 6) & 0x3f) );
            s += ( 0x80 | (wc & 0x3f) );
        }
        else if ( 0x10000 <= wc && wc <= 0x1fffff )
        {
            s += ( 0xf0 | (wc >> 18) );
            s += ( 0x80 | ((wc >> 12) & 0x3f) );
            s += ( 0x80 | ((wc >> 6) & 0x3f) );
            s += ( 0x80 | (wc & 0x3f) );
        }
        else if ( 0x200000 <= wc && wc <= 0x3ffffff )
        {
            s += ( 0xf8 | (wc >> 24) );
            s += ( 0x80 | ((wc >> 18) & 0x3f) );
            s += ( 0x80 | ((wc >> 12) & 0x3f) );
            s += ( 0x80 | ((wc >> 6) & 0x3f) );
            s += ( 0x80 | (wc & 0x3f) );
        }
        else if ( 0x4000000 <= wc && wc <= 0x7fffffff )
        {
            s += ( 0xfc | (wc >> 30) );
            s += ( 0x80 | ((wc >> 24) & 0x3f) );
            s += ( 0x80 | ((wc >> 18) & 0x3f) );
            s += ( 0x80 | ((wc >> 12) & 0x3f) );
            s += ( 0x80 | ((wc >> 6) & 0x3f) );
            s += ( 0x80 | (wc & 0x3f) );
        }
    }
    return s;
}

//read data from the file, Word By Word
//when used in this manner, we'll get space-delimited bits of text from the file
//but all of the whitespace that separated words (including newlines) was lost. 
void readDataFromFileWBW(vector<string> &data)
{
    ifstream fin("words.dat");  
    string s;  
    while( fin >> s ) 
    {    
        //cout << "Read from file: " << s << endl;
        data.push_back(s);
    }
}

void saveImg(string file,Mat &img, vector<int> &params){
    const char * c = file.c_str();
    imwrite(c,img,params);
}

int main(int argc, const char **argv){
    vector<string> data;
    readDataFromFileWBW(data);
    cout<<"AllData:"<<data.size()<<endl;
    Size canvasSize = Size(32,32);
    CvScalar fontSize = Scalar(24);
    string imgDir = "img/";
    vector<int> p;
    p.push_back(CV_IMWRITE_PNG_COMPRESSION);
    p.push_back(100);
    //CvxText text("/home/kyarvis/.local/share/fonts/simfang.ttf");
    //CvxText text("/home/kyarvis/.local/share/fonts/fangsong_GB2312.ttf");
    CvxText text("/usr/share/fonts/opentype/noto/NotoSansCJK-Regular.ttc");
    text.setFont(NULL,&fontSize,NULL,NULL);
    for(int i=0;i<data.size();i++){
        wstring str = UTF8toUnicode(data[i]);
        const wchar_t * s = str.c_str();
        Rect2f rect = text.getCharPixelSize(*s);
        float x = canvasSize.width / 2 - rect.width / 2+1;
        float y = canvasSize.height / 2 + rect.height / 2-1;
        Mat canvas(canvasSize,CV_8UC4,Scalar(0,0,0,255));
        text.putText(canvas, s, cvPoint(x,y), Scalar(255,255,255,255));
        string file = imgDir+to_string(i)+".png";
        saveImg(file,canvas,p);
    }
    cout<<"Complete!"<<endl;
    return 0;
}