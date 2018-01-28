#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <math.h>
/*
 * <setjmp.h> is used for the optional error recovery mechanism shown in
 * the second part of the example.
 */
#include <setjmp.h>
extern "C" {
  #include "jpeglib.h"
  #include "jerror.h"
}

#include "../include/IMG.h"
IMG_DATA IMG::create(int width, int height)
{
  IMG_DATA re;
  re.width = width;
  re.height = height;
  re.components = 3;
  re.data = (unsigned char*)malloc(sizeof(char)*width*height*re.components);
  IMG::clear(re,IMG_COLOR::BLACK);
  return re;
}

void IMG::clear(IMG_DATA image_data, unsigned char* color)
{
  int n = image_data.width*image_data.height;
  int size = image_data.components*sizeof(char);
  unsigned char* s = image_data.data;
  for(int i=0;i<n;i++)
  {
    memcpy(s,color,size);
    s+=size;
  }
}

void IMG::draw_rect(IMG_DATA image_data,IMG_RECT rect,unsigned char * rgb){
  IMG::draw_line(image_data,rect.left,rect.top,rect.left,rect.bottom,rgb);
  IMG::draw_line(image_data,rect.right,rect.top,rect.right,rect.bottom,rgb);
  IMG::draw_line(image_data,rect.left,rect.top,rect.right,rect.top,rgb);
  IMG::draw_line(image_data,rect.right,rect.bottom,rect.left,rect.bottom,rgb);
}

void IMG::fill_rect(IMG_DATA image_data,IMG_RECT rect,unsigned char * rgb)
{
  int line = image_data.width*image_data.components*sizeof(char);
  int cur_p = rect.top*line + rect.left*image_data.components*sizeof(char);
  int size = sizeof(char)*image_data.components;
  int width=rect.right-rect.left;
  int height=rect.bottom-rect.top;
  for(int i=0;i<height;i++)
  {
    for(int j=0;j<width;j++)
    {
      memcpy(image_data.data+cur_p+j*size,rgb,size);
    }
    cur_p+=line;
  }
}


void IMG::draw_line(IMG_DATA image_data,int x1,int y1,int x2,int y2,unsigned char * rgb){
  int d = x2-x1;
  if(d!=0){
    int x,y;
    float k = (float)(y2-y1)/d;
    int m = abs((int)k)+1;
    int length = abs(d)*m;
    for(int i=0;i<length;i++){
      if(d>0){
        x=x1+(float)i/m;
        y=y1+k*i/m;
      }
      else{
        x=x1-(float)i/m;
        y=y1-k*i/m;
      }
      if(x>=0 && x<image_data.width && y>=0 && y<image_data.height){
        IMG::set_rgb(image_data,x,y,rgb);
      }
    }
  }
  else{
    d = y2-y1;
    if(d!=0){
      int x,y;
      float k = (float)(x2-x1)/d;
      int m = abs((int)k)+1;
      int length = abs(d)*m;
      for(int i=0;i<length;i++){
        if(d>0){
          y=y1+(float)i/m;
          x=x1+k*i/m;
        }
        else{
          y=y1-(float)i/m;
          x=x1-k*i/m;
        }
        if(x>=0 && x<image_data.width && y>=0 && y<image_data.height){
          IMG::set_rgb(image_data,x,y,rgb);
        }
      }
    }
    else{
      IMG::set_rgb(image_data,x1,y1,rgb);
    }
  }
  
}

IMG_DATA IMG::copy_data(IMG_DATA image_data)
{
  IMG_DATA new_data;
  new_data.width = image_data.width;
  new_data.height = image_data.height;
  new_data.components = image_data.components;
  int size = image_data.width*image_data.height*image_data.components;
  new_data.data = (unsigned char*)malloc(size*sizeof(char));
  memcpy(new_data.data,image_data.data,size);
  return new_data;
}

unsigned char * IMG::get_rgb(IMG_DATA image_data,int x,int y)
{
  //unsigned char * rgb = (unsigned char*)malloc(sizeof(char) * image_data.components);
  int line_length = image_data.width*image_data.components;
  unsigned char * rw = image_data.data+y*line_length+x*image_data.components;
  //memcpy(rgb,rw,image_data.components);
  return rw;
}

void IMG::set_rgb(IMG_DATA image_data,int x,int y,unsigned char * rgb)
{
  int line_length = image_data.width*image_data.components;
  unsigned char * rw = image_data.data+y*line_length+x*image_data.components;
  memcpy(rw,rgb,image_data.components);
}

void IMG::set_rgb(IMG_DATA image_data,int x,int y,unsigned char r,unsigned char g,unsigned char b)
{
  int line_length = image_data.width*image_data.components;
  unsigned char * rw = image_data.data+y*line_length+x*image_data.components;
  *rw=r;
  *(rw+1)=g;
  *(rw+2)=b;
}

// JPEG IMAGE WR

void IMG::write_JPEG (char * filename, IMG_DATA image_data, int quality)
{
  unsigned char * rgb_data = image_data.data;
  jpeg_compress_struct cinfo;
  jpeg_error_mgr jerr;
  /* More stuff */
  FILE * outfile;		/* target file */
  JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
  int row_stride;		/* physical row width in image buffer */

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);
  if ((outfile = fopen(filename, "wb")) == NULL) {
    fprintf(stderr, "can't open %s\n", filename);
    exit(1);
  }
  jpeg_stdio_dest(&cinfo, outfile);
  cinfo.image_width = image_data.width; 	/* image width and height, in pixels */
  cinfo.image_height = image_data.height;
  cinfo.input_components = 3;		/* # of color components per pixel */
  cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */
  jpeg_set_defaults(&cinfo);
  jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-jpeg values */);
  jpeg_start_compress(&cinfo, TRUE);
  row_stride = image_data.width * cinfo.input_components;	/* JSAMPLEs per row in image_buffer */
  unsigned char * rgb_data_r=rgb_data;
  while (cinfo.next_scanline < cinfo.image_height) {
    row_pointer[0] = rgb_data_r;
    (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
    rgb_data_r+=row_stride;
  }
  jpeg_finish_compress(&cinfo);
  fclose(outfile);
  jpeg_destroy_compress(&cinfo);
}


struct my_error_mgr {
  jpeg_error_mgr pub;	/* "public" fields */
  jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;


METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message) (cinfo);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}


IMG_DATA IMG::read_JPEG (char * filename)
{
  jpeg_decompress_struct cinfo;
  my_error_mgr jerr;
  /* More stuff */
  IMG_DATA re;
  FILE * infile;		/* source file */
  JSAMPARRAY buffer;		/* Output row buffer */
  int row_stride;		/* physical row width in output buffer */
  unsigned char * rgb_buffer = NULL;

  if ((infile = fopen(filename, "rb")) == NULL) {
    fprintf(stderr, "can't open %s\n", filename);
    return re;
  }
  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;
  if (setjmp(jerr.setjmp_buffer)) {
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
    return re;
  }
  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, infile);
  (void) jpeg_read_header(&cinfo, TRUE);
  (void) jpeg_start_decompress(&cinfo);
  row_stride = cinfo.output_width * cinfo.output_components;
  int rgb_size = row_stride * cinfo.output_height;
  rgb_buffer = (unsigned char*)malloc(sizeof(char) * rgb_size);
  buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
  unsigned char * rgb_buffer_r=rgb_buffer;
  while (cinfo.output_scanline < cinfo.output_height) {
    (void) jpeg_read_scanlines(&cinfo, buffer, 1);
    // 复制到内存  
    memcpy(rgb_buffer_r, buffer[0], row_stride);  
    rgb_buffer_r += row_stride;
  }
  (void) jpeg_finish_decompress(&cinfo);
  /* This is an important step since it will release a good deal of memory. */
  jpeg_destroy_decompress(&cinfo);
  fclose(infile);
  re.width = cinfo.output_width;
  re.height = cinfo.output_height;
  re.components = cinfo.output_components;
  re.data = rgb_buffer;
  return re;
}

// BMP IMAGE WR


/*定义WORD为两个字节的类型*/
typedef unsigned short int WORD;
/*定义DWORD为e四个字节的类型*/
typedef unsigned int DWORD;
/*位图文件头*/
#pragma pack(2)//两字节对齐，否则bmp_fileheader会占16Byte
struct BMP_FILE_HEADER
{
  WORD bType; /* 文件标识符 */
  DWORD bSize; /* 文件的大小 */
  WORD bReserved1; /* 保留值,必须设置为0 */ 
  WORD bReserved2; /* 保留值,必须设置为0 */
  DWORD bOffset; /* 文件头的最后到图像数据位开始的偏移量 */
};
/*位图信息头*/
struct BMP_INFO_HEADER
{
  DWORD bInfoSize; /* 信息头的大小 */
  DWORD bWidth; /* 图像的宽度 */
  DWORD bHeight; /* 图像的高度 */
  WORD bPlanes; /* 图像的位面数 */
  WORD bBitCount; /* 每个像素的位数 */
  DWORD bCompression; /* 压缩类型 */
  DWORD bmpImageSize; /* 图像的大小,以字节为单位 */
  DWORD bXPelsPerMeter; /* 水平分辨率 */
  DWORD bYPelsPerMeter; /* 垂直分辨率 */
  DWORD bClrUsed; /* 使用的色彩数 */
  DWORD bClrImportant; /* 重要的颜色数 */
};

IMG_DATA IMG::read_BMP(char * filename){
  IMG_DATA re;
  BMP_FILE_HEADER head;
  BMP_INFO_HEADER head_info;
  FILE * infile;		/* source file */
  int row_stride;		/* physical row width in output buffer */
  unsigned char * rgb_buffer = NULL;
  if ((infile = fopen(filename, "rb")) == NULL) {
    fprintf(stderr, "can't open %s\n", filename);
    return re;
  }
  fseek(infile,0,SEEK_SET);
  fread(&head, sizeof(BMP_FILE_HEADER), 1,infile);
  fread(&head_info, sizeof(BMP_INFO_HEADER), 1,infile);
  int bmpWidth = head_info.bWidth;
  int bmpHeight = head_info.bHeight;
  int biBitCount = head_info.bBitCount;
  if(biBitCount==24){
    int size=bmpWidth*bmpHeight;
    re.width=bmpWidth;
    re.height=bmpHeight;
    re.components=3;
    int bmpLineSize = ((((bmpWidth * biBitCount) + 31) & ~31) / 8);
    re.data=(unsigned char *)malloc(sizeof(char)*size*3);
    unsigned char * r_ = re.data;
    unsigned char * bgr =(unsigned char *)malloc(sizeof(char)*3);
    
    for(int i=0;i<bmpHeight;i++){
      fseek(infile,-bmpLineSize*i,SEEK_END);
      for(int j=0;j<bmpWidth;j++){
        fread(bgr,sizeof(char)*3,1,infile);
        *(r_)=*(bgr+2);
        *(r_+1)=*(bgr+1);
        *(r_+2)=*(bgr);
        r_+=3;
      }
    }
    free(bgr);
  }
  else{
    fprintf(stderr, "%s is not 24 BMP\n", filename);
    fclose(infile);
    return re;
  }
  fclose(infile);
  return re;  
}

IMG_DATA_CLUSTER IMG::slice_block(IMG_DATA image_data,int size_width,int size_height){
  IMG_DATA_CLUSTER cluster;
  cluster.width = ceil((float)image_data.width/(float)size_width);
  cluster.height = ceil((float)image_data.height/(float)size_height);
  cluster.length = cluster.width*cluster.height;
  cluster.blocks = (IMG_DATA_BLOCK*)malloc(sizeof(IMG_DATA_BLOCK)*cluster.length);
  int components = image_data.components;
  int x = 0;
  int y = 0;
  while(x>=0 && y>=0){
    int copyWidth = size_width;
    int copyHeight = size_height;
    if(x*size_width >= image_data.width){
      x=0;
      y++;
    }
    if(y*size_height >= image_data.height)break;
    int start_p_x = x*size_width;
    int start_p_y = y*size_height;
    if(start_p_x+copyWidth>image_data.width)copyWidth = image_data.width-start_p_x;
    if(start_p_y+copyHeight>image_data.height)copyHeight = image_data.height-start_p_y;
    int start_copy_length = (image_data.width*start_p_y+start_p_x)*image_data.components*sizeof(char);
    int copy_row_length = components*copyWidth*sizeof(char);
    unsigned char* data_block =(unsigned char *)malloc(copy_row_length*copyHeight);
    int cur_copyTo = 0;
    for(int i=0;i<copyHeight;i++){
      memcpy(data_block+cur_copyTo,image_data.data+start_copy_length,copy_row_length);
      start_copy_length+=image_data.width*image_data.components*sizeof(char);
      cur_copyTo+=copy_row_length;
    }
    IMG_DATA_BLOCK item;
    IMG_DATA itemData;
    itemData.width = copyWidth;
    itemData.height = copyHeight;
    itemData.components = components;
    itemData.data = data_block;
    item.index = cluster.width*y+x;
    item.x = x;
    item.y = y;
    item.data = itemData;
    *(cluster.blocks+item.index)=item;
    x++;
  }
  return cluster;
}