#include "m_image.h"
#include "m_matrix.h"
#ifndef M_EDGE_H
#define M_EDGE_H
class EDGE
{
public:
  static void prewitt_mark(m_image_data image_data,m_image_data image_data_out, m_matrix &map_data);
  static void connected_mark(m_image_data image_data,m_image_data image_data_out);
};
#endif