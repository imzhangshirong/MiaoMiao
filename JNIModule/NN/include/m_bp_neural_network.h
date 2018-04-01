#ifndef M_BP_NEURAL_NETWORK_H
#define M_BP_NEURAL_NETWORK_H

#include <vector>
using namespace std;

class m_bp_neural_network
{
private:
public:
  int input_num;
  int net_num;
  int output_num;
  int sample_num;
  float alpha;
  float lambda;
  float sigma;
  m_matrix* layers;//n
  m_matrix* bases;//n
  m_matrix* delta_layers;//n
  m_matrix* delta_bases;//n
  m_matrix* last_delta_layers;//n
  m_matrix* layers_f_input;//n+1
  m_matrix* layers_net_input;//n+1
  m_matrix errors;


  m_bp_neural_network(int input_num,initializer_list<int> net_num,int output_num,float alpha,float lambda,float sigma);
  m_matrix gene(m_matrix &input,float (*func)(float));
  m_matrix error(m_matrix &result);
  void clean_input();
  void back_propagation(m_matrix &result,float (*funcd)(float));
  void print();
  void init();
  float error_f(m_matrix &result);
  m_matrix cost(m_matrix &result);
  void update();
  m_matrix error();
  void clean_delta();
  m_matrix cross_entropy_cost(m_matrix &result);
};

#endif