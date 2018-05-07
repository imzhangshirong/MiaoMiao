// CvxText.h
#pragma once

#ifndef OPENCV_CVX_TEXT_2007_08_31_H
#define OPENCV_CVX_TEXT_2007_08_31_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

class CvxText
{
    CvxText &operator=(const CvxText &);

    //================================================================
    //================================================================

  public:
    CvxText(const char *freeType);
    virtual ~CvxText();

    void getFont(int *type,
                 CvScalar *size = NULL, bool *underline = NULL, float *diaphaneity = NULL);

    void setFont(int *type,
                 CvScalar *size = NULL, bool *underline = NULL, float *diaphaneity = NULL);

    void restoreFont();

    //================================================================

    int putText(Mat &img, const char *text, CvPoint pos);

    int putText(Mat &img, const wchar_t *text, CvPoint pos);

    int putText(Mat &img, const char *text, CvPoint pos, CvScalar color);

    int putText(Mat &img, const wchar_t *text, CvPoint pos, CvScalar color);
    Rect2f getCharPixelSize(wchar_t c);
  private:
    void putWChar(Mat &img, wchar_t wc, CvPoint &pos, CvScalar color);

  private:
    FT_Library m_library;
    FT_Face m_face;

    int m_fontType;
    CvScalar m_fontSize;
    bool m_fontUnderline;
    float m_fontDiaphaneity;

    //================================================================
    //================================================================
};

#endif // OPENCV_CVX_TEXT_2007_08_31_H
