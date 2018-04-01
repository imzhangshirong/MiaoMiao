#include "../include/m_matrix.h"
#include "../include/m_bp_neural_network.h"
#include <vector>
#include <iostream>
#include <math.h>

using namespace std;

m_bp_neural_network::m_bp_neural_network(int input_num, initializer_list<int> net_num, int output_num,float alpha,float lambda,float sigma)
{
  this->alpha = alpha;
  this->lambda = lambda;
  this->sigma = sigma;
  int net_s = net_num.size();
  this->layers = new m_matrix[net_s+1];
  this->bases = new m_matrix[net_s+1];
  
  this->layers_f_input = new m_matrix[net_s+2];
  this->layers_net_input = new m_matrix[net_s+2];

  this->input_num = input_num;
  this->net_num = net_s;
  this->output_num = output_num;
  m_matrix r(input_num, 1);
  
  this->layers_net_input[0]=m_matrix(r);
  this->layers_f_input[0]=m_matrix(r);
  
  const int *nets_c = net_num.begin();
  m_matrix w(*(nets_c), input_num);
  m_matrix b(*(nets_c), 1);
  r = m_matrix(*(nets_c), 1);
  this->layers[0] = w;
  this->bases[0] =b;
  this->layers_net_input[1]=m_matrix(r);
  this->layers_f_input[1]=m_matrix(r);
  for (int i = 0; i < net_s - 1; i++)
  {
    w = m_matrix(*(nets_c + i + 1), *(nets_c + i));
    b = m_matrix(*(nets_c + i + 1), 1);
    r = m_matrix(*(nets_c + i + 1), 1);
    this->layers[i+1]=w;
    this->bases[i+1]=b;
    this->layers_f_input[i+2]=r;
    this->layers_net_input[i+2]=m_matrix(r);
    
  }
  w = m_matrix(output_num, *(nets_c+net_s - 1));
  b = m_matrix(output_num, 1);
  r = m_matrix(output_num, 1);
  this->layers[net_s] = w;
  this->bases[net_s] = b;
  this->layers_f_input[net_s+1]=r;
  this->layers_net_input[net_s+1]=r;
  this->errors = r;
  this->sample_num = 0;

  this->delta_layers = new m_matrix[net_s+1];
  this->last_delta_layers = new m_matrix[net_s+1];
  this->delta_bases = new m_matrix[net_s+1];
  for(int i=0;i<net_s+1;i++)
  {
    this->delta_layers[i] = m_matrix(this->layers[i].width, this->layers[i].height);
    this->last_delta_layers[i] = m_matrix(this->layers[i].width, this->layers[i].height);
    this->delta_bases[i] = m_matrix(this->bases[i].width, this->bases[i].height);
  }
}

float sub1(float x)
{
  return (x-0.5f)*2;
}

void m_bp_neural_network::init()
{
  for(int i=0;i<this->net_num+1;i++)
  {
    this->layers[i].random();
    //this->layers[i].filter(sub1);
    //this->layers[i].mul(1.0f/100);
  }
  for(int i=0;i<this->net_num+1;i++)
  {
    this->bases[i].random();
    //this->bases[i].filter(sub1);
    //this->bases[i].mul(1.0f/100);
  }
}

void m_bp_neural_network::print()
{
  cout<<"\n/////////////////INPUT/////////////////"<<endl;
  layers_f_input[0].print();
  cout<<"======================================="<<endl;
  for(int i=0;i<this->net_num+1;i++)
  {
    layers[i].print();
    cout<<"--------------------------"<<endl;
    bases[i].print();
    cout<<"--------------------------OUT"<<endl;
    layers_net_input[i+1].print();
    cout<<"--------------------------Func OUT"<<endl;
    layers_f_input[i+1].print();
    cout<<"======================================="<<endl;
  }
}

void m_bp_neural_network::clean_input()
{
  for (int i = 0; i < this->net_num+2; i++)
  {
    this->layers_f_input[i].mul(0);
    this->layers_net_input[i].mul(0);
  }
}

m_matrix m_bp_neural_network::gene(m_matrix &input, float (*func)(float))
{
  m_matrix re(input);
  
  this->clean_input();
  this->layers_f_input[0] = input; 
  this->layers_net_input[0] = input;
  for (int i = 0; i < this->net_num+1; i++)
  {
    m_matrix m1 = this->layers_f_input[i].mul(this->layers[i]);
    m_matrix m2 = m1.add(this->bases[i]);
    this->layers_net_input[i + 1] = m_matrix(m2);
    //if(i<this->net_num)
    m2.filter(func);
    this->layers_f_input[i + 1] = m2;
    re.release();
    re = m2;
    
  }

  return re;
}

float powf2(float x)
{
  return powf(x,2);
}

m_matrix m_bp_neural_network::cost(m_matrix &result)
{
  m_matrix cost = this->error(result);
  cost.filter(powf2);
  cost.mul(0.5f);
  return cost;
}

float m_bp_neural_network::error_f(m_matrix &result)
{
  int kn = this->net_num+1;
  m_matrix re = this->error(result);
  float err_f=0;
  for(int i=0;i<this->output_num;i++)
  {
    err_f+=powf(re.get(i,0),2);
  }
  return err_f/this->output_num;
}

m_matrix m_bp_neural_network::error(m_matrix &result)
{
  int kn = this->net_num+1;
  m_matrix err(this->layers_f_input[kn]);
  err.mul(-1);
  m_matrix re = err.add(result);
  return re;
}

m_matrix m_bp_neural_network::error()
{
  if(this->sample_num==0)return m_matrix(0,0);
  m_matrix all_error = this->errors;
  all_error.mul(1.0f/this->sample_num);
  return all_error;
}


void m_bp_neural_network::back_propagation(m_matrix &result,float (*funcd)(float))
{
  //m_matrix cost = this->cost(result);
  //cout<<"Cost///////////////////////"<<endl;
  //cost.print();
  //alpha learnRate
  int kn = this->net_num+1;
  m_matrix* layersNext = new m_matrix[kn];
  m_matrix* basesNext = new m_matrix[kn];
  m_matrix error = this->error(result);
  m_matrix error_abs = error;
  error_abs.filter(powf2);
  float min_err = error_abs.min();
  if(min_err<0.0000001){
    //cout<<"skip"<<endl;
    return;
  }
  error_abs.mul(0.5f);
  m_matrix all_error = this->errors;
  this->errors.release();
  this->errors = all_error.add(error_abs);
  error.mul(-1);
  //m_matrix net0 = this->layers_net_input[kn];
  //net0.filter(funcd);
  //m_matrix S = error.mul_v(net0);
  m_matrix S = error;
  for(int k=kn-1;k>=0;k--)
  {
    //cout<<"S///////////////////////"<<endl;
    //S.print();
    //this->layers_f_input[k].print();
    m_matrix f_i_cur = this->layers_f_input[k].transpose();
    //f_i_cur.print();
    //cout<<k<<"|dW///////////////////////"<<endl;
    m_matrix dW = f_i_cur.mul(S);
    //dW.print();
    //cout<<k<<"|dB///////////////////////"<<endl;
    m_matrix dB = S;
    //dB.print();
    //cout<<k<<"|///////////////////////"<<endl;
    //this->delta_layers[k].print();
    //cout<<k<<"|///////////////////////"<<endl;
    
    layersNext[k]=this->delta_layers[k].add(dW);
    basesNext[k]=this->delta_bases[k].add(dB);
    //layersNext[k].print();
    m_matrix net_cur = this->layers_net_input[(k)];
    m_matrix W_cur = this->layers[k].transpose(); 
    net_cur.filter(funcd);
    m_matrix Ws = S.mul(W_cur);
    S.release();
    S = Ws.mul_v(net_cur);
  }
  delete [] this->delta_layers;
  delete [] this->delta_bases;
  this->delta_layers = layersNext;
  this->delta_bases = basesNext;
  this->sample_num++;
  //this->delta_layers[0].print();
  //cout<<endl<<endl;
}

float lnf(float x)
{
  return logf(x)/logf(M_E);
}

m_matrix m_bp_neural_network::cross_entropy_cost(m_matrix &result)
{
  int kn = this->net_num+1;
  m_matrix re = result;
  for(int i=0;i<result.width;i++)
  {
    float y = result.get(i,0);
    float a = this->layers_f_input[kn].get(i,0);
    re.set(i,0,-(y*lnf(a)+(1-y)*lnf(1-a)));
  }
  return re;
}

void m_bp_neural_network::clean_delta()
{
  
  int kn = this->net_num+1;
  for(int i=0;i<kn;i++)
  {
    this->last_delta_layers[i].set_array(this->delta_layers[i].data(),this->delta_layers[i].width*this->delta_layers[i].height);
    this->delta_layers[i].mul(0);
    this->delta_bases[i].mul(0);
  }
  this->sample_num = 0;
  this->errors.mul(0);
}

void m_bp_neural_network::update()
{
  if(this->sample_num==0)return;
  int kn = this->net_num+1;
  float ka = -this->alpha/this->sample_num;
  float ks = -this->sigma/this->sample_num;
  m_matrix* layersNext = new m_matrix[kn];
  m_matrix* basesNext = new m_matrix[kn];
  for(int i=0;i<kn;i++)
  {
    this->delta_layers[i].mul(ka);
    this->delta_bases[i].mul(ka);
    m_matrix temp = this->layers[i];
    temp.mul(-this->alpha*this->lambda);
    m_matrix last = this->last_delta_layers[i];
    last.mul(ks);
    layersNext[i] = this->layers[i].add(this->delta_layers[i]).add(last);
    basesNext[i] = this->bases[i].add(this->delta_bases[i]);
  }
  delete [] this->layers;
  delete [] this->bases;
  this->layers = layersNext;
  this->bases = basesNext;
  this->clean_delta();
}