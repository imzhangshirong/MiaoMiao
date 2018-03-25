#ifndef M_BP_NEURAL_NETWORK_H
#define M_BP_NEURAL_NETWORK_H

#include <vector>
using namespace std;

class m_bp_neural_network
{
public:
  int input_num;
  int net_num;
  int output_num;
  m_matrix* layers;//n
  m_matrix* offsets;//n
  m_matrix* layers_f_result;//n+1
  m_matrix* layers_net_result;//n+1


  m_bp_neural_network(int input_num,initializer_list<int> net_num,int output_num);
  m_matrix gene(m_matrix &input,float (*func)(float));
  m_matrix error(m_matrix &result);
  void clean_result();
  void back_propagation(m_matrix &result,float sigma,float alpha,float (*funcd)(float));
  void print();
  void init();

};

#endif