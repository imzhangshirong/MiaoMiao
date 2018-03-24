#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../include/m_complex.h"
#include "../include/m_matrix.h"
#include "../include/m_matrixc.h"
#include "../include/m_image.h"
#include "../include/m_domain.h"

#include "../include/m_processor.h"
#include "../include/m_vert2.h"

void m_processor::binary(m_image_data &image_data, m_image_data &image_data_out, int threshold, int range)
{
  for (int i = 0; i < image_data.height; i++)
  {
    for (int j = 0; j < image_data.width; j++)
    {
      unsigned char *rgb = m_image::get_rgb(image_data, j, i);
      if (abs(*rgb - threshold) > range)
      {
        m_image::set_rgb(image_data_out, j, i, m_image_color::BLACK);
      }
      else
      {
        m_image::set_rgb(image_data_out, j, i, m_image_color::WHITE);
      }
    }
  }
}

void m_processor::binary2(m_image_data &image_data, m_image_data &image_data_out, int threshold, int range, int whiteDirect)
{
  for (int i = 0; i < image_data.height; i++)
  {
    for (int j = 0; j < image_data.width; j++)
    {
      unsigned char *rgb = m_image::get_rgb(image_data, j, i);
      int d = *rgb - threshold;
      if (abs(d) > range && d * whiteDirect < 0)
      {
        m_image::set_rgb(image_data_out, j, i, m_image_color::BLACK);
      }
      else
      {
        m_image::set_rgb(image_data_out, j, i, m_image_color::WHITE);
      }
    }
  }
}

void m_processor::gray_scale(m_image_data &image_data, m_image_data &image_data_out)
{
  for (int i = 0; i < image_data.height; i++)
  {
    for (int j = 0; j < image_data.width; j++)
    {
      unsigned char *rgb = m_image::get_rgb(image_data, j, i);
      unsigned char gray = (*rgb + *(rgb + 1) + *(rgb + 2)) / 3;
      m_image::set_rgb(image_data_out, j, i, gray, gray, gray);
    }
  }
}

m_matrix m_processor::gray2matrix(m_image_data &image_data, int *grayMap, int *averageGray)
{
  m_matrix re(image_data.width, image_data.height);
  int grayAverage = 0;
  for (int i = 0; i < image_data.height; i++)
  {
    for (int j = 0; j < image_data.width; j++)
    {
      unsigned char gray = *(m_image::get_rgb(image_data, j, i));
      re.set(j, i, gray);
      grayAverage += gray;
    }
  }
  grayAverage /= (image_data.height * image_data.width);
  *averageGray = grayAverage;
  return re;
}

void block_binary(m_image_data &image_data, int block_width, int block_height, bool (*bFunc)(int))
{
  int components = image_data.components;
  int x = 0;
  int y = 0;
  while (x >= 0 && y >= 0)
  {
    int copyWidth = block_width;
    int copyHeight = block_height;
    if (x * block_width >= image_data.width)
    {
      x = 0;
      y++;
    }
    if (y * block_height >= image_data.height)
      break;
    int start_p_x = x * block_width;
    int start_p_y = y * block_height;
    if (start_p_x + copyWidth > image_data.width)
      copyWidth = image_data.width - start_p_x;
    if (start_p_y + copyHeight > image_data.height)
      copyHeight = image_data.height - start_p_y;
    for (int i = 0; i < copyHeight; i++)
    {
      for (int j = 0; j < copyWidth; j++)
      {
        int p_x = start_p_x + j;
        int p_y = start_p_y + i;
        unsigned char *rgb = m_image::get_rgb(image_data, p_x, p_y);
        if ((*bFunc)(*rgb))
        {
          m_image::set_rgb(image_data, p_x, p_y, m_image_color::BLACK);
        }
        else
        {
          m_image::set_rgb(image_data, p_x, p_y, m_image_color::WHITE);
        }
      }
    }
    x++;
  }
}

void m_processor::draw_matrix(m_matrix &matrix, m_image_data &image_data_out, unsigned char *(*func)(int x, int y, float value))
{
  for (int i = 0; i < matrix.height; i++)
  {
    for (int j = 0; j < matrix.width; j++)
    {
      unsigned char *color = func(j, i, matrix.get(j, i));
      m_image::set_rgb(image_data_out, j, i, color);
    }
  }
}

void m_processor::draw_verts(m_matrix &matrix, m_image_data &image_data_out, void (*func)(m_image_data image_data, int x, int y, float value))
{
  for (int i = 0; i < matrix.height; i++)
  {
    for (int j = 0; j < matrix.width; j++)
    {
      func(image_data_out, j, i, matrix.get(j, i));
    }
  }
}

m_matrix m_processor::draw_vert_mark(m_matrix &matrix, int block_size, m_vert2 *verts_d, float *verts_d_r, int verts_d_length)
{
  m_matrix re(matrix.width, matrix.height);
  int size = block_size;
  int valid = size * size / 2;
  int half = size / 2;
  int x = 0, y = 0, dx = 0, dy = 0;
  int v_size = size * size;
  for (int i = 0; i < matrix.height; i++)
  {
    for (int j = 0; j < matrix.width; j++)
    {
      m_vert2 verts[v_size];
      int v_n = 0;
      for (int k = 0; k < size; k++)
      {
        for (int l = 0; l < size; l++)
        {
          dx = k - half;
          dy = l - half;
          x = j + dx;
          y = i + dy;
          if (x >= 0 && x < matrix.width && y >= 0 && y < matrix.height)
          {
            float value = matrix.get(x, y);
            if (value > 0)
            {
              verts[v_n] = m_vert2(dx, dy);
              v_n++;
            }
          }
        }
      }
      if (v_n < valid)
        continue;
      m_vert2 vert_c(0.0f, 0.0f);
      for (int k = 0; k < v_n; k++)
      {
        vert_c = vert_c.add(verts[k]);
      }
      vert_c = vert_c.multiply(1.0f / v_n);
      for (int k = 0; k < v_n; k++)
      {
        verts[k] = verts[k].sub(vert_c);
      }
      float min_d = FLT_MAX;
      int min_index = -1;
      for (int k = 0; k < verts_d_length; k++)
      {
        m_vert2 base_vert = verts_d[k];
        float cur_d = 0.0f;
        for (int l = 0; l < v_n; l++)
        {
          cur_d += verts[l].dot(base_vert);
        }
        if (cur_d < min_d)
        {
          min_d = cur_d;
          min_index = k;
        }
      }
      re.set(j, i, verts_d_r[min_index]);
    }
  }
  return re;
}

//FFT2///////////////////////////////////////////////////////////
m_complex _W_u_L(float u, float L)
{
  static float pi2 = -M_PI * 2;
  float W_uL_d = pi2 * (float)u / (float)L;
  m_complex W_uL(cosf(W_uL_d), sinf(W_uL_d));
  return W_uL;
}

m_complex _fft2(float *sum_d, int u, int L)
{
  int n = L;
  int n_r = n / 2;
  m_complex W_uL = _W_u_L((float)u, (float)L);
  m_complex F1;
  m_complex F2;
  if (n_r > 1)
  {
    m_complex fb(0.0f, 0.0f);
    float rd1[n_r];
    float rd2[n_r];
    for (int r = 0; r < n_r; r++)
    {
      rd1[r] = sum_d[2 * r];
      rd2[r] = sum_d[2 * r + 1];
    }
    F1 = _fft2(rd1, u, n_r);
    F2 = _fft2(rd2, u, n_r);
  }
  else
  {
    //r=0;
    F1 = m_complex(sum_d[0], 0.0f);
    F2 = m_complex(sum_d[1], 0.0f);
  }
  F2.mul(W_uL);
  F1.add(F2);
  return F1;
}

m_complex _fft2(m_complex *sum_d, int u, int L)
{
  int n = L;
  int n_r = n / 2;
  m_complex W_uL = _W_u_L((float)u, (float)L);
  m_complex F1;
  m_complex F2;
  if (n_r > 1)
  {
    m_complex fb(0.0f, 0.0f);
    m_complex rd1[n_r];
    m_complex rd2[n_r];
    for (int r = 0; r < n_r; r++)
    {
      rd1[r] = sum_d[2 * r];
      rd2[r] = sum_d[2 * r + 1];
    }
    F1 = _fft2(rd1, u, n_r);
    F2 = _fft2(rd2, u, n_r);
  }
  else
  {
    //r=0;
    F1 = m_complex(1.0f, 0.0f);
    F2 = m_complex(1.0f, 0.0f);
    F1.mul(sum_d[0]);
    F2.mul(sum_d[1]);
  }
  F2.mul(W_uL);
  F1.add(F2);
  return F1;
}

void _fftA(m_matrixc &rec, m_matrix &matrix)
{
  for (int m = 0; m < rec.height; m++)
  {
    float line[matrix.width];
    for (int i = 0; i < matrix.width; i++)
    {
      line[i] = matrix.get(i, m);
    }
    for (int k = 0; k < rec.width; k++)
    {
      m_complex fb = _fft2(line, k, matrix.width);
      rec.set(k, m, fb);
    }
  }
}

void _fftB(m_matrixc &rec, m_matrixc &matrix)
{
  for (int m = 0; m < rec.width; m++)
  {
    m_complex line[matrix.height];
    for (int i = 0; i < matrix.height; i++)
    {
      line[i] = matrix.get(m, i);
    }
    for (int k = 0; k < rec.height; k++)
    {
      m_complex fb = _fft2(line, k, matrix.height);
      rec.set(m, k, fb);
    }
  }
}

m_matrix m_processor::fft2(m_matrix &matrix, int size)
{
  m_matrixc recA(size, size);
  _fftA(recA, matrix);
  m_matrixc recB(size, size);
  _fftB(recB, recA);
  m_matrixc rec(size, size);
  int half_x = size / 2;
  int half_y = size / 2;

  for (int l = 0; l < size; l++)
  {
    for (int k = 0; k < size; k++)
    {
      m_complex fb = recB.get(k, l);
      if (l < half_y && k < half_x)
      {
        rec.set(k + half_x, l + half_y, fb);
      }
      else if (l < half_y && k >= half_x)
      {
        rec.set(k - half_x, l + half_y, fb);
      }
      else if (l >= half_y && k >= half_x)
      {
        rec.set(k - half_x, l - half_y, fb);
      }
      else if (l >= half_y && k < half_x)
      {
        rec.set(k + half_x, l - half_y, fb);
      }
    }
  }
  return rec.to_float();
}
//DFT///////////////////////////////////////////////////////////
m_matrix m_processor::dft2(m_matrix &matrix, int size)
{
  m_matrixc rec(size, size);
  int half_x = size / 2;
  int half_y = size / 2;
  float pi2 = -M_PI * 2;

  for (int l = 0; l < rec.height; l++)
  {
    for (int k = 0; k < rec.width; k++)
    {
      m_complex fb(0.0f, 0.0f);
      for (int j = 0; j < matrix.height; j++)
      {
        for (int i = 0; i < matrix.width; i++)
        {
          float v = matrix.get(i, j);
          if (v != 0.0f)
          {
            float dd = pi2 * ((float)(k * i) / (float)matrix.width + (float)(l * j) / (float)matrix.height);
            m_complex fbt(cosf(dd) * v, sinf(dd) * v);
            fb.add(fbt);
          }
        }
      }
      if (l < half_y && k < half_x)
      {
        rec.set(k + half_x, l + half_y, fb);
      }
      else if (l < half_y && k >= half_x)
      {
        rec.set(k - half_x, l + half_y, fb);
      }
      else if (l >= half_y && k >= half_x)
      {
        rec.set(k - half_x, l - half_y, fb);
      }
      else if (l >= half_y && k < half_x)
      {
        rec.set(k + half_x, l - half_y, fb);
      }
    }
  }
  return rec.to_float();
}

float houghLineTranser(float x, float y, float thelta)
{
  return y * sinf(thelta) + x * cosf(thelta);
}

m_matrix m_processor::ht_line(m_matrix &matrix, int theltaStep, int lengthStep)
{
  float max_length = sqrtf(matrix.height * matrix.height + matrix.width * matrix.width);
  float max_thelta = M_PI;
  float dl = max_length / lengthStep;
  float dt = max_thelta / theltaStep;
  m_matrix re(theltaStep, lengthStep);
  for (int i = 0; i < matrix.height; i++)
  {
    for (int j = 0; j < matrix.width; j++)
    {
      float value = matrix.get(j, i);
      if (value > 0)
      {
        float thelta = 0;
        for (int k = 0; k < theltaStep; k++)
        {
          float r = houghLineTranser(j, i, thelta);
          if (r > dl)
          {
            int indexR = (r / dl);
            //if(abs(indexR)<lengthStep)
            {
              //if(indexR<0)indexR+=lengthStep;
              re.set(k, indexR, re.get(k, indexR) + 10.0f * value / 255);
            }
          }

          thelta += dt;
        }
      }
    }
  }
  return re;
}

vector<m_domain> m_processor::bwlabel(m_matrix &matrix, float threshold)
{
  m_matrix labelM(matrix.width, matrix.height);
  int label = 1;
  for (int i = 0; i < matrix.height; i++)
  {
    float last = matrix.get(0, i);
    for (int j = 0; j < matrix.width; j++)
    {
      float value = matrix.get(j, i);
      if (i == 0)
      {
        if ((abs(last - value) > threshold))
        {
          label++;
        }
        labelM.set(j, i, label);
      }
      else
      {
        float v1 = -1;
        float v2 = matrix.get(j, i - 1);
        float v3 = -1;
        if (j - 1 >=0)
          v1 = matrix.get(j - 1, i - 1);
        if (j + 1 < matrix.width)
          v3 = matrix.get(j + 1, i - 1);
        int labeln = -1;
        if (v1>=0 && abs(v1 - value) <= threshold)
        {
          int t = labelM.get(j-1, i - 1);
          if(labeln<0 || t<labeln)labeln = t;
        }
        if (v2>=0 && abs(v2 - value) <= threshold)
        {
          int t = labelM.get(j, i - 1);
          if(labeln<0 || t<labeln)labeln = t;
        }
        if (v3>=0 && abs(v3 - value) <= threshold)
        {
          int t = labelM.get(j+1, i - 1);
          if(labeln<0 || t<labeln)labeln = t;
        }
        if (j>0 && (abs(last - value) <= threshold))
        {
          int t = labelM.get(j-1, i);
          if(labeln<0 || t<labeln)labeln = t;
        }
        if(labeln<0){
          label++;
          labeln = label;
        }
        labelM.set(j, i, labeln);
      }
      last = value;
    }
  }
  m_domain *labels = new m_domain[label];
  for (int i = matrix.height-2; i >=0; i--)
  {
    float last = matrix.get(0, i);
    for (int j = matrix.width-1; j >=0; j--)
    {
      float value = matrix.get(j, i);
      {
        float v1 = -1;
        float v2 = matrix.get(j, i + 1);
        float v3 = -1;
        if (j - 1 >=0)
          v1 = matrix.get(j - 1, i + 1);
        if (j + 1 < matrix.width)
          v3 = matrix.get(j + 1, i + 1);
        int labeln = -1;
        if (v1>=0 && abs(v1 - value) <= threshold)
        {
          int t = labelM.get(j-1, i +1);
          if(labeln<0 || t<labeln)labeln = t;
        }
        if (v2>=0 && abs(v2 - value) <= threshold)
        {
          int t = labelM.get(j, i + 1);
          if(labeln<0 || t<labeln)labeln = t;
        }
        if (v3>=0 && abs(v3 - value) <= threshold)
        {
          int t = labelM.get(j+1, i + 1);
          if(labeln<0 || t<labeln)labeln = t;
        }
        int old = labelM.get(j, i);
        if(labeln>0 && labeln<old){
          
          
          labels[old-1].flag=-1;
          labelM.set(j, i, labeln);
        }
        
      }
      last = value;
    }
  }
  vector<m_domain> domains;
  //labelM.print();
  for (int i = 0; i < matrix.height; i++)
  {
    for (int j = 0; j < matrix.width; j++)
    {
      int labelc = (int)labelM.get(j,i);
      m_matrix_index index;
      index.x = j;
      index.y = i;
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

void m_processor::draw_labeled_domain(vector<m_domain> &domains,m_image_data &image_data_out)
{
  vector<unsigned char*> colors;
  colors.push_back(m_image_color::WHITE);
  colors.push_back(m_image_color::CYAN);
  colors.push_back(m_image_color::BLACK);
  colors.push_back(m_image_color::MEGENTA);
  colors.push_back(m_image_color::RED);
  int cid = 0;
  for(int i=0;i<domains.size();i++)
  {
    unsigned char* color = colors[cid];
    for(int j=0;j<domains[i].indeces.size();j++)
    {
      
      m_matrix_index index = domains[i].indeces[j];
      m_image::set_rgb(image_data_out,index.x,index.y,color);
    }
    cid++;
    cid%=colors.size();
  }
}
