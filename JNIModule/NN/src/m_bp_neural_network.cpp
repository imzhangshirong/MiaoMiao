#include "../include/m_bp_neural_network.h"
#include <vector>
#include <iostream>
#include <armadillo>
#include <math.h>

using namespace std;
using namespace arma;

m_bp_neural_network::m_bp_neural_network(int input_num, initializer_list<int> net_num, initializer_list<string> net_func)
{
  this->step = step;
  int n_layers = net_num.size();
  this->layers_func = new string[n_layers];
  this->layers = new mat[n_layers];
  this->bases = new mat[n_layers];
  this->hessian_layers = new mat[n_layers];

  this->layers_f_input = new mat[n_layers + 1];
  this->layers_net_input = new mat[n_layers + 1];
  this->n_input = input_num;
  this->n_layers = n_layers;
  mat r(1, input_num);

  this->layers_net_input[0] = r;
  this->layers_f_input[0] = r;

  const int *nets_c = net_num.begin();
  const string *nets_func = net_func.begin();
  this->n_output = *(nets_c + n_layers - 1);

  mat w(input_num, *(nets_c));
  mat b(1, *(nets_c));
  r = mat(1, *(nets_c));
  this->layers[0] = w;
  this->layers_func[0] = *(nets_func);
  this->bases[0] = b;
  this->layers_net_input[1] = r;
  this->layers_f_input[1] = r;
  for (int i = 0; i < n_layers - 2; i++)
  {
    w = mat(*(nets_c + i), *(nets_c + i + 1));
    b = mat(1, *(nets_c + i + 1));
    r = mat(1, *(nets_c + i + 1));
    this->layers[i + 1] = w;
    this->layers_func[i + 1] = *(nets_func + i + 1);
    this->bases[i + 1] = b;
    this->layers_f_input[i + 2] = r;
    this->layers_net_input[i + 2] = r;
  }
  w = mat(*(nets_c + n_layers - 2), this->n_output);
  b = mat(1, this->n_output);
  r = mat(1, this->n_output);
  this->layers[n_layers - 1] = w;
  this->layers_func[n_layers - 1] = *(nets_func + n_layers - 2);
  this->bases[n_layers - 1] = b;
  this->layers_f_input[n_layers] = r;
  this->layers_net_input[n_layers] = r;

  this->delta_layers = new mat[n_layers];
  this->last_delta_layers = new mat[n_layers];
  this->delta_bases = new mat[n_layers];
  for (int i = 0; i < n_layers; i++)
  {
    this->delta_layers[i] = mat(this->layers[i].n_rows, this->layers[i].n_cols);
    this->last_delta_layers[i] = mat(this->layers[i].n_rows, this->layers[i].n_cols);
    this->delta_bases[i] = mat(this->bases[i].n_rows, this->bases[i].n_cols);
  }
}

double sigmoid_tanh(double x)
{
  double v = tanhf(x);
  return v;
}

double dsigmoid_tanh(double x)
{
  double d = sigmoid_tanh(x);
  return 1 - d * d;
}

double sigmoid(double x)
{
  double v = 1 / (1 + exp(-x));
  return v;
}

double linear(double x)
{
  return x;
}

double dlinear(double x)
{
  return 1;
}

double dsigmoid(double x)
{
  double d = sigmoid(x);
  return d * (1 - d);
}

void mapper(mat &mat, double (*func)(double))
{
  for (int i = 0; i < mat.n_rows; i++)
  {
    for (int j = 0; j < mat.n_cols; j++)
    {
      mat(i, j) = func(mat(i, j));
    }
  }
}

void sig_func(string name, mat &input)
{
  double (*func)(double) = NULL;
  if (name == "logsig")
  {
    func = sigmoid;
  }
  else if (name == "tansig")
  {
    func = sigmoid_tanh;
  }
  else if (name == "linear")
  {
    func = linear;
  }
  mapper(input, func);
}

void sig_dfunc(string name, mat &input)
{
  double (*func)(double) = NULL;

  if (name == "logsig")
  {
    func = dsigmoid;
  }
  else if (name == "tansig")
  {
    func = dsigmoid_tanh;
  }
  else if (name == "linear")
  {
    func = dlinear;
  }
  mapper(input, func);
}

double init_rand(double x)
{
  return (x - 0.5) * 2;
}

void m_bp_neural_network::init()
{
  for (int i = 0; i < this->n_layers; i++)
  {
    this->layers[i].randu();
    mapper(this->layers[i], init_rand);
    this->bases[i].randu();
    mapper(this->bases[i], init_rand);
  }
}

void m_bp_neural_network::print()
{
  cout << "\n/////////////////INPUT/////////////////" << endl;
  layers_f_input[0].print();
  cout << "=======================================" << endl;
  for (int i = 0; i < this->n_layers; i++)
  {
    layers[i].print();
    cout << "--------------------------" << endl;
    bases[i].print();
    cout << "--------------------------OUT" << endl;
    layers_net_input[i + 1].print();
    cout << "--------------------------Func OUT" << endl;
    layers_f_input[i + 1].print();
    cout << "=======================================" << endl;
  }
}

mat m_bp_neural_network::sim(mat &input)
{
  mat re = input;
  for (int i = 0; i < this->n_layers; i++)
  {
    mat m1 = re * this->layers[i];
    mat m2 = m1 + this->bases[i];
    sig_func(this->layers_func[i], m2);
    re = m2;
  }
  return re;
}

void m_bp_neural_network::sim(mat &input, int index)
{
  this->layers_f_input[0].row(index) = input;
  this->layers_net_input[0].row(index) = input;
  for (int i = 0; i < this->n_layers; i++)
  {
    mat m1 = this->layers_f_input[i].row(index) * this->layers[i];
    mat m2 = m1 + this->bases[i];
    this->layers_net_input[i + 1].row(index) = m2;
    sig_func(this->layers_func[i], m2);
    this->layers_f_input[i + 1].row(index) = m2;
  }
}

double pow2(double x)
{
  return pow(x, 2);
}
//train_func : traingd,trainlm
void m_bp_neural_network::train(string train_func, vector<mat> &input, vector<mat> &result, int max_epoch, double alpha)
{
  this->stop_train = false;
  int kn = this->n_layers;
  this->step = alpha;
  int sample_num = input.size();
  for (int i = 0; i < kn; i++)
  {
    this->hessian_layers[i].set_size(sample_num, this->layers[i].n_cols * (this->layers[i].n_rows + 1));
  }
  for (int i = 0; i < kn + 1; i++)
  {
    this->layers_f_input[i].set_size(sample_num, this->layers_f_input[i].n_cols);
    this->layers_net_input[i].set_size(sample_num, this->layers_f_input[i].n_cols);
  }
  this->mse.set_size(sample_num, 1);
  this->errors.set_size(sample_num, this->n_output);
  this->forward(input, result);
  this->mse_v = as_scalar(sum(sum(this->mse))) / sample_num;
  for (int i = 0; i < max_epoch; i++)
  {
    cout << i + 1 << " => MSE:" << this->mse_v << endl;
    this->back_propagation(sample_num);
    this->update(train_func, input, result);
    if (this->stop_train)
    {
      cout << i + 1 << " End Training!" << endl;
      break;
    }
  }
}

void m_bp_neural_network::forward(vector<mat> &input, vector<mat> &result)
{
  int kn = this->n_layers;
  int sample_num = input.size();

  for (int j = 0; j < sample_num; j++)
  {
    this->sim(input[j], j);
    mat error = result[j] - this->layers_f_input[kn].row(j);
    mat mse = error;
    mapper(mse, pow2);
    this->mse(j, 0) = as_scalar(sum(sum(mse))) / 2;
    this->errors.row(j) = error;
  }
}

void m_bp_neural_network::back_propagation(int sample_num)
{
  int kn = this->n_layers;
  //this->errors.print("eeeeeeeeeeee");
  for (int i = 0; i < sample_num; i++)
  {
    mat error = this->errors.row(i);
    mat net0 = this->layers_net_input[kn].row(i);
    sig_dfunc(this->layers_func[kn - 1], net0);
    mat S = net0 % error * (-1);
    for (int k = kn - 1; k >= 0; k--)
    {
      mat f_i_cur = this->layers_f_input[k].row(i);
      mat St = S.t();
      mat dW = St * f_i_cur;
      mat dB = St;
      mat dWB = mat(dW.n_rows, dW.n_cols + 1);
      dWB.cols(0, dW.n_cols - 1) = dW;

      dWB.col(dWB.n_cols - 1) = dB;
      dWB.set_size(dWB.n_rows * dWB.n_cols, 1);
      dWB = dWB.t();

      this->hessian_layers[k].row(i) = dWB;
      mat net_cur = this->layers_net_input[(k)].row(i);
      mat W_cur = this->layers[k].t();
      if (k > 0)
      {
        sig_dfunc(this->layers_func[k], net_cur);
        mat Ws = S * W_cur;
        S = Ws % (net_cur);
      }
    }
  }
}

double lnf(double x)
{
  return logf(x) / logf(M_E);
}

void m_bp_neural_network::update(string type, vector<mat> &input, vector<mat> &result)
{

  int kn = this->n_layers;

  int max_step_times = 100;
  int sample_num = input.size();
  if (type == "trainlm")
  {
    double mu_step = 2;
    int cols = 0;
    for (int i = 0; i < kn; i++)
    {
      cols += this->hessian_layers[i].n_cols;
    }
    mat hessian = mat(sample_num, cols);
    int last_cols = 0;
    for (int i = 0; i < kn; i++)
    {
      hessian.cols(last_cols, last_cols + this->hessian_layers[i].n_cols - 1) = this->hessian_layers[i];
      last_cols += this->hessian_layers[i].n_cols;
    }
    mat hessian_trans = hessian.t();
    mat HtH = hessian_trans * hessian;
    mat kI;
    kI.copy_size(HtH);
    kI.eye();
    mat delta = hessian_trans * this->mse;

    double old_step = this->step;
    int k = 0;
    for (k = 0; k < max_step_times; k++)
    {
      mat Hi = inv(HtH + this->step * kI);
      mat delta_k = Hi * delta;
      delta_k = delta_k.t();
      //delta_k.print("MMMMMMM");
      last_cols = 0;
      for (int i = 0; i < kn; i++)
      {

        mat dWB = delta_k.cols(last_cols, last_cols + this->hessian_layers[i].n_cols - 1);
        //dW.print("dW");
        dWB.set_size(this->layers[i].n_cols, this->layers[i].n_rows + 1);
        dWB = dWB.t();
        mat dB = dWB.row(dWB.n_rows - 1);
        mat dW = mat(dWB.n_rows - 1, dWB.n_cols);
        dW = dWB.rows(0, dW.n_rows - 1);
        //dB.print("dB");
        this->delta_layers[i] = dW;
        this->delta_bases[i] = dB;
        //this->layers[i].print();
        //this->bases[i].print();
        this->layers[i] -= dW;
        this->bases[i] -= dB;
        last_cols += this->hessian_layers[i].n_cols;
      }
      forward(input, result);
      double mse_ = as_scalar(sum(sum(this->mse))) / sample_num;

      if (mse_ < this->mse_v)
      {
        this->step /= mu_step;
        cout << this->step << "|" << this->mse_v << " => " << mse_ << endl;
        this->mse_v = mse_;
        break;
      }
      for (int i = kn - 1; i >= 0; i--)
      {
        this->layers[i] += this->delta_layers[i];
        this->bases[i] += this->delta_bases[i];
      }
      this->step *= mu_step;
    }
    if (k >= max_step_times && this->step > old_step)
    {
      this->step = old_step;
      this->stop_train = true;
    }
  }
  else if (type == "traingd")
  {
    for (int i = 0; i < kn; i++)
    {
      this->delta_layers[i].fill(0);
      this->delta_bases[i].fill(0);
      for (int j = 0; j < sample_num; j++)
      {
        mat dWB = this->hessian_layers[i].row(j);
        dWB.set_size(this->layers[i].n_cols, this->layers[i].n_rows + 1);

        dWB = dWB.t();
        mat dB = dWB.row(dWB.n_rows - 1);
        mat dW = mat(dWB.n_rows - 1, dWB.n_cols);
        dW = dWB.rows(0, dW.n_rows - 1);
        this->delta_layers[i] += dW;
        this->delta_bases[i] += dB;
      }
    }
    double old_step = this->step;
    int k = 0;
    double alpha = this->step / sample_num;
    for (int i = 0; i < kn; i++)
    {
      this->layers[i] -= alpha * this->delta_layers[i];
      this->bases[i] -= alpha * this->delta_bases[i];
    }
    forward(input, result);
    double mse_ = as_scalar(sum(sum(this->mse))) / sample_num;
    cout << this->step << "|" << this->mse_v << " => " << mse_ << endl;
    this->mse_v = mse_;
  }
}