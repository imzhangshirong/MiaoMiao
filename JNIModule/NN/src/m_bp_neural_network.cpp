#include "../include/m_matrix.h"
#include "../include/m_bp_neural_network.h"
#include <vector>
#include <iostream>

using namespace std;

m_bp_neural_network::m_bp_neural_network(int input_num, initializer_list<int> net_num, int output_num)
{
  
  int net_s = net_num.size();
  this->layers = new m_matrix[net_s+1];
  this->offsets = new m_matrix[net_s+1];
  this->layers_f_result = new m_matrix[net_s+2];
  this->layers_net_result = new m_matrix[net_s+2];

  this->input_num = input_num;
  this->net_num = net_s;
  this->output_num = output_num;
  m_matrix r(input_num, 1);
  
  this->layers_net_result[0]=m_matrix(r);
  this->layers_f_result[0]=m_matrix(r);
  
  const int *nets_c = net_num.begin();
  m_matrix w(*(nets_c), input_num);
  m_matrix b(*(nets_c), 1);
  r = m_matrix(*(nets_c), 1);
  this->layers[0] = w;
  this->offsets[0] =b;
  this->layers_net_result[1]=m_matrix(r);
  this->layers_f_result[1]=m_matrix(r);
  for (int i = 0; i < net_s - 1; i++)
  {
    w = m_matrix(*(nets_c + i + 1), *(nets_c + i));
    b = m_matrix(*(nets_c + i + 1), 1);
    r = m_matrix(*(nets_c + i + 1), 1);
    this->layers[i+1]=w;
    this->offsets[i+1]=b;
    this->layers_f_result[i+2]=r;
    this->layers_net_result[i+2]=m_matrix(r);
    
  }
  w = m_matrix(output_num, *(nets_c+net_s - 1));
  b = m_matrix(output_num, 1);
  r = m_matrix(output_num, 1);
  this->layers[net_s] = w;
  this->offsets[net_s] = b;
  this->layers_f_result[net_s+1]=r;
  this->layers_net_result[net_s+1]=m_matrix(r);
  
}

void m_bp_neural_network::init()
{
  for(int i=0;i<this->net_num+1;i++)
  {
    this->layers[i].random();
  }
  for(int i=0;i<this->net_num+1;i++)
  {
    this->offsets[i].random();
  }
}

void m_bp_neural_network::print()
{
  cout<<"\n/////////////////INPUT/////////////////"<<endl;
  layers_f_result[0].print();
  cout<<"======================================="<<endl;
  for(int i=0;i<this->net_num+1;i++)
  {
    layers[i].print();
    cout<<"--------------------------"<<endl;
    offsets[i].print();
    cout<<"--------------------------OUT"<<endl;
    layers_f_result[i+1].print();
    cout<<"======================================="<<endl;
  }
}

void m_bp_neural_network::clean_result()
{
  for (int i = 0; i < this->net_num+2; i++)
  {
    this->layers_f_result[i].release();
    this->layers_net_result[i].release();
  }
  delete [] this->layers_f_result;
  delete [] this->layers_net_result;
  this->layers_f_result = new m_matrix[this->net_num+2];
  this->layers_net_result = new m_matrix[this->net_num+2];
}

m_matrix m_bp_neural_network::gene(m_matrix &input, float (*func)(float))
{
  m_matrix re(input);
  this->clean_result();
  this->layers_f_result[0] = input; 
  this->layers_net_result[0] = input;
  for (int i = 0; i < this->net_num+1; i++)
  {
    m_matrix m1 = re.mul(layers[i]);
    m_matrix m2 = m1.add(offsets[i]);
    
    this->layers_net_result[i + 1] = m_matrix(m2);    
    if(i<this->net_num)m2.filter(func);
    this->layers_f_result[i + 1] = m2;
    re.release();
    re = m2;
  }

  return re;
}

m_matrix m_bp_neural_network::error(m_matrix &result)
{
  int kn = this->net_num+1;
  m_matrix err(layers_f_result[kn]);
  err.mul(-1);
  m_matrix re = err.add(result);
  return re;
}

void m_bp_neural_network::back_propagation(m_matrix &result,float alpha,float sigma, float (*funcd)(float))
{
  //alpha learnRate
  int kn = this->net_num+1;
  m_matrix* layers = new m_matrix[kn];
  m_matrix* offsets = new m_matrix[kn];

  m_matrix err = this->error(result);
  m_matrix ey = err;
  m_matrix lastResult = this->layers_net_result[kn];//start at 1
  m_matrix lastDF = lastResult;
  m_matrix en = ey;
  m_matrix S(en);
  lastResult.release();
  lastResult = this->layers_f_result[kn-1];//start at 1
  m_matrix lastRT = lastResult.transpose();
  m_matrix ew = lastRT.mul(S);
  ew.mul(alpha);
  m_matrix eo = en;
  eo.mul(alpha);
  layers[kn - 1] = this->layers[kn - 1].add(ew);
  offsets[kn - 1] = this->offsets[kn - 1].add(eo);
  
  for (int k = kn - 2; k >= 0; k--)
  {
    m_matrix wT = this->layers[k+1].transpose();
    m_matrix ey_ = S.mul(wT);
    m_matrix lastResult_ = this->layers_net_result[k+1];//start at 1
    m_matrix lastDF_ = lastResult_;
    lastDF_.filter(funcd);
    m_matrix en_ = ey_.mul_v(lastDF_);
    S.release();
    S = en_;
    
    lastResult_.release();
    lastResult_ = this->layers_f_result[k];//start at 1
    m_matrix lastRT_ = lastResult_.transpose();
    m_matrix ew_ = lastRT_.mul(S);

    ew_.mul(alpha);
    en_.mul(alpha);
    layers[k] = this->layers[k].add(ew_);
    offsets[k] = this->offsets[k].add(en_);
  }
  delete [] this->layers;
  delete [] this->offsets;
  this->layers = layers;
  this->offsets = offsets;
}