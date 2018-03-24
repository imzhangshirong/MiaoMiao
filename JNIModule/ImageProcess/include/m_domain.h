#ifndef M_DOMAIN_H
#define M_DOMAIN_H

#include <vector>
#include "m_matrix.h"

using namespace std;

struct m_domain
{
  int flag;
  vector<m_matrix_index> indeces;
};

#endif