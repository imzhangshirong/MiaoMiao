#ifndef M_BP_NEURAL_NETWORK_H
#define M_BP_NEURAL_NETWORK_H

#include <vector>
#include <armadillo>
using namespace std;
using namespace arma;

class m_bp_neural_network
{
private:
  void sim(mat &input,int index);
  bool stop_train;
public:
  int n_input;
  int n_layers;
  int n_output;
  double step;
  string* layers_func;
  mat* layers;//n
  mat* bases;//n
  mat* delta_layers;//n
  mat* delta_bases;//n
  mat* hessian_layers;//n
  mat* last_delta_layers;//n
  mat* layers_f_input;//n+1
  mat* layers_net_input;//n+1
  mat errors;
  mat mse;
  double mse_v;
  mat sim(mat &input);
  m_bp_neural_network(int input_num, initializer_list<int> net_num,initializer_list<string> net_func);
  void forward(vector<mat> &input, vector<mat> &result);
  void back_propagation(int sample_num);
  void print();
  void init();
  void update(string type,vector<mat> &input, vector<mat> &result);
  void train(string train_func, vector<mat> &input, vector<mat> &result, int max_epoch, double alpha);
};

#endif