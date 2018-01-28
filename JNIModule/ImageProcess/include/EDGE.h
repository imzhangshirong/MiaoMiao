#include "IMG.h"
#include "MATRIX.h"
#ifndef EDGE_H
#define EDGE_H
class EDGE
{
public:
  static void prewitt_mark(IMG_DATA image_data,IMG_DATA image_data_out, MATRIX2 map_data);
  static void connected_mark(IMG_DATA image_data,IMG_DATA image_data_out);
};
#endif