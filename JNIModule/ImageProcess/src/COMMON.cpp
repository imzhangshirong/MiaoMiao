#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <stdio.h>
#include <string>
#include <cstring>
#include <sys/time.h>
#include "../include/m_image.h"

using namespace std;

namespace COMM_FUNC
{
  long get_timestamp()
  {
    struct timeval start;//s
    struct timeval end;//us

    gettimeofday(&start,NULL);
    return start.tv_sec*1000+start.tv_usec/1000;
  }

  char* to_p_char(string str)
  {
    int copy_l = str.length()+1;
    char* re = (char*)malloc(copy_l*sizeof(char));
    memcpy(re,str.data(),copy_l);
    return re;
  }

  void output_image(string name,m_image_data image_data)
  {
    string path = "./output/";
    path.append(name);
    m_image::write_JPEG(to_p_char(path),image_data,100);
  }

  m_image_data input_image(string name)
  {
    string path = "./input/";
    path.append(name);
    m_image_data jpeg_data = m_image::read_JPEG(to_p_char(path));
    cout<<"Read JPEG "<<path<<" => width:"<<jpeg_data.width<<",height:"<<jpeg_data.height<<endl;
    return jpeg_data;
  }
}
#endif